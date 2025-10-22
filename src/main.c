// I decided to start over from scratch
#include "raylib.h" 
#include "rcamera.h"

#define CHUNK_SIZE 16

enum BlockType {
    BLOCK_AIR,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE,
};

typedef struct Chunk{
    int blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
}Chunk;

typedef struct ChunkEntry{
    int cx, cy, cz;
    int key;
}ChunkEntry;




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

    //make base chunk
    Chunk homeChunk;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                homeChunk.blocks[x][y][z] = BLOCK_DIRT;
            }
        }
    }

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
                            DrawCubeV((Vector3) { x, y, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                            DrawCubeWiresV((Vector3) { x, y, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
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