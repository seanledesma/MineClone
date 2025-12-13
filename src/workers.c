#include "include.h"

mtx_t queue_mutex;
cnd_t job_available;
const int MAX_JOBS = 1000;
bool RUNNING = true;
Job* job_head = NULL;
Job* job_tail = NULL;

mtx_t result_mutex;
cnd_t result_ready;
mtx_t chunk_table_mutex;
JobResult* result_queue_head = NULL;
JobResult* result_queue_tail = NULL;

// Track pending jobs to prevent duplicates
typedef struct PendingJob {
    int cx, cy, cz;
    struct PendingJob* next;
} PendingJob;

PendingJob* pending_head = NULL;

bool is_job_pending(int cx, int cy, int cz) {
    PendingJob* current = pending_head;
    while (current) {
        if (current->cx == cx && current->cy == cy && current->cz == cz) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void add_pending_job(int cx, int cy, int cz) {
    PendingJob* pending = (PendingJob*)malloc(sizeof(PendingJob));
    if (!pending) return;
    pending->cx = cx;
    pending->cy = cy;
    pending->cz = cz;
    pending->next = pending_head;
    pending_head = pending;
}

static void remove_pending_job(int cx, int cy, int cz) {
    PendingJob* current = pending_head;
    PendingJob* prev = NULL;
    
    while (current) {
        if (current->cx == cx && current->cy == cy && current->cz == cz) {
            if (prev) {
                prev->next = current->next;
            } else {
                pending_head = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void job_push(Job* job) {
    job->next_job_entry = NULL;

    if(job_tail == NULL) {
        job_head = job;
        job_tail = job;
    } else {
        job_tail->next_job_entry = job;
        job_tail = job;
    }
}

Job* job_pop() {
    Job* job = job_head;

    if (job == NULL) return NULL;

    job_head = job_head->next_job_entry;

    if (job_head == NULL) {
        job_tail = NULL;
    }

    return job;
}

bool job_queue_empty() {
    return job_head == NULL;
}

void result_push(JobResult* result) {
    result->next_result_entry = NULL; 
    mtx_lock(&result_mutex);

    if (result_queue_tail == NULL) {
        result_queue_head = result;
        result_queue_tail = result;
    } else {
        result_queue_tail->next_result_entry = result;
        result_queue_tail = result;
    }
    
    mtx_unlock(&result_mutex);
    cnd_signal(&result_ready);
}

JobResult* result_pop() {
    mtx_lock(&result_mutex);
    JobResult* result = result_queue_head;
    if (result == NULL) {
        mtx_unlock(&result_mutex);
        return NULL;
    } 

    result_queue_head = result->next_result_entry; 
    if (result_queue_head == NULL) result_queue_tail = NULL;
    
    result->next_result_entry = NULL;
    mtx_unlock(&result_mutex);
    return result;
}

bool result_queue_empty() {
    mtx_lock(&result_mutex); 
    bool empty = result_queue_head == NULL; 
    mtx_unlock(&result_mutex);
    return empty;
}

int chunk_generator_worker(void* arg) {
    (void)arg; // Suppress unused parameter warning
    
    mtx_lock(&queue_mutex);

    while(RUNNING) {
        while(job_queue_empty() && RUNNING) {
            cnd_wait(&job_available, &queue_mutex);
        }
        
        if (!RUNNING) break; // Exit if shutting down

        Job* job = job_pop();
        if (job == NULL) {
            continue;
        }
        
        // Cache coordinates BEFORE unlocking
        int j_cx = job->cx;
        int j_cy = job->cy;
        int j_cz = job->cz;
        ChunkTable* j_table = job->table;

        mtx_unlock(&queue_mutex);

        // Check if chunk already exists - if so, this is a REGENERATION
        Chunk* existing = get_chunk(j_table, j_cx, j_cy, j_cz);
        
        Chunk* chunk_to_mesh = NULL;
        bool is_new_chunk = false;
        
        if (existing != NULL) {
            // REGENERATION: Use existing chunk with its current block data
            chunk_to_mesh = existing;
            is_new_chunk = false;
        } else {
            // NEW CHUNK: Create fresh chunk with terrain generation
            chunk_to_mesh = create_chunk(job);
            is_new_chunk = true;
        }
        
        free(job);
        
        if (chunk_to_mesh == NULL) { 
            mtx_lock(&queue_mutex);
            remove_pending_job(j_cx, j_cy, j_cz);
            continue;
        }

        // FIX: For regeneration, we need to snapshot the block data to avoid
        // race conditions with the main thread modifying blocks.
        // For new chunks, the data is safe since no one else has access yet.
        //
        // NOTE: GenerateRawMesh also reads neighbor chunks for face culling.
        // Those reads are protected by the mutex in get_chunk and IsBlockAir.
        // The block modification in player.c now locks chunk_table_mutex.
        
        // Generate the raw mesh buffers from the chunk's block data
        JobResult* result = GenerateRawMesh(j_table, chunk_to_mesh); 

        if (result != NULL) {
            result->cx = j_cx;
            result->cy = j_cy;
            result->cz = j_cz;
            result->table = j_table;
            result->is_new_chunk = is_new_chunk;

            // Submit the raw mesh data to the results queue
            result_push(result);
            
            // FIX: DO NOT change chunk state here!
            // The chunk stays in CHUNK_STATE_REGENERATING (which is drawable)
            // until the main thread processes the result and uploads the mesh.
            // This prevents the chunk from becoming non-drawable during the
            // window between result_push and ProcessJobResults.
            //
            // OLD BUGGY CODE (removed):
            // chunk_to_mesh->state = CHUNK_STATE_BLOCKS_READY; 
            
        } else {
            // If meshing failed and it was a new chunk, free it
            if (is_new_chunk) {
                free(chunk_to_mesh);
            }
        }

        mtx_lock(&queue_mutex);
        remove_pending_job(j_cx, j_cy, j_cz);
    }
    mtx_unlock(&queue_mutex);
    return 0;
}

Job* create_job(ChunkTable* table, int cx, int cy, int cz) {
    Job* job = (Job*)malloc(sizeof(Job));
    if (job == NULL) return NULL;
    job->table = table;
    job->cx = cx;
    job->cy = cy;
    job->cz = cz;
    job->next_job_entry = NULL;
    return job;
}

// Function to safely request a chunk job (handles deduplication)
bool request_chunk_job(ChunkTable* table, int cx, int cy, int cz) {
    // First check if chunk already exists and is ready or regenerating
    Chunk* existing = get_chunk(table, cx, cy, cz);
    if (existing != NULL && (existing->state == CHUNK_STATE_READY || 
                              existing->state == CHUNK_STATE_REGENERATING)) {
        return false; // Already have this chunk with valid mesh
    }
    
    mtx_lock(&queue_mutex);
    
    // Check if job already pending
    if (is_job_pending(cx, cy, cz)) {
        mtx_unlock(&queue_mutex);
        return false;
    }
    
    // Create and queue the job
    Job* job = create_job(table, cx, cy, cz);
    if (job) {
        add_pending_job(cx, cy, cz);
        job_push(job);
        cnd_signal(&job_available);
    }
    
    mtx_unlock(&queue_mutex);
    return job != NULL;
}