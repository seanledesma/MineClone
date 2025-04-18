/*
*   TODO:
*       - figure out how to make camera behave like a player, gravity, etc
*
*/






//basic window
#include "raylib.h" 
#include <string.h>

#define screenWidth 2000
#define screenHeight 1050
// starting with smaller chunk sizes, can change later
const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 64;
const int CHUNK_DEPTH = 16;

typedef enum {
    BLOCK_AIR = 0,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE,
    BLOCK_WATER,
    BLOCK_LAVA,
}BlockType;

typedef struct {
    BlockType blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
    Vector3 position;
} Chunk;

typedef struct{

} Player;

// PROTOTYPES
void InitializeChunk(Chunk* chunk);
bool IsBlockVisible(Chunk*, int x, int y, int z);



int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raymine v0.0.1");
    SetTargetFPS(60);

    // BlockType chunk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
    // memset(chunk, 0, sizeof(chunk));
    // create a single chunk for now
    Chunk chunk;
    InitializeChunk(&chunk);
    

    Vector3 tempCubePos = { 0.0f };

    Camera3D camera = { 0 };
    camera.position = (Vector3) { 50.0f, 50.0f, 50.0f };
    camera.target = (Vector3) { 0.0f, 32.0f, 0.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;




    DisableCursor();
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);


        BeginDrawing();
            ClearBackground(BLUE);

            BeginMode3D(camera);

                for(int x = 0; x < CHUNK_WIDTH; x++){
                    for(int y = 0; y < CHUNK_HEIGHT; y++) {
                        for(int z = 0; z < CHUNK_DEPTH; z++) {
                            //if block is NOT visible, skip that particular block (function will return T or F)
                            if(IsBlockVisible(&chunk, x, y, z) == false) continue;

                            

                            if(chunk.blocks[x][y][z] == BLOCK_GRASS) {
                                DrawCubeV((Vector3) { x, y + 0.5f, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, DARKGREEN);
                                DrawCubeWiresV((Vector3) { x, y + 0.5f, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE);
                            }else if(chunk.blocks[x][y][z] == BLOCK_DIRT) {
                                DrawCubeV((Vector3) { x, y + 0.5f, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, DARKBROWN);
                                DrawCubeWiresV((Vector3) { x, y + 0.5f, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE);
                            }else {
                                DrawCubeV((Vector3) { x, y + 0.5f, z}, (Vector3) { 1.0f, 1.0f, 1.0f }, DARKGRAY);
                                DrawCubeWiresV((Vector3) { x, y + 0.5f, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE);
                            }
                        }
                    }
                }

            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;       
}

void InitializeChunk(Chunk* chunk) {
    //set all blocks to air initially
    memset(chunk->blocks, 0, sizeof(chunk->blocks));
    //set the other blocks to stone, grass, or dirt
    //will add more to make plane uneven later
    for(int x = 0; x < CHUNK_WIDTH; x++){
        for(int y = 0; y < CHUNK_HEIGHT; y++) {
            for(int z = 0; z < CHUNK_DEPTH; z++) {
                //recheck this later, make sure am using < and >= correctly
                if(y == 48)
                    chunk->blocks[x][y][z] = BLOCK_GRASS;
                if(y < 48) {
                    if(y >= 42) {
                        chunk->blocks[x][y][z] = BLOCK_DIRT;
                    }else {
                        chunk->blocks[x][y][z] = BLOCK_STONE;
                    }
                }
            }
        }
    }
}
//should probably look into ways to only render block faces facing air, instead of whole block even if only one side touches air
bool IsBlockVisible(Chunk* chunk, int x, int y, int z) {
    //all air blocks are invisible, so return false
    if(chunk->blocks[x][y][z] == BLOCK_AIR) return false;
    //if the block is not an outside block AND block isn't touching air, return false
    if(x > 0 && x < CHUNK_WIDTH - 1 && y > 0 && y < CHUNK_HEIGHT - 1 && z > 0 && z < CHUNK_DEPTH - 1) {
        if(chunk->blocks[x-1][y][z] != BLOCK_AIR && 
            chunk->blocks[x+1][y][z] != BLOCK_AIR && 
            chunk->blocks[x][y-1][z] != BLOCK_AIR && 
            chunk->blocks[x][y+1][z] != BLOCK_AIR && 
            chunk->blocks[x][y][z-1] != BLOCK_AIR && 
            chunk->blocks[x][y][z+1] != BLOCK_AIR)
            {
                return false;
            }
    }

    return true;
}