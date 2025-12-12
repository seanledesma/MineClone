#define FNL_IMPL
#include "include.h"

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raymine v0.1.0");

    grassTex = LoadTexture("assets/grass_full.png");
    dirtTex = LoadTexture("assets/dirt.png");
    stoneTex = LoadTexture("assets/cobblestone.png");

    SetTextureFilter(grassTex, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(grassTex, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(dirtTex, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(dirtTex, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(stoneTex, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(stoneTex, TEXTURE_WRAP_CLAMP);

    Player player;
    InitPlayer(&player);

    World world;
    InitWorld(&world);

    ChunkTable chunkTable;
    memset(&chunkTable, 0, sizeof(ChunkTable));

    //initialize mutex lock for jobs list
    mtx_init(&queue_mutex, mtx_plain);
    cnd_init(&job_available);

    // NEW: Initialize results queue primitives
    mtx_init(&result_mutex, mtx_plain);
    cnd_init(&result_ready);

    // NEW: Initialize chunk table mutex
    mtx_init(&chunk_table_mutex, mtx_plain);

    const int NUM_WORKERS = 4;
    thrd_t worker_threads[NUM_WORKERS];
    for (int i = 0; i < NUM_WORKERS; i++) {
        thrd_create(&worker_threads[i], chunk_generator_worker, NULL);
    }
    // for (int i = 0; i < NUM_WORKERS; i++) {
    //     // thrd_create does the following:
    //     // a) Allocates resources for a new thread.
    //     // b) Makes this new thread begin execution at the 'chunk_generator_worker' function.
    //     // c) Passes 'NULL' as the argument (which is the void* arg in the worker function).
    //     int result = thrd_create(
    //         &worker_threads[i],          // The thrd_t handle to store the new thread's ID
    //         chunk_generator_worker,      // The function the new thread will execute (the job loop)
    //         NULL                         // The argument to pass to the function (void* arg)
    //     );

    //     if (result != thrd_success) {
    //         // Handle error: Not enough resources to create a thread
    //         // raylib: 
    //         TraceLog(LOG_ERROR, "Failed to create worker thread %d", i);
    //     }
    // }

    // next I want to update the current chunk as the player moves
    int cx = (int)floor((player.camera.position.x + HALF_CHUNK) / CHUNK_SIZE);
    int cy = (int)floor((player.camera.position.y + HALF_CHUNK) / CHUNK_SIZE);
    int cz = (int)floor((player.camera.position.z + HALF_CHUNK) / CHUNK_SIZE);


    Chunk *current_chunk = get_current_chunk(&chunkTable, cx, cy, cz);
    Chunk *chunk_iterator = current_chunk;

    UpdateNearbyChunks(cx, cy, cz);
    int prevcx = cx;
    int prevcy = cy;
    int prevcz = cz;

    //int blocksRendered = 0;


    DisableCursor();
    SetTargetFPS(240);
    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

        UpdatePlayer(&chunkTable, &player, dt);



        /* I am going to do something different here. I will use cx cy and cz as my integer coords for my hash function, and 
            I will translate those to world coords upon creation */
        cx = (int)floor((player.camera.position.x + HALF_CHUNK) / CHUNK_SIZE);
        cy = (int)floor((player.camera.position.y + HALF_CHUNK) / CHUNK_SIZE);
        cz = (int)floor((player.camera.position.z + HALF_CHUNK) / CHUNK_SIZE);

        if (prevcx != cx || prevcy != cy || prevcz != cz) {
            current_chunk = get_current_chunk(&chunkTable, cx, cy, cz);
            UpdateNearbyChunks(cx, cy, cz);
            prevcx = cx;
            prevcy = cy;
            prevcz = cz;
        }


        // NEW: Process any completed chunk meshes before drawing the frame
        ProcessChunkResults();
        ProcessJobResults(&chunkTable);
        
        //facesDrawn = 0;
        BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode3D(player.camera);
                if (nearbyChunkCount < 0 || nearbyChunkCount > NEARBY_CHUNK_ARRAY_SIZE) {
                    nearbyChunkCount = 0;
                }
                for (int i = 0; i < nearbyChunkCount; i++) {
                    Chunk* chunk = get_chunk(&chunkTable, cx, cy, cz);
                    
                    if (chunk == NULL) {
                        // If it's a new location, create the job and request it
                        Job* job = create_job(&chunkTable, cx, cy, cz);
                        if (job) {
                            job_push(job);
                            cnd_signal(&job_available); // Wake up a worker!
                        }
                    } else if (chunk->state == CHUNK_STATE_READY) {
                        // Draw the chunk only if its mesh has been uploaded
                        DrawModel(chunk->grassModel, chunk->world_pos, 1.0f, WHITE);
                        DrawModel(chunk->dirtModel, chunk->world_pos, 1.0f, WHITE);
                        DrawModel(chunk->stoneModel, chunk->world_pos, 1.0f, WHITE);
                    }


                    //targetBlock = RayCastTargetBlock(&camera, &chunkTable);
                    DrawCubeWiresV(
                        (Vector3) { 
                            player.targetBlockWorld.x + 0.5f, 
                            player.targetBlockWorld.y + 0.5f, 
                            player.targetBlockWorld.z + 0.5f }, 
                        (Vector3) { 1.0f, 1.0f, 1.0f }, 
                        PINK);

                }


            EndMode3D();

            // crosshairs
            DrawLine(GetScreenWidth() / 2, GetScreenHeight() / 2 - 10, GetScreenWidth() / 2, GetScreenHeight() / 2 + 10, WHITE);
            DrawLine(GetScreenWidth() / 2 - 10, GetScreenHeight() / 2, GetScreenWidth() / 2 + 10, GetScreenHeight() / 2, WHITE);
            //DrawText(TextFormat("current chunk coords: x:%d, y:%d, z:%d ", current_chunk->world_pos.x, current_chunk->world_pos.y, current_chunk->world_pos.z), 190, 200, 20, LIGHTGRAY);
            //DrawText(TextFormat("current block coords: x:%d, y:%d, z:%d ", current_chunk->blocks[0]->x, current_chunk->pos.y, current_chunk->pos.z), 190, 200, 20, LIGHTGRAY);
            DrawText(TextFormat("player coords: x:%.2f, y:%.2f, z:%.2f ", player.camera.position.x, player.camera.position.y, player.camera.position.z), 100, 8, 20, LIGHTGRAY);
            //DrawText(TextFormat("cx:%d cy:%d cz:%d", cx,cy,cz), 450, 20, 20, WHITE);

            DrawText(TextFormat("faces rendered: %d", facesDrawn), 650, 20, 20, LIGHTGRAY);
            //DrawText(TextFormat("current_chunk x: %.2f", current_chunk->table_pos.x), 10, 100, 20, LIGHTGRAY);
            //DrawText(TextFormat("target block x: %.2f, target block y: %.2f, target block z: %.2f", player.targetBlockWorld.x, player.targetBlockWorld.y, player.targetBlockWorld.z), 10, 150, 20, LIGHTGRAY);

            DrawText(TextFormat("target block: %.2f, %.2f, %.2f", player.targetBlockWorld.x, player.targetBlockWorld.y, player.targetBlockWorld.z), 650, 60, 20, LIGHTGRAY);
            //DrawText(TextFormat("target block type: %d", blocksRendered), 650, 20, 20, LIGHTGRAY);

            //blocksRendered = 0;



            DrawFPS(10, 10);
        EndDrawing();
    }

    //need to close threads
    RUNNING = false;
    cnd_broadcast(&job_available); //wake up all threads, to close them
    for (int i = 0; i < NUM_WORKERS; i++) {
        thrd_join(worker_threads[i], NULL);//block main thread until workers finish
    }
    mtx_destroy(&queue_mutex);
    cnd_destroy(&job_available);
    mtx_destroy(&result_mutex);
    cnd_destroy(&result_ready);

    UnloadTexture(stoneTex);
    UnloadTexture(dirtTex);
    UnloadTexture(grassTex);

    void CleanupChunkTable(ChunkTable* table);


    CloseWindow();

    return 0;       
}
