#include "chunkmanager.h"
#include "myhash.h"




//hash_index takes the resut of the hash function, and makes it a number that can fit in my hash table.
size_t hash_index(uint64_t key) {
    return key % HASH_TABLE_SIZE;
}

//saves chunk in chunk table using hash index (which we get after getting hash)
//will save new chunk entry as new node in linked list at appropriate bucket index
void add_chunk(ChunkTable *table, int cx, int cy, int cz, Chunk chunk) {

    /* create new node (chunk entry) */
    ChunkEntry* new_entry = (ChunkEntry*)malloc(sizeof(ChunkEntry));
    
    new_entry->key = chunk_hash(cx, cy, cz);
    new_entry->cx = cx;
    new_entry->cy = cy;
    new_entry->cz = cz;
    new_entry->chunk = chunk;
    new_entry->next_chunk_entry = NULL;

    /* get index for hashtable, place new entry at head of index's linked list */
    int index = hash_index(new_entry->key);
    
    new_entry->next_chunk_entry = table->buckets[index];
    table->buckets[index] = new_entry;

}

Chunk *get_chunk(ChunkTable *table, int cx, int cy, int cz) {

    /* since I know the cx,cy,cz coords I can recreate the hash and find the correct index */
    uint64_t key = chunk_hash(cx, cy, cz);
    int index = hash_index(key);
    /* I need to cycle through the chunk entries at index til I find the right key */
    ChunkEntry *current_entry = table->buckets[index];
    while(current_entry != NULL) {
        if(current_entry->key == key) {
            // keep an eye on this, should be fine
            return &current_entry->chunk;
        }
        current_entry = current_entry->next_chunk_entry;
    }
    return NULL;
}

void remove_chunk(ChunkTable *table, int cx, int cy, int cz) {
    /* the goal here is to find the key using the coords, get the index, find the right chunk 
        entry, and connect the nodes on either side of it to remove it */
    uint64_t key = chunk_hash(cx, cy, cz);
    int index = hash_index(key);

    ChunkEntry *current_entry = table->buckets[index];
    //key not present in the linked list?
    if (current_entry == NULL) return;

    //handle head node 
    if(current_entry->key == key) {
        table->buckets[index] = current_entry->next_chunk_entry;
        free(current_entry);
        return;
    }
    //not head node? let's keep track of old nodes to link to next node should we find the matching key
    ChunkEntry *prev = current_entry;
    current_entry = current_entry->next_chunk_entry;
    while(current_entry != NULL) {
        if(current_entry->key == key) {
            prev->next_chunk_entry = current_entry->next_chunk_entry;
            free(current_entry);
            return;
        }
        prev = current_entry;
        current_entry = current_entry->next_chunk_entry;
    }
}

void create_chunk(ChunkTable *table, int cx, int cy, int cz) {
    Chunk new_chunk;
    // for (int x = 0; x < CHUNK_SIZE; x++) {
    //     for (int y = 0; y < CHUNK_SIZE; y++) {
    //         for (int z = 0; z < CHUNK_SIZE; z++) {
    //             new_chunk.blocks[x][y][z].blockType = BLOCK_DIRT;
    //             new_chunk.blocks[x][y][z].pos = (Vector3) { 
    //                 x + (cx * CHUNK_SIZE), 
    //                 (0 - y) + (cy * CHUNK_SIZE), 
    //                 z + (cz * CHUNK_SIZE)};
    //         }
    //     }
    // }
    new_chunk.table_pos = (Vector3) { cx, cy, cz };
    new_chunk.world_pos = (Vector3) { cx * CHUNK_SIZE, cy * CHUNK_SIZE, cz * CHUNK_SIZE };
    /* This is interesting. Cubes in raylib have their origin in the center, so I need to account for that when
        placing blocks inside chunks. The first chunk will have the center at (0,0,0), so I need to start at (-8,0,0)
        and end at (8,0,0). But the problem is that those blocks are also drawn from center, so I actually need to 
        compensate for that */
    float baseX = -(CHUNK_SIZE / 2) + 0.5f;
    float baseY = -(CHUNK_SIZE / 2) + 0.5f;
    float baseZ = -(CHUNK_SIZE / 2) + 0.5f;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
               
                new_chunk.blocks[x][y][z].pos = (Vector3) { 
                    new_chunk.world_pos.x + baseX, 
                    new_chunk.world_pos.y + baseY, 
                    new_chunk.world_pos.z + baseZ };

                if (new_chunk.blocks[x][y][z].pos.y <= 0) {
                    new_chunk.blocks[x][y][z].blockType = BLOCK_DIRT;
                } else {
                    new_chunk.blocks[x][y][z].blockType = BLOCK_AIR;
                }
                

                baseZ++;
            }
            baseZ = -(CHUNK_SIZE / 2) + 0.5f;
            baseY++;
        }
        baseY = -(CHUNK_SIZE / 2) + 0.5f;
        baseX++;
    }
    add_chunk(table, cx, cy, cz, new_chunk);

    return;
}

Chunk *get_current_chunk(ChunkTable *table, int cx, int cy, int cz) {
    
    Chunk *chunk = get_chunk(table, cx, cy, cz);
    if (chunk == NULL) {
        create_chunk(table, cx, cy, cz);
        chunk = get_chunk(table, cx, cy, cz);
    }
    return chunk;
}