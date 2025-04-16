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

    Vector3 tempCubePos = { 0.0f };

    Camera3D camera = { 0 };
    camera.position = (Vector3) { 50.0f, 50.0f, 50.0f };
    camera.target = (Vector3) { 0.0f, 32.0f, 0.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;


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
                            if(chunk[x][y][z] == BLOCK_AIR) continue;

                            if(x > 0 && x < CHUNK_WIDTH - 1 && y > 0 && y < CHUNK_HEIGHT - 1 && z > 0 && z < CHUNK_DEPTH - 1) {
                                if(chunk[x-1][y][z] != BLOCK_AIR && 
                                    chunk[x+1][y][z] != BLOCK_AIR && 
                                    chunk[x][y-1][z] != BLOCK_AIR && 
                                    chunk[x][y+1][z] != BLOCK_AIR && 
                                    chunk[x][y][z-1] != BLOCK_AIR && 
                                    chunk[x][y][z+1] != BLOCK_AIR)
                                    {
                                        continue;
                                    }
                            }
                            

                            if(chunk[x][y][z] == BLOCK_DIRT) {
                                DrawCubeV((Vector3) { x, y + 0.5f, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, DARKGREEN);
                                DrawCubeWiresV((Vector3) { x, y + 0.5f, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE);

                            } else {
                                DrawCubeV((Vector3) { x, y + 0.5f, z}, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
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