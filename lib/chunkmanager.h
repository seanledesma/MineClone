#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "include.h"

#define CHUNK_SIZE 16
#define HALF_CHUNK 8
#define HASH_TABLE_SIZE 1024
#define CHUNK_RENDER_MAX 2

Vector3 nearbyChunks [1000]; //can update the amount later
int nearbyChunkCount;

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
}Chunk;

typedef struct ChunkEntry{
    Chunk chunk;
    int cx, cy, cz;
    uint64_t key;
    struct ChunkEntry* next_chunk_entry;
}ChunkEntry;

typedef struct {
    ChunkEntry *buckets[HASH_TABLE_SIZE];
}ChunkTable;

size_t hash_index(uint64_t key);
void add_chunk(ChunkTable *table, int cx, int cy, int cz, Chunk chunk);
Chunk *get_chunk(ChunkTable *table, int cx, int cy, int cz);
void remove_chunk(ChunkTable *table, int cx, int cy, int cz);
void create_chunk(ChunkTable *table, int cx, int cy, int cz);
Chunk *get_current_chunk(ChunkTable *table, int cx, int cy, int cz);
void UpdateNearbyChunks(int cx, int cy, int cz);

#endif