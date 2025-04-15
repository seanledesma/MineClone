//basic window
#include "raylib.h" 
#include <string.h>

#define screenWidth 800
#define screenHeight 450
// starting with smaller chunk sizes, can change later
const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 64;
const int CHUNK_DEPTH = 16;

typedef enum {
    BLOCK_AIR = 0,
    BLOCK_DIRT,
    BLOCK_STONE,
    BLOCK_WATER,
    BLOCK_LAVA,
}BlockType;


int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raymine v0.0.1");
    SetTargetFPS(60);

    BlockType chunk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
    memset(chunk, 0, sizeof(chunk));

    for(int x = 0; x < CHUNK_WIDTH; x++){
        for(int y = 0; y < CHUNK_HEIGHT; y++) {
            for(int z = 0; z < CHUNK_DEPTH; z++) {
                //recheck this later, make sure am using < and >= correctly
                if(y < 48) {
                    if(y >= 42) {
                        chunk[x][y][z] = BLOCK_DIRT;
                    }else {
                        chunk[x][y][z] = BLOCK_STONE;
                    }
                }
            }
        }
    }

    //main game loop here
    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            for(int x = 0; x < CHUNK_WIDTH; x++){
                for(int y = 0; y < CHUNK_HEIGHT; y++) {
                    for(int z = 0; z < CHUNK_DEPTH; z++) {
                        if(chunk[x][y][z] == BLOCK_AIR) continue;

                        if(chunk[x][y][z] == BLOCK_DIRT) {
                            DrawCubeV((Vector3) {x / CHUNK_WIDTH, y / CHUNK_HEIGHT, z / CHUNK_DEPTH}, 
                                    (Vector3) {10,10,10}, GREEN);
                        }
                    }
                }
            }


        EndDrawing();
    }

    CloseWindow();

    return 0;       
}