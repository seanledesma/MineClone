// I decided to start over from scratch
#include "raylib.h" 
#include "rcamera.h"
#include "raymath.h"
#include "myhash.h"
#include <stdlib.h>

#define CHUNK_SIZE 16
#define HALF_CHUNK 8
#define HASH_TABLE_SIZE 1024
#define CHUNK_RENDER_MAX 2

enum BlockType {
    BLOCK_AIR,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE,
};

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

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "MineClone");

    //LOGIC
    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 5.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    ChunkTable chunkTable = { 0 };


    // next I want to update the current chunk as the player moves
    int cx = (int)floor((camera.position.x + HALF_CHUNK) / CHUNK_SIZE);
    int cy = (int)floor((camera.position.y + HALF_CHUNK) / CHUNK_SIZE);
    int cz = (int)floor((camera.position.z + HALF_CHUNK) / CHUNK_SIZE);
    Chunk *current_chunk = get_current_chunk(&chunkTable, cx, cy, cz);
    

    DisableCursor();
    SetTargetFPS(120);
    while(!WindowShouldClose()) {

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advanced camera controls, it's recommended to compute camera movement manually
        UpdateCamera(&camera, cameraMode);                  // Update camera
/*
        // Camera PRO usage example (EXPERIMENTAL)
        // This new camera function allows custom movement/rotation values to be directly provided
        // as input parameters, with this approach, rcamera module is internally independent of raylib inputs
        UpdateCameraPro(&camera,
            (Vector3){
                (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
                (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,
                (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
                (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
                0.0f                                                // Move up-down
            },
            (Vector3){
                GetMouseDelta().x*0.05f,                            // Rotation: yaw
                GetMouseDelta().y*0.05f,                            // Rotation: pitch
                0.0f                                                // Rotation: roll
            },
            GetMouseWheelMove()*2.0f);                              // Move to target (zoom)
*/

        // // next I want to update the current chunk as the player moves

        // I'm going to attempt to generate chunks surrounding the player
        //let's make an array so we don't have to loop this many times to render
        //((CHUNK_RENDER_MAX * 2) + 1) ^ 3 example: 2 blocks deep, 2 + 2 + og block (1) = 5, 5^3 = 125
        
        //int nearby_chunk_count = ((CHUNK_RENDER_MAX * 2) + 1) ^ 3;
        //int nearby_chunk_count = 100000;
        // Vector3 visible_chunk_positions[nearby_chunk_count];
        // int chunk_tracker = 0;
        // for (int x = -CHUNK_RENDER_MAX; x <= CHUNK_RENDER_MAX; x++) {
        //     for (int y = -CHUNK_RENDER_MAX; y <= CHUNK_RENDER_MAX; y++) {
        //         for (int z = -CHUNK_RENDER_MAX; z <= CHUNK_RENDER_MAX; z++) {
        //             //create chunks for each axis, starting at -render max, then to +render max
        //             create_chunk(&chunkTable, cx + (x * CHUNK_SIZE * CHUNK_RENDER_MAX), cy + (y * CHUNK_SIZE * CHUNK_RENDER_MAX), cz + (z * CHUNK_SIZE * CHUNK_RENDER_MAX));
        //             //create_chunk(&chunkTable, cx + x, cy + y, cz + z);
        //             visible_chunk_positions[chunk_tracker] = (Vector3) { cx + (x * CHUNK_SIZE * CHUNK_RENDER_MAX), cy + (y * CHUNK_SIZE * CHUNK_RENDER_MAX), cz + (z * CHUNK_SIZE * CHUNK_RENDER_MAX) };
        //             chunk_tracker++;
        //         }
        //     }
        // }

        
        /* I am going to do something different here. I will use cx cy and cz as my integer coords for my hash function, and 
            I will translate those to world coords upon creation */
        cx = (int)floor((camera.position.x + HALF_CHUNK) / CHUNK_SIZE);
        cy = (int)floor((camera.position.y + HALF_CHUNK) / CHUNK_SIZE);
        cz = (int)floor((camera.position.z + HALF_CHUNK) / CHUNK_SIZE);

        Chunk *current_chunk = get_current_chunk(&chunkTable, cx, cy, cz);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);

                //DrawCubeV((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                //DrawCubeWiresV((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                //for (int i = 0; i < nearby_chunk_count; i++) {
                    // for (int x = 0; x < CHUNK_SIZE; x++) {
                    //     for (int y = 0; y < CHUNK_SIZE; y++) {
                    //         for (int z = 0; z < CHUNK_SIZE; z++) {
                    //             //DrawCubeV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                    //             DrawCubeWiresV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                    //         }
                    //     }
                    // }

                    // for (int a = -CHUNK_RENDER_MAX; a < CHUNK_RENDER_MAX; a++) {
                    //     for (int b = -CHUNK_RENDER_MAX; b < CHUNK_RENDER_MAX; b++) {
                    //         for (int c = -CHUNK_RENDER_MAX; c < CHUNK_RENDER_MAX; c++) {
                    //             for (int x = 0; x < CHUNK_SIZE; x++) {
                    //                 for (int y = 0; y < CHUNK_SIZE; y++) {
                    //                     for (int z = 0; z < CHUNK_SIZE; z++) {
                    //                         //DrawCubeV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                    //                         //DrawCubeWiresV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                    //                     }
                    //                 }
                    //             }
                    //             DrawCubeWiresV(current_chunk->pos, (Vector3) { CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE }, GRAY);
                    //             //create chunks for each axis, starting at -render max, then to +render max
                    //             //current_chunk = get_chunk(&chunkTable, current_chunk->pos.x, cy + (b * CHUNK_SIZE * CHUNK_RENDER_MAX), cz + (c * CHUNK_SIZE * CHUNK_RENDER_MAX));
                    //             //current_chunk = get_chunk(&chunkTable, current_chunk->pos.x, current_chunk->pos.y, current_chunk->pos.z);

                    //         }
                    //     }
                    // }

                    // for (int x = 0; x < CHUNK_SIZE; x++) {
                    //     for (int y = 0; y < CHUNK_SIZE; y++) {
                    //         for (int z = 0; z < CHUNK_SIZE; z++) {
                    //             //DrawCubeV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                    //             //DrawCubeWiresV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                    //         }
                    //     }
                    // }
                    //DrawCubeWiresV(current_chunk->pos, (Vector3) { CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE }, GRAY);
                    //create chunks for each axis, starting at -render max, then to +render max
                    //current_chunk = get_chunk(&chunkTable, visible_chunk_positions[i].x, visible_chunk_positions[i].y, visible_chunk_positions[i].z);
                    //current_chunk = get_chunk(&chunkTable, current_chunk->pos.x, cy + (b * CHUNK_SIZE * CHUNK_RENDER_MAX), cz + (c * CHUNK_SIZE * CHUNK_RENDER_MAX));
                    

                    // DrawCubeWiresV(current_chunk->pos, (Vector3) { CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE }, PINK);
                    // for (int x = 0; x < CHUNK_SIZE; x++) {
                    //     for (int y = 0; y < CHUNK_SIZE; y++) {
                    //         for (int z = 0; z < CHUNK_SIZE; z++) {
                    //             //DrawCubeV(home_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                    //             DrawCubeWiresV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                    //         }
                    //     }
                    // }
                    // //current_chunk = get_chunk(&chunkTable, visible_chunk_positions[i].x, visible_chunk_positions[i].y, visible_chunk_positions[i].z);
                    // current_chunk = get_current_chunk(&chunkTable, cx, cy, cz);
                //}

                DrawCubeWiresV(current_chunk->world_pos, (Vector3) { CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE }, PINK);
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    for (int y = 0; y < CHUNK_SIZE; y++) {
                        for (int z = 0; z < CHUNK_SIZE; z++) {
                            //DrawCubeV(home_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                            //DrawCubeWiresV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                            if (current_chunk->blocks[x][y][z].blockType == BLOCK_DIRT) {
                                DrawCubeV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                                DrawCubeWiresV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                            }
                        }
                    }
                }
                //current_chunk = get_chunk(&chunkTable, visible_chunk_positions[i].x, visible_chunk_positions[i].y, visible_chunk_positions[i].z);
                //current_chunk = get_current_chunk(&chunkTable, cx, cy, cz);

                // DrawCubeWiresV(current_chunk->pos, (Vector3) { CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE }, PINK);
                // for (int x = 0; x < CHUNK_SIZE; x++) {
                //     for (int y = 0; y < CHUNK_SIZE; y++) {
                //         for (int z = 0; z < CHUNK_SIZE; z++) {
                //             //DrawCubeV(home_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                //             DrawCubeWiresV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                //         }
                //     }
                // }
                // current_chunk = get_chunk
                

            EndMode3D();
            DrawText(TextFormat("current chunk coords: x:%f, y:%d, z:%d ", current_chunk->world_pos.x, current_chunk->world_pos.y, current_chunk->world_pos.z), 190, 200, 20, LIGHTGRAY);
            //DrawText(TextFormat("current block coords: x:%d, y:%d, z:%d ", current_chunk->blocks[0]->x, current_chunk->pos.y, current_chunk->pos.z), 190, 200, 20, LIGHTGRAY);
            DrawText(TextFormat("player coords: x:%.2f, y:%.2f, z:%.2f ", camera.position.x, camera.position.y, camera.position.z), 40, 20, 20, LIGHTGRAY);
            DrawText(TextFormat("cx:%d cy:%d cz:%d", cx,cy,cz), 450, 20, 20, WHITE);


            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;       
}

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