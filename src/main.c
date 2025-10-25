// I decided to start over from scratch
#include "raylib.h" 
#include "rcamera.h"

#include "myhash.h"
#include "chunkmanager.h"








bool IsBlockAir(Chunk* chunk, int x, int y, int z) {
    // need to see if block is even in current chunk
    // will need to handle this different when making more chunks visible
    //if (x <)


    if (chunk->blocks[x][y][z].blockType == BLOCK_AIR) {
        return true;
    }
    return false;
}




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
    
    int blocksRendered = 0;
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

        current_chunk = get_current_chunk(&chunkTable, cx, cy, cz);
        

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
                            // need to check to see if block I will draw is even visible (has a neighbor that is block air)
                            bool isVisible = false;
                            if (IsBlockAir(current_chunk, x+1, y, z)) isVisible = true;
                            if (IsBlockAir(current_chunk, x-1, y, z)) isVisible = true;
                            if (IsBlockAir(current_chunk, x, y+1, z)) isVisible = true;
                            if (IsBlockAir(current_chunk, x, y-1, z)) isVisible = true;
                            if (IsBlockAir(current_chunk, x, y, z+1)) isVisible = true;
                            if (IsBlockAir(current_chunk, x, y, z-1)) isVisible = true;
                            

                            if (isVisible) {
                                if (current_chunk->blocks[x][y][z].blockType == BLOCK_DIRT) {
                                    DrawCubeV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, RAYWHITE);
                                    DrawCubeWiresV(current_chunk->blocks[x][y][z].pos, (Vector3) { 1.0f, 1.0f, 1.0f }, GRAY);
                                    blocksRendered++;
                                }
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
            //DrawText(TextFormat("current chunk coords: x:%d, y:%d, z:%d ", current_chunk->world_pos.x, current_chunk->world_pos.y, current_chunk->world_pos.z), 190, 200, 20, LIGHTGRAY);
            //DrawText(TextFormat("current block coords: x:%d, y:%d, z:%d ", current_chunk->blocks[0]->x, current_chunk->pos.y, current_chunk->pos.z), 190, 200, 20, LIGHTGRAY);
            DrawText(TextFormat("player coords: x:%.2f, y:%.2f, z:%.2f ", camera.position.x, camera.position.y, camera.position.z), 40, 20, 20, LIGHTGRAY);
            DrawText(TextFormat("cx:%d cy:%d cz:%d", cx,cy,cz), 450, 20, 20, WHITE);

            DrawText(TextFormat("blocks rendered: %d", blocksRendered), 650, 20, 20, LIGHTGRAY);
            blocksRendered = 0;



            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;       
}

