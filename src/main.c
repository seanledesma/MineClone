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

    // Initialize results queue primitives
    mtx_init(&result_mutex, mtx_plain);
    cnd_init(&result_ready);

    // Initialize chunk table mutex
    mtx_init(&chunk_table_mutex, mtx_plain);

    const int NUM_WORKERS = 4;
    thrd_t worker_threads[NUM_WORKERS];
    for (int i = 0; i < NUM_WORKERS; i++) {
        thrd_create(&worker_threads[i], chunk_generator_worker, NULL);
    }

    // next I want to update the current chunk as the player moves
    int cx = (int)floor((player.camera.position.x + HALF_CHUNK) / CHUNK_SIZE);
    int cy = (int)floor((player.camera.position.y + HALF_CHUNK) / CHUNK_SIZE);
    int cz = (int)floor((player.camera.position.z + HALF_CHUNK) / CHUNK_SIZE);


    Chunk *current_chunk = get_current_chunk(&chunkTable, cx, cy, cz);
    (void)current_chunk; // Suppress unused warning

    UpdateNearbyChunks(cx, cy, cz);
    int prevcx = cx;
    int prevcy = cy;
    int prevcz = cz;


    DisableCursor();
    SetTargetFPS(240);
    while(!WindowShouldClose()) {
        float dt = GetFrameTime();

        UpdatePlayer(&chunkTable, &player, dt);

        cx = (int)floor((player.camera.position.x + HALF_CHUNK) / CHUNK_SIZE);
        cy = (int)floor((player.camera.position.y + HALF_CHUNK) / CHUNK_SIZE);
        cz = (int)floor((player.camera.position.z + HALF_CHUNK) / CHUNK_SIZE);

        if (prevcx != cx || prevcy != cy || prevcz != cz) {
            current_chunk = get_current_chunk(&chunkTable, cx, cy, cz);
            (void)current_chunk;
            UpdateNearbyChunks(cx, cy, cz);
            prevcx = cx;
            prevcy = cy;
            prevcz = cz;
        }

        // Process any completed chunk meshes before drawing the frame
        ProcessJobResults(&chunkTable);
        
        // FIX: Reset facesDrawn each frame
        facesDrawn = 0;
        
        BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode3D(player.camera);
                if (nearbyChunkCount < 0 || nearbyChunkCount > NEARBY_CHUNK_ARRAY_SIZE) {
                    nearbyChunkCount = 0;
                }
                for (int i = 0; i < nearbyChunkCount; i++) {
                    int chunk_cx = (int)nearbyChunks[i].x;
                    int chunk_cy = (int)nearbyChunks[i].y;
                    int chunk_cz = (int)nearbyChunks[i].z;
                    
                    Chunk* chunk = get_chunk(&chunkTable, chunk_cx, chunk_cy, chunk_cz);
                    
                    if (chunk == NULL) {
                        // FIX: Use the new request_chunk_job that handles deduplication
                        request_chunk_job(&chunkTable, chunk_cx, chunk_cy, chunk_cz);
                    } else if (chunk->state == CHUNK_STATE_READY || chunk->state == CHUNK_STATE_REGENERATING) {
                        // Draw chunks that are ready OR regenerating (still have valid old mesh)
                        // FIX: Count faces for display
                        facesDrawn += chunk->grassMesh.triangleCount / 2;
                        facesDrawn += chunk->dirtMesh.triangleCount / 2;
                        facesDrawn += chunk->stoneMesh.triangleCount / 2;
                        
                        DrawModel(chunk->grassModel, chunk->world_pos, 1.0f, WHITE);
                        DrawModel(chunk->dirtModel, chunk->world_pos, 1.0f, WHITE);
                        DrawModel(chunk->stoneModel, chunk->world_pos, 1.0f, WHITE);
                    }
                }

                // Draw target block highlight
                DrawCubeWiresV(
                    (Vector3) { 
                        player.targetBlockWorld.x + 0.5f, 
                        player.targetBlockWorld.y + 0.5f, 
                        player.targetBlockWorld.z + 0.5f }, 
                    (Vector3) { 1.0f, 1.0f, 1.0f }, 
                    PINK);

            EndMode3D();

            // crosshairs
            DrawLine(GetScreenWidth() / 2, GetScreenHeight() / 2 - 10, GetScreenWidth() / 2, GetScreenHeight() / 2 + 10, WHITE);
            DrawLine(GetScreenWidth() / 2 - 10, GetScreenHeight() / 2, GetScreenWidth() / 2 + 10, GetScreenHeight() / 2, WHITE);
            
            DrawText(TextFormat("player coords: x:%.2f, y:%.2f, z:%.2f ", player.camera.position.x, player.camera.position.y, player.camera.position.z), 100, 8, 20, LIGHTGRAY);
            DrawText(TextFormat("faces rendered: %d", facesDrawn), 650, 20, 20, LIGHTGRAY);
            DrawText(TextFormat("target block: %.2f, %.2f, %.2f", player.targetBlockWorld.x, player.targetBlockWorld.y, player.targetBlockWorld.z), 650, 60, 20, LIGHTGRAY);

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
    mtx_destroy(&chunk_table_mutex);

    UnloadTexture(stoneTex);
    UnloadTexture(dirtTex);
    UnloadTexture(grassTex);

    CleanupChunkTable(&chunkTable);

    CloseWindow();

    return 0;       
}