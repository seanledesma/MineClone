// I decided to start over from scratch
#include "raylib.h" 
#include "rcamera.h"
#include "myhash.h"
#include <stdlib.h>

#define CHUNK_SIZE 16
#define HASH_TABLE_SIZE 1024

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
    Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
}Chunk;

typedef struct ChunkEntry{
    Chunk chunk;
    int cx, cy, cz;
    int key;
    struct ChunkEntry* next_chunk_entry;
}ChunkEntry;

typedef struct {
    ChunkEntry *buckets[HASH_TABLE_SIZE];
}ChunkTable;


int hash_index(uint64_t key);
void add_chunk(ChunkTable *table, int cx, int cy, int cz, Chunk chunk);
Chunk *get_chunk(ChunkTable *table, int cx, int cy, int cz);
void remove_chunk(ChunkTable *table, int cx, int cy, int cz);

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "MineClone");

    //LOGIC
    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 5.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    ChunkTable chunkTable;

    //make base chunk
    Chunk homeChunk;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                homeChunk.blocks[x][y][z].blockType = BLOCK_DIRT;
                homeChunk.blocks[x][y][z].pos = (Vector3) { (x * -1) - 1, (y * -1) - 1, (z * -1) - 1 };
            }
        }
    }

    add_chunk(&chunkTable, 0, 0, 0, homeChunk);
    

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

        // next I want to make a chunk


        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);

                //DrawCubeV((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                //DrawCubeWiresV((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);

                for (int x = 0; x < CHUNK_SIZE; x++) {
                    for (int y = 0; y < CHUNK_SIZE; y++) {
                        for (int z = 0; z < CHUNK_SIZE; z++) {
                            DrawCubeV(homeChunk.blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                            DrawCubeWiresV(homeChunk.blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                        }
                    }
                }

            EndMode3D();
            //DrawText("congrats! our first window!", 190, 200, 20, LIGHTGRAY);
            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;       
}

//hash_index takes the resut of the hash function, and makes it a number that can fit in my hash table.
int hash_index(uint64_t key) {
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
    }
    return NULL;
}

void remove_chunk(ChunkTable *table, int cx, int cy, int cz) {
    /* the goal here is to find the key using the coords, get the index, find the right chunk 
        entry, and connect the nodes on either side of it to remove it */
    uint64_t key = chunk_hash(cx, cy, cz);
    int index = hash_index(key);

    ChunkEntry *current_entry = table->buckets[index];
    //handle head node first
    if(current_entry->key == key) {
        table->buckets[index] = current_entry->next_chunk_entry;
        return;
    }
    //not head node? let's keep track of old nodes to link to next node should we find the matching key
    ChunkEntry *temp;
    while(current_entry != NULL) {
        temp = current_entry;
        current_entry = current_entry->next_chunk_entry;
        if(current_entry->key == key) {
            temp->next_chunk_entry = current_entry->next_chunk_entry;
            free(current_entry);
            return;
        }
    }
    //key not present in the linked list?
    if (current_entry == NULL) {
        return;
    }

    return;
}