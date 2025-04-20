/*
*   TODO:
*       - figure out how to make camera behave like a player, gravity, etc
*
*/

//basic window
#include "raylib.h" 
#include "raymath.h"
#include <string.h>
#include <float.h>

#define screenWidth 2000
#define screenHeight 1050
//#define EYE_LEVEL player.height * 0.9;
// starting with smaller chunk sizes, can change later
const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 64;
const int CHUNK_DEPTH = 16;

const float GRAVITY = -9.8f;

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
    bool isOnGround;
    bool hasTargetBlock;
    float height;
    Vector3 position;
    Vector3 velocity;
    Vector3 targetBlock;
    Vector3 targetFace;
    BlockType selectedBlock;
} Player;

typedef struct {
    float yaw;
    float pitch;
} CameraController;

// PROTOTYPES
void InitializeChunk(Chunk* chunk);
bool IsBlockVisible(Chunk*, int* x, int* y, int* z);
void UpdatePlayer(Player* player, Camera3D* camera, CameraController* camCtrl, Chunk* chunk, float deltaTime);
void CastRay(Camera3D camera, Player* player, Chunk* chunk);
void CastRayDDA(Camera3D camera, Player* player, Chunk* chunk);



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

    Player player = {
        .isOnGround = false,
        .hasTargetBlock = false,
        .height = 2.0f,
        .position = (Vector3) { 0.0f, 60.0f, 5.0f },
        .velocity = (Vector3) { 0 },
        .selectedBlock = BLOCK_DIRT,
    };

    Camera3D camera = { 0 };
    camera.position = (Vector3) { player.position.x, player.position.y + player.height * 0.9f, player.position.z };
    camera.target = (Vector3) { 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    CameraController camCtrl = { 0 };




    DisableCursor();
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        if(IsKeyPressed(KEY_F1))
            EnableCursor();
        if(IsKeyPressed(KEY_F2))
            DisableCursor();


        float deltaTime = GetFrameTime();
        //UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        UpdatePlayer(&player, &camera, &camCtrl, &chunk, deltaTime);


        CastRay(camera, &player, &chunk);
       //CastRayDDA(camera, &player, &chunk);


        BeginDrawing();
            ClearBackground(BLUE);

            BeginMode3D(camera);

                for(int x = 0; x < CHUNK_WIDTH; x++){
                    for(int y = 0; y < CHUNK_HEIGHT; y++) {
                        for(int z = 0; z < CHUNK_DEPTH; z++) {
                            //if block is NOT visible, skip that particular block (function will return T or F)
                            if(IsBlockVisible(&chunk, &x, &y, &z) == false) continue;

                            

                            if(chunk.blocks[x][y][z] == BLOCK_GRASS) {
                                DrawCubeV((Vector3) { x+0.5f, y + 0.5f, z+0.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, DARKGREEN);
                                DrawCubeWiresV((Vector3) { x+0.5f, y + 0.5f, z+0.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE);
                            }else if(chunk.blocks[x][y][z] == BLOCK_DIRT) {
                                DrawCubeV((Vector3) { x+0.5f, y + 0.5f, z+0.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, DARKBROWN);
                                //DrawCubeWiresV((Vector3) { x, y + 0.5f, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE);
                            }else {
                                DrawCubeV((Vector3) { x+0.5f, y + 0.5f, z+0.5f}, (Vector3) { 1.0f, 1.0f, 1.0f }, DARKGRAY);
                                //DrawCubeWiresV((Vector3) { x, y + 0.5f, z }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE);
                            }
                        }
                    }
                }
                // Vector3 rayDir = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
                // Vector3 rayEnd = Vector3Add(camera.position, Vector3Scale(rayDir, 8.0f));       
                // DrawLine3D(camera.position, rayEnd, RED);
                // Vector3 up = Vector3Scale((Vector3){0, 0.05f, 0}, 1);
                // DrawLine3D(Vector3Add(camera.position, up), Vector3Add(rayEnd, up), RED);
                
                if(player.hasTargetBlock) {
                    DrawCubeWiresV((Vector3){ 
                        player.targetBlock.x + 0.5f, player.targetBlock.y + 0.5f, player.targetBlock.z + 0.5f}, 
                        (Vector3) { 1.0f, 1.0f, 1.0f }, BLACK);
                }

                // if (player.hasTargetBlock) {
                //     Vector3 rayEnd = (Vector3){
                //         player.targetBlock.x + 0.5f,
                //         player.targetBlock.y + 0.5f,
                //         player.targetBlock.z + 0.5f
                //     };
                //     DrawLine3D(camera.position, rayEnd, RED);
                // } else {
                //     Vector3 rayEnd = Vector3Add(camera.position, 
                //     Vector3Scale(Vector3Normalize(Vector3Subtract(camera.target, camera.position)), 8.0f));
                //     DrawLine3D(camera.position, rayEnd, RED);
                // }

                
                // Vector3 rayEnd = Vector3Add(camera.position, Vector3Scale(player.lookDirection, 2.0f));  // Shorter line for testing
                // DrawLine3D(camera.position, rayEnd, RED);

                //Vector3 rayEnd = Vector3Add(camera.position, Vector3Scale(player.lookDirection, 8.0f));  // Multiply by a reasonable distance

                //DrawLine3D(camera.position, rayEnd, RED);


            EndMode3D();

            //DrawRectangle(screenWidth / 2 - 20, screenHeight / 2 - 5, 40, 10, WHITE);
            // DrawRectangle(screenWidth / 2 - 5, screenHeight / 2 - 20, 10, 40, WHITE);
            DrawLine(GetScreenWidth() / 2, GetScreenHeight() / 2 - 10, GetScreenWidth() / 2, GetScreenHeight() / 2 + 10, WHITE);
            DrawLine(GetScreenWidth() / 2 - 10, GetScreenHeight() / 2, GetScreenWidth() / 2 + 10, GetScreenHeight() / 2, WHITE);

            DrawText(TextFormat("player pos: X: %.2f Y: %.2f Z: %.2f",
                                player.position.x, player.position.y, player.position.z),
                                300, 10, 20, RAYWHITE);


            if(player.hasTargetBlock) {
                DrawText(TextFormat("Target block: X: %d Y: %d Z: %d", 
                        (int)player.targetBlock.x, (int)player.targetBlock.y, (int)player.targetBlock.z),
                        600, 40, 20, WHITE);
            } else {
                DrawText("No target block", 300, 40, 20, RED);
            }



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
bool IsBlockVisible(Chunk* chunk, int* x, int* y, int* z) {
    //all air blocks are invisible, so return false
    if(chunk->blocks[*x][*y][*z] == BLOCK_AIR) return false;
    //if the block is not an outside block AND block isn't touching air, return false
    if(*x > 0 && *x < CHUNK_WIDTH - 1 && *y > 0 && *y < CHUNK_HEIGHT - 1 && *z > 0 && *z < CHUNK_DEPTH - 1) {
        if(chunk->blocks[*x-1][*y][*z] != BLOCK_AIR && 
            chunk->blocks[*x+1][*y][*z] != BLOCK_AIR && 
            chunk->blocks[*x][*y-1][*z] != BLOCK_AIR && 
            chunk->blocks[*x][*y+1][*z] != BLOCK_AIR && 
            chunk->blocks[*x][*y][*z-1] != BLOCK_AIR && 
            chunk->blocks[*x][*y][*z+1] != BLOCK_AIR) {
                return false;
        }
    }
    return true;
}

void UpdatePlayer(Player* player, Camera3D* camera, CameraController* camCtrl, Chunk* chunk, float deltaTime) {
    //I'll be honest, the next little bit that has to do w/ camCtrl chatGPT helped me with :)
    // setting up mouse look
    Vector2 mouseDelta = GetMouseDelta();
    float sensitivity = 0.003f;
    camCtrl->yaw -= mouseDelta.x * sensitivity;
    camCtrl->pitch -= mouseDelta.y * sensitivity;

    //clamp pitch so we can't flip over
    const float pitchLimit = PI/2.0f - 0.01f;
    if (camCtrl->pitch > pitchLimit) camCtrl->pitch = pitchLimit;
    if (camCtrl->pitch < -pitchLimit) camCtrl->pitch = -pitchLimit;

    //get look direction
    Vector3 direction = (Vector3) {
        cosf(camCtrl->pitch) * sinf(camCtrl->yaw),
        sinf(camCtrl->pitch),
        cosf(camCtrl->pitch) * cosf(camCtrl->yaw)
    };

    // basic movement
    Vector3 forward = { direction.x, 0.0f, direction.z };
    Vector3 right = { -forward.z, 0.0f, forward.x };
    Vector3 move = Vector3Zero();
    float speed = 5.0f;

    if (IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
    if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
    if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);
    if (IsKeyDown(KEY_D)) move = Vector3Add(move, right);

    if (Vector3Length(move) > 0.0f)
        move = Vector3Scale(Vector3Normalize(move), speed * deltaTime);
    
    player->position = Vector3Add(player->position, move);


    //next, need to figure out what block we are over, and if we are right on top of it
    Vector3 floorBlock = { 0 };
    for(int i = (int) player->position.y; i >= 0; i--) {
        //TODO: MAKE SURE WE ARE ONLY PASSING THE CURRENT CHUNK TO THIS FUNC
        // bugs could come from all this type casting :( watch out)
        if(player->position.x >= CHUNK_WIDTH || player->position.x < 0 || 
            player->position.z >= CHUNK_DEPTH || player->position.z < 0) {
            //if we went off the chunk, (this will probably need to change when we add more chunks)
            player->isOnGround = false;
            break;
        } else if(chunk->blocks[(int) player->position.x][i][(int) player->position.z] != BLOCK_AIR) {
            floorBlock = (Vector3) { player->position.x, (float) i, player->position.z };
            break;
        }
    }
    //update if player has hit the floor block
    if(player->position.y <= floorBlock.y + 1) {
        //if player pos is any block but air, we are on ground
        player->isOnGround = true;             
        //next, we need to set camera 1.8 blocks about pos
    }else{
        player->isOnGround = false;
    }
    //jumping
    if(IsKeyPressed(KEY_SPACE) && player->isOnGround) {
        player->velocity.y = 5.0f;
        player->isOnGround = false;
    }

    //if player is in the air, fall down. (may update this to add free mode)
    if(player->isOnGround) {
       player->velocity.y = 0; 
    } else {
        player->velocity.y += GRAVITY * deltaTime;
    }

    player->position.y += player->velocity.y * deltaTime;
    camera->position = (Vector3) { 
        player->position.x, 
        player->position.y + player->height * 0.9f,
        player->position.z
    };
    camera->target = Vector3Add(camera->position, direction);
    camera->up = (Vector3) { 0.0f, 1.0f, 0.0f };

}

void CastRay(Camera3D camera, Player* player, Chunk* chunk) {
    player->hasTargetBlock = false;

    float maxDistance = 8.0f;
    float stepSize = 0.05f; //how fine (?) to step through space, how frequent we are checking if we hit
    //player->lookDirection = Vector3Normalize(player->lookDirection); // debuggin, but probably needed?
    Vector3 rayDir = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
   //Vector3 rayPos = camera.position;
   Vector3 rayPos = Vector3Add(camera.position, Vector3Scale(rayDir, 0.1f));


    for (float t = 0; t  < maxDistance; t += stepSize) {
        Vector3 pos = Vector3Add(rayPos, Vector3Scale(rayDir, t));

        int blockX = (int)floor(pos.x);
        int blockY = (int)floor(pos.y);
        int blockZ = (int)floor(pos.z);

        if(blockX >= 0 && blockX < CHUNK_WIDTH && 
            blockY >= 0 && blockY < CHUNK_HEIGHT && 
            blockZ >= 0 && blockZ < CHUNK_DEPTH) {
            
            if(chunk->blocks[blockX][blockY][blockZ] != BLOCK_AIR) {
                player->hasTargetBlock = true;
                player->targetBlock = (Vector3) { blockX, blockY, blockZ };

                break;
            }
        }
    }
}

// This is a more accurate ray casting implementation using the DDA algorithm
// Digital Differential Analysis (DDA) is commonly used in voxel games for precise ray casting
void CastRayDDA(Camera3D camera, Player* player, Chunk* chunk) {
    player->hasTargetBlock = false;
    
    Vector3 rayDir = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 rayStart = camera.position;
    
    // Maximum distance
    float maxDist = 8.0f;
    
    // Current voxel indices
    int voxelX = (int)floor(rayStart.x);
    int voxelY = (int)floor(rayStart.y);
    int voxelZ = (int)floor(rayStart.z);
    
    // Direction to step in each dimension (+1 or -1)
    int stepX = rayDir.x > 0 ? 1 : -1;
    int stepY = rayDir.y > 0 ? 1 : -1;
    int stepZ = rayDir.z > 0 ? 1 : -1;
    
    // Distance from current position to next voxel boundary
    float nextVoxelBoundaryX = voxelX + (stepX > 0 ? 1 : 0);
    float nextVoxelBoundaryY = voxelY + (stepY > 0 ? 1 : 0);
    float nextVoxelBoundaryZ = voxelZ + (stepZ > 0 ? 1 : 0);
    
    // Distance the ray has to travel to cross a voxel boundary
    float tMaxX = rayDir.x != 0 ? (nextVoxelBoundaryX - rayStart.x) / rayDir.x : FLT_MAX;
    float tMaxY = rayDir.y != 0 ? (nextVoxelBoundaryY - rayStart.y) / rayDir.y : FLT_MAX;
    float tMaxZ = rayDir.z != 0 ? (nextVoxelBoundaryZ - rayStart.z) / rayDir.z : FLT_MAX;
    
    // Distance the ray travels per voxel
    float tDeltaX = rayDir.x != 0 ? (float)stepX / rayDir.x : FLT_MAX;
    float tDeltaY = rayDir.y != 0 ? (float)stepY / rayDir.y : FLT_MAX;
    float tDeltaZ = rayDir.z != 0 ? (float)stepZ / rayDir.z : FLT_MAX;
    
    // Face normal
    Vector3 faceNormal = {0};
    float rayLength = 0.0f;
    
    // Traverse the grid
    while (rayLength < maxDist) {
        // Find the closest voxel boundary
        if (tMaxX < tMaxY && tMaxX < tMaxZ) {
            voxelX += stepX;
            rayLength = tMaxX;
            tMaxX += tDeltaX;
            faceNormal = (Vector3){-stepX, 0, 0};
        } else if (tMaxY < tMaxZ) {
            voxelY += stepY;
            rayLength = tMaxY;
            tMaxY += tDeltaY;
            faceNormal = (Vector3){0, -stepY, 0};
        } else {
            voxelZ += stepZ;
            rayLength = tMaxZ;
            tMaxZ += tDeltaZ;
            faceNormal = (Vector3){0, 0, -stepZ};
        }
        
        // Check if the voxel is within bounds
        if (voxelX >= 0 && voxelX < CHUNK_WIDTH &&
            voxelY >= 0 && voxelY < CHUNK_HEIGHT &&
            voxelZ >= 0 && voxelZ < CHUNK_DEPTH) {
            
            // Check if the voxel is not air
            if (chunk->blocks[voxelX][voxelY][voxelZ] != BLOCK_AIR) {
                player->hasTargetBlock = true;
                player->targetBlock = (Vector3){voxelX, voxelY, voxelZ};
                player->targetFace = faceNormal;
                break;
            }
        } else {
            // Out of bounds
            break;
        }
    }
}
