#include "include.h"




mtx_t queue_mutex;
cnd_t job_available;
const int MAX_JOBS = 1000;
bool RUNNING = true;
//JobList* job_list;
Job* job_head = NULL;
Job* job_tail = NULL;

// --- RESULTS QUEUE (New) ---
mtx_t result_mutex;
cnd_t result_ready;
mtx_t chunk_table_mutex;
JobResult* result_queue_head = NULL; // Head of the results queue
JobResult* result_queue_tail = NULL; // Tail of the results queue


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

    if (job == NULL) return NULL; // queue is empty

    job_head = job_head->next_job_entry;

    if (job_head == NULL) {
        job_tail = NULL;   //if job head is null, queue is empty, so tail must be null
    }

    return job;
}

bool job_queue_empty() {
    return job_head == NULL;
}


// --- NEW RESULTS QUEUE FUNCTIONS ---
void result_push(JobResult* result) {
    result->next_result_entry = NULL; 
    mtx_lock(&result_mutex);

    // [Your queue insertion logic here, similar to job_push]
    if (result_queue_tail == NULL) {
        result_queue_head = result;
        result_queue_tail = result;
    } else {
        result_queue_tail->next_result_entry = result;
        result_queue_tail = result;
    }
    
    mtx_unlock(&result_mutex);

    // CRITICAL: Signal that a result is ready
    cnd_signal(&result_ready); // ADD THIS
}

JobResult* result_pop() {
    // Implement logic similar to job_pop
    mtx_lock(&result_mutex); // Must lock when modifying the results queue
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
    // Lock needed for thread-safe access to check the head
    mtx_lock(&result_mutex); 
    bool empty = result_queue_head == NULL; 
    mtx_unlock(&result_mutex);
    return empty;
}


int chunk_generator_worker(void* arg) {
    mtx_lock(&queue_mutex);

    while(RUNNING) {
        //wait for job
        while(job_queue_empty()) {
            cnd_wait(&job_available, &queue_mutex);
        }

        //job is available! take it from queue
        Job* job = job_pop();
        // Cache job coordinates before unlocking the queue mutex
        int j_cx = job->cx;
        int j_cy = job->cy;
        int j_cz = job->cz;
        ChunkTable* j_table = job->table;

        mtx_unlock(&queue_mutex); //release lock, exiting critical section

        Chunk* chunk = create_chunk(job);
        if (chunk == NULL) { 
            free(job);
            mtx_lock(&queue_mutex); // Re-lock before looping
            continue;
        }

        // Generate the raw mesh buffers (CPU-only part from meshmanager.c)
        JobResult* result = GenerateRawMesh(job->table, chunk); 
        free(job); // Job finished, free the request object

        if (result != NULL) {
            // CRITICAL FIX: Assign coordinates for the main thread to use
            result->cx = j_cx;
            result->cy = j_cy;
            result->cz = j_cz;

            // 4. Submit the raw mesh data to the results queue
            result_push(result);
            chunk->state = CHUNK_STATE_BLOCKS_READY; 
        } else {
            // If meshing failed, the chunk is leaked. Free it here.
            free(chunk); // Or just free(chunk) if DestroyChunk is not defined
        }

        // mtx_lock(&queue_mutex);
        // add_chunk(job->table, job->cx, job->cy, job->cz, new_chunk);
        // mtx_unlock(&queue_mutex);

        mtx_lock(&queue_mutex);
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
    job->next_job_entry = NULL; // Initialize pointer
    return job;
}

