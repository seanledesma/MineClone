#include "include.h"

Vector3 nearbyChunks [NEARBY_CHUNK_ARRAY_SIZE]; 
int nearbyChunkCount = NEARBY_CHUNK_ARRAY_SIZE;

size_t hash_index(uint64_t key) {
    return key % HASH_TABLE_SIZE;
}

void add_chunk(ChunkTable* table, int cx, int cy, int cz, Chunk* chunk) {
    mtx_lock(&chunk_table_mutex);
    
    // Check if chunk already exists to avoid duplicates
    uint64_t key = chunk_hash(cx, cy, cz);
    int index = hash_index(key);
    ChunkEntry *existing = table->buckets[index];
    while(existing != NULL) {
        if(existing->key == key) {
            // Chunk already exists, don't add duplicate
            mtx_unlock(&chunk_table_mutex);
            return;
        }
        existing = existing->next_chunk_entry;
    }
    
    ChunkEntry* new_entry = (ChunkEntry*)malloc(sizeof(ChunkEntry));
    if (!new_entry) {
        mtx_unlock(&chunk_table_mutex);
        return;
    }
    new_entry->key = key;
    new_entry->cx = cx;
    new_entry->cy = cy;
    new_entry->cz = cz;
    new_entry->chunk = chunk;
    new_entry->next_chunk_entry = NULL;

    new_entry->next_chunk_entry = table->buckets[index];
    table->buckets[index] = new_entry;
    mtx_unlock(&chunk_table_mutex);
}

Chunk *get_chunk(ChunkTable* table, int cx, int cy, int cz) {
    mtx_lock(&chunk_table_mutex);
    uint64_t key = chunk_hash(cx, cy, cz);
    int index = hash_index(key);
    ChunkEntry *current_entry = table->buckets[index];
    while(current_entry != NULL) {
        if(current_entry->key == key) {
            Chunk* result = current_entry->chunk;
            mtx_unlock(&chunk_table_mutex);
            return result;
        }
        current_entry = current_entry->next_chunk_entry;
    }
    mtx_unlock(&chunk_table_mutex);
    return NULL;
}

// Non-locking version for use when mutex is already held
Chunk *get_chunk_unlocked(ChunkTable* table, int cx, int cy, int cz) {
    uint64_t key = chunk_hash(cx, cy, cz);
    int index = hash_index(key);
    ChunkEntry *current_entry = table->buckets[index];
    while(current_entry != NULL) {
        if(current_entry->key == key) {
            return current_entry->chunk;
        }
        current_entry = current_entry->next_chunk_entry;
    }
    return NULL;
}

void remove_chunk(ChunkTable* table, int cx, int cy, int cz) {
    mtx_lock(&chunk_table_mutex);
    uint64_t key = chunk_hash(cx, cy, cz);
    int index = hash_index(key);

    ChunkEntry *current_entry = table->buckets[index];
    if (current_entry == NULL) {
        mtx_unlock(&chunk_table_mutex);
        return;
    }

    if(current_entry->key == key) {
        table->buckets[index] = current_entry->next_chunk_entry;
        free(current_entry->chunk);
        free(current_entry);
        mtx_unlock(&chunk_table_mutex);
        return;
    }
    
    ChunkEntry *prev = current_entry;
    current_entry = current_entry->next_chunk_entry;
    while(current_entry != NULL) {
        if(current_entry->key == key) {
            prev->next_chunk_entry = current_entry->next_chunk_entry;
            free(current_entry->chunk);
            free(current_entry);
            mtx_unlock(&chunk_table_mutex);
            return;
        }
        prev = current_entry;
        current_entry = current_entry->next_chunk_entry;
    }
    mtx_unlock(&chunk_table_mutex);
}

Chunk* create_chunk(Job* job) {
    Chunk* new_chunk = (Chunk*)malloc(sizeof(Chunk));
    memset(new_chunk, 0, sizeof(Chunk));
    new_chunk->table_pos = (Vector3) { job->cx, job->cy, job->cz };
    new_chunk->world_pos = (Vector3) { job->cx * CHUNK_SIZE, (job->cy * CHUNK_SIZE), job->cz * CHUNK_SIZE };
    
    float baseX = -(CHUNK_SIZE / 2) + 0.5f;
    float baseY = -(CHUNK_SIZE / 2) + 0.5f;
    float baseZ = -(CHUNK_SIZE / 2) + 0.5f;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                new_chunk->blocks[x][y][z].pos = (Vector3) { 
                    new_chunk->world_pos.x + baseX, 
                    new_chunk->world_pos.y + baseY, 
                    new_chunk->world_pos.z + baseZ };

                int absolute_x = (job->cx * CHUNK_SIZE) + x - HALF_CHUNK;
                int absolute_y = (job->cy * CHUNK_SIZE) + y - (HALF_CHUNK - 1);
                int absolute_z = (job->cz * CHUNK_SIZE) + z - HALF_CHUNK;
                new_chunk->blocks[x][y][z].blockType = DecideBlockType(new_chunk, absolute_x, absolute_y, absolute_z);

                baseZ++;
            }
            baseZ = -(CHUNK_SIZE / 2) + 0.5f;
            baseY++;
        }
        baseY = -(CHUNK_SIZE / 2) + 0.5f;
        baseX++;
    }

    return new_chunk;
}

Chunk *get_current_chunk(ChunkTable *table, int cx, int cy, int cz) {
    Chunk *chunk = get_chunk(table, cx, cy, cz);
    if (chunk == NULL) {
        request_chunk_job(table, cx, cy, cz);
        return NULL;
    }
    return chunk;
}

void UpdateNearbyChunks(int cx, int cy, int cz) {
    memset(nearbyChunks, 0, sizeof(nearbyChunks));
    int tracker = 0;
    for (int x = -CHUNK_RENDER_MAX + cx; x <= CHUNK_RENDER_MAX + cx; x++) {
        for (int y = -CHUNK_RENDER_MAX + cy; y <= CHUNK_RENDER_MAX + cy; y++) {
            for (int z = -CHUNK_RENDER_MAX + cz; z <= CHUNK_RENDER_MAX + cz; z++) {
                nearbyChunks[tracker++] = (Vector3) { x, y, z };
            }
        }
    }
    nearbyChunkCount = tracker;
}

void CleanupChunkTable(ChunkTable* table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ChunkEntry* entry = table->buckets[i];
        while (entry != NULL) {
            ChunkEntry* next = entry->next_chunk_entry;
            
            if (entry->chunk) {
                if (entry->chunk->grassModel.meshCount > 0) {
                    UnloadModel(entry->chunk->grassModel);
                }
                if (entry->chunk->dirtModel.meshCount > 0) {
                    UnloadModel(entry->chunk->dirtModel);
                }
                if (entry->chunk->stoneModel.meshCount > 0) {
                    UnloadModel(entry->chunk->stoneModel);
                }
                
                free(entry->chunk);
            }
            
            free(entry);
            entry = next;
        }
    }
}

// Queue for models that need to be unloaded on the main thread
typedef struct DeferredUnload {
    Model grass;
    Model dirt;
    Model stone;
    struct DeferredUnload* next;
} DeferredUnload;

static DeferredUnload* deferred_unload_head = NULL;
static mtx_t deferred_unload_mutex;
static bool deferred_unload_initialized = false;

void InitDeferredUnload(void) {
    if (!deferred_unload_initialized) {
        mtx_init(&deferred_unload_mutex, mtx_plain);
        deferred_unload_initialized = true;
    }
}

void QueueDeferredUnload(Model grass, Model dirt, Model stone) {
    DeferredUnload* item = (DeferredUnload*)malloc(sizeof(DeferredUnload));
    if (!item) return;
    
    item->grass = grass;
    item->dirt = dirt;
    item->stone = stone;
    
    mtx_lock(&deferred_unload_mutex);
    item->next = deferred_unload_head;
    deferred_unload_head = item;
    mtx_unlock(&deferred_unload_mutex);
}

void ProcessDeferredUnloads(void) {
    mtx_lock(&deferred_unload_mutex);
    DeferredUnload* item = deferred_unload_head;
    deferred_unload_head = NULL;
    mtx_unlock(&deferred_unload_mutex);
    
    while (item) {
        DeferredUnload* next = item->next;
        
        if (item->grass.meshCount > 0) UnloadModel(item->grass);
        if (item->dirt.meshCount > 0) UnloadModel(item->dirt);
        if (item->stone.meshCount > 0) UnloadModel(item->stone);
        
        free(item);
        item = next;
    }
}

void ProcessJobResults(ChunkTable* table) {
    JobResult* result = result_pop();
    while(result != NULL) {
        Chunk* target_chunk = NULL;
        bool should_update_neighbors = false;
        
        if (result->is_new_chunk) {
            // NEW CHUNK: Add the worker's chunk to the table
            Chunk* worker_chunk = result->chunk;
            
            // Double-check it wasn't added by another thread
            Chunk* existing = get_chunk(table, result->cx, result->cy, result->cz);
            if (existing == NULL) {
                add_chunk(table, result->cx, result->cy, result->cz, worker_chunk);
                target_chunk = worker_chunk;
                should_update_neighbors = true;
            } else {
                // Race condition: chunk was added by someone else, free ours
                free(worker_chunk);
                target_chunk = existing;
            }
        } else {
            // REGENERATION: Use existing chunk (result->chunk points to it)
            target_chunk = result->chunk;
        }
        
        if (target_chunk) {
            // FIX: Build new meshes into TEMPORARY variables first
            // This way the chunk's actual mesh/model pointers stay valid
            // until we do a single atomic swap at the end
            
            Mesh new_grassMesh = {0};
            Mesh new_dirtMesh = {0};
            Mesh new_stoneMesh = {0};
            Model new_grassModel = {0};
            Model new_dirtModel = {0};
            Model new_stoneModel = {0};
            
            // Upload new meshes into temporaries
            FinalizeAndUploadMesh(&new_grassMesh, &new_grassModel, 
                                 result->grass_vertices, result->grass_normals, result->grass_uvs, 
                                 result->grass_colors, result->grass_vert_count, grassTex);
            
            FinalizeAndUploadMesh(&new_dirtMesh, &new_dirtModel, 
                                 result->dirt_vertices, result->dirt_normals, result->dirt_uvs, 
                                 result->dirt_colors, result->dirt_vert_count, dirtTex);
            
            FinalizeAndUploadMesh(&new_stoneMesh, &new_stoneModel, 
                                 result->stone_vertices, result->stone_normals, result->stone_uvs, 
                                 result->stone_colors, result->stone_vert_count, stoneTex);
            
            // Store old models for cleanup
            Model old_grass = target_chunk->grassModel;
            Model old_dirt = target_chunk->dirtModel;
            Model old_stone = target_chunk->stoneModel;
            bool had_old_models = (old_grass.meshCount > 0 || 
                                   old_dirt.meshCount > 0 || 
                                   old_stone.meshCount > 0);
            
            // ATOMIC SWAP: Replace all mesh/model pointers at once
            // After this point, rendering will use the new meshes
            target_chunk->grassMesh = new_grassMesh;
            target_chunk->dirtMesh = new_dirtMesh;
            target_chunk->stoneMesh = new_stoneMesh;
            target_chunk->grassModel = new_grassModel;
            target_chunk->dirtModel = new_dirtModel;
            target_chunk->stoneModel = new_stoneModel;
            
            // NOW safe to unload old models (new ones are active)
            if (had_old_models) {
                if (old_grass.meshCount > 0) UnloadModel(old_grass);
                if (old_dirt.meshCount > 0) UnloadModel(old_dirt);
                if (old_stone.meshCount > 0) UnloadModel(old_stone);
            }
                                     
            target_chunk->state = CHUNK_STATE_READY;
            
            // FIX: Check if another remesh was requested while we were working
            if (target_chunk->needsRemesh) {
                target_chunk->needsRemesh = false;
                RegenerateChunk(table, target_chunk);
            }
            
            // If this was a new chunk, neighbors need to update their boundary faces
            if (should_update_neighbors) {
                int cx = result->cx;
                int cy = result->cy;
                int cz = result->cz;
                
                // Check each neighbor - if they exist, they need to remesh
                // FIX: Also regenerate chunks that are REGENERATING (they'll get needsRemesh set)
                Chunk* neighbor;
                
                #define MAYBE_REGEN(n) do { \
                    if ((n) && ((n)->state == CHUNK_STATE_READY || (n)->state == CHUNK_STATE_REGENERATING)) \
                        RegenerateChunk(table, (n)); \
                } while(0)
                
                neighbor = get_chunk(table, cx - 1, cy, cz);
                MAYBE_REGEN(neighbor);
                neighbor = get_chunk(table, cx + 1, cy, cz);
                MAYBE_REGEN(neighbor);
                neighbor = get_chunk(table, cx, cy - 1, cz);
                MAYBE_REGEN(neighbor);
                neighbor = get_chunk(table, cx, cy + 1, cz);
                MAYBE_REGEN(neighbor);
                neighbor = get_chunk(table, cx, cy, cz - 1);
                MAYBE_REGEN(neighbor);
                neighbor = get_chunk(table, cx, cy, cz + 1);
                MAYBE_REGEN(neighbor);
                
                #undef MAYBE_REGEN
            }
        }
        
        free(result);
        result = result_pop();
    }
}