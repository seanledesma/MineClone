#define FNL_IMPL
#include "include.h"

int main(void) {
    const int screenWidth = 3840;
    const int screenHeight = 2160;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raymine v0.0.2");

    Texture2D grassTex = LoadTexture("assets/grass_full.png");
    Texture2D dirtTex = LoadTexture("assets/dirt.png");
    Texture2D stoneTex = LoadTexture("assets/cobblestone.png");

    SetTextureFilter(grassTex, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(grassTex, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(dirtTex, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(dirtTex, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(stoneTex, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(stoneTex, TEXTURE_WRAP_CLAMP);

    // Define the camera to look into our 3d world (position, target, up vector)
    // Camera camera = { 0 };
    // camera.position = (Vector3){ 0.0f, 2.0f, 5.0f };    // Camera position
    // camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    // camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    // camera.fovy = 60.0f;                                // Camera field-of-view Y
    // camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    Player player;
    InitPlayer(&player);

    ChunkTable chunkTable;
    memset(&chunkTable, 0, sizeof(ChunkTable));


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

    int blocksRendered = 0;
    DisableCursor();
    SetTargetFPS(120);
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
        
        BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode3D(player.camera);
                if (nearbyChunkCount < 0 || nearbyChunkCount > NEARBY_CHUNK_ARRAY_SIZE) {
                    nearbyChunkCount = 0;
                }
                for (int i = 0; i < nearbyChunkCount; i++) {
                    chunk_iterator = get_current_chunk(&chunkTable, nearbyChunks[i].x, nearbyChunks[i].y, nearbyChunks[i].z);
                    if (!chunk_iterator) {
                        continue; //in case get_current_chunk fails, skip this loop
                    }
                    
                    for (int x = 0; x < CHUNK_SIZE; x++) {
                        for (int y = 0; y < CHUNK_SIZE; y++) {
                            for (int z = 0; z < CHUNK_SIZE; z++) {
                                //skip air blocks entirely 
                                if (chunk_iterator->blocks[x][y][z].blockType == BLOCK_AIR) {
                                    continue;
                                }
                                uint8_t visibleFaces = 0;
                                if (IsBlockAir(&chunkTable, chunk_iterator, x, y, z+1)) visibleFaces |= FACE_FRONT;
                                if (IsBlockAir(&chunkTable, chunk_iterator, x, y, z-1)) visibleFaces |= FACE_BACK;
                                if (IsBlockAir(&chunkTable, chunk_iterator, x, y+1, z)) visibleFaces |= FACE_TOP;
                                if (IsBlockAir(&chunkTable, chunk_iterator, x, y-1, z)) visibleFaces |= FACE_BOTTOM;
                                if (IsBlockAir(&chunkTable, chunk_iterator, x+1, y, z)) visibleFaces |= FACE_RIGHT;
                                if (IsBlockAir(&chunkTable, chunk_iterator, x-1, y, z)) visibleFaces |= FACE_LEFT;
                                

                                if (visibleFaces > 0) {
                                    Texture2D texture;
                                    if (chunk_iterator->blocks[x][y][z].blockType == BLOCK_GRASS) {
                                        texture = grassTex;
                                    } else if (chunk_iterator->blocks[x][y][z].blockType == BLOCK_DIRT) {
                                        texture = dirtTex;
                                    } else if (chunk_iterator->blocks[x][y][z].blockType == BLOCK_STONE) {
                                        texture = stoneTex;
                                    } else {
                                        continue; //unknown block type
                                    }
                                    
                                    DrawCubeTextureCulled(texture, chunk_iterator->blocks[x][y][z].pos, 
                                                        1.0f, 1.0f, 1.0f, WHITE, visibleFaces);
                                    blocksRendered++;
                                }
                            }
                        }
                    }
                    // if (current_chunk->blocks[(int)floor(targetBlock.x)][(int)floor(targetBlock.y)][(int)floor(targetBlock.z)].blockType == BLOCK_AIR) {
                    //     DrawCubeWiresV((Vector3) { targetBlock.x + 0.5f, targetBlock.y + 0.5f, targetBlock.z + 0.5f}, (Vector3) { 1.0f, 1.0f, 1.0f }, BLACK);
                    // }


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

            DrawText(TextFormat("blocks rendered: %d", blocksRendered), 650, 20, 20, LIGHTGRAY);
            //DrawText(TextFormat("current_chunk x: %.2f", current_chunk->table_pos.x), 10, 100, 20, LIGHTGRAY);
            //DrawText(TextFormat("target block x: %.2f, target block y: %.2f, target block z: %.2f", player.targetBlockWorld.x, player.targetBlockWorld.y, player.targetBlockWorld.z), 10, 150, 20, LIGHTGRAY);

            DrawText(TextFormat("target block: %.2f, %.2f, %.2f", player.targetBlockWorld.x, player.targetBlockWorld.y, player.targetBlockWorld.z), 650, 60, 20, LIGHTGRAY);
            //DrawText(TextFormat("target block type: %d", blocksRendered), 650, 20, 20, LIGHTGRAY);

            blocksRendered = 0;



            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;       
}
