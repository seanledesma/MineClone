#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "include.h"

#define CHUNK_SIZE 32
#define HALF_CHUNK 16
#define HASH_TABLE_SIZE 1024
#define CHUNK_RENDER_MAX 5

#define CHUNK_VIEW_DISTANCE (2 * CHUNK_RENDER_MAX + 1)
#define NEARBY_CHUNK_ARRAY_SIZE (CHUNK_VIEW_DISTANCE * CHUNK_VIEW_DISTANCE * CHUNK_VIEW_DISTANCE)

// Chunk States
#define CHUNK_STATE_EMPTY          0
#define CHUNK_STATE_REQUESTED      1
#define CHUNK_STATE_BLOCKS_READY   2
#define CHUNK_STATE_READY          3
#define CHUNK_STATE_REGENERATING   4  // Has valid mesh but new one is being generated

extern Vector3 nearbyChunks [NEARBY_CHUNK_ARRAY_SIZE]; 
extern int nearbyChunkCount;

typedef struct Job Job;
typedef struct JobResult JobResult;

typedef enum BlockType {
    BLOCK_AIR,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE,
}BlockType;

typedef struct Block {
    Vector3 pos;
    int blockType;
} Block;

typedef struct Chunk{
    Vector3 table_pos;
    Vector3 world_pos;
    Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    Mesh grassMesh;
    Mesh dirtMesh;
    Mesh stoneMesh;

    Model grassModel;
    Model dirtModel;
    Model stoneModel;

    int state;
    bool needsRemesh;  // FIX: Flag to track if another remesh is needed
}Chunk;

typedef struct ChunkEntry{
    Chunk* chunk;
    int cx, cy, cz;
    uint64_t key;
    struct ChunkEntry* next_chunk_entry;
}ChunkEntry;

typedef struct {
    ChunkEntry *buckets[HASH_TABLE_SIZE];
}ChunkTable;

size_t hash_index(uint64_t key);
void add_chunk(ChunkTable* table, int cx, int cy, int cz, Chunk* chunk);

Chunk *get_chunk(ChunkTable* table, int cx, int cy, int cz);
void remove_chunk(ChunkTable* table, int cx, int cy, int cz);
Chunk* create_chunk(Job* job);
Chunk *get_current_chunk(ChunkTable* table, int cx, int cy, int cz);
void UpdateNearbyChunks(int cx, int cy, int cz);
void CleanupChunkTable(ChunkTable* table);
void ProcessChunkResults();
void ProcessJobResults(ChunkTable* table);


#endif