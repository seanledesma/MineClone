/*
*   TODO:
*   - add multiple chunks, world data
*   - figure out tectures instead of cubes
*   - also different ground height
*   - and now need to expand wall collision logic
*/

//basic window
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <string.h>
#include <float.h>

#define screenWidth 1200
#define screenHeight 750
//#define EYE_LEVEL PLAYER_HEIGHT * 0.9;
// starting with smaller chunk sizes, can change later
const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 64;
const int CHUNK_DEPTH = 16;
const float PLAYER_HEIGHT = 2.0f;
const float PLAYER_RADIUS = 0.2f;
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
    Vector3 position;
    Vector3 nextPosition;
    Vector3 velocity;
    Vector3 floorBlock;
    Vector3 targetBlock;
    Vector3 targetFace;
    BlockType selectedBlock;
} Player;

typedef struct {
    float yaw;
    float pitch;
    Vector3 direction;
} CameraController;

// PROTOTYPES
void InitializeChunk(Chunk* chunk);
bool IsBlockVisible(Chunk*, int x, int y, int z);
void SetLookDirection(CameraController* camCtrl);
void CastRay(Camera3D camera, Player* player, Chunk* chunk);
float GetGroundY(Vector3 pos, Chunk* chunk);
bool CheckOnGround(Player* player, Chunk* chunk);
void UpdatePlayer(Player* player, Camera3D* camera, CameraController* camCtrl, Chunk* chunk, float deltaTime);
void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color); // Draw cube textured




int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raymine v0.0.1");
    SetTargetFPS(60);

    Texture2D grassTex = LoadTexture("assets/grass_full.png");
    Texture2D dirtTex = LoadTexture("assets/dirt.png");
    Texture2D stoneTex = LoadTexture("assets/cobblestone.png");

    SetTextureFilter(grassTex, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(grassTex, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(dirtTex, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(dirtTex, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(stoneTex, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(stoneTex, TEXTURE_WRAP_CLAMP);

    // create a single chunk for now
    Chunk chunk;
    InitializeChunk(&chunk);

    Player player = {
        .isOnGround = false,
        .hasTargetBlock = false,
        .position = (Vector3) { 0.0f, 50.0f, 5.0f },
        .nextPosition = (Vector3) { 0.0f, 50.0f, 5.0f },
        .velocity = (Vector3) { 0 },
        .selectedBlock = BLOCK_DIRT,
    };

    Camera3D camera = { 0 };
    camera.position = (Vector3) { player.position.x, player.position.y + PLAYER_HEIGHT * 0.9f, player.position.z };
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

        SetLookDirection(&camCtrl);

        CastRay(camera, &player, &chunk);

        UpdatePlayer(&player, &camera, &camCtrl, &chunk, deltaTime);


        BeginDrawing();
            ClearBackground(BLUE);

            BeginMode3D(camera);

                for(int x = 0; x < CHUNK_WIDTH; x++){
                    for(int y = 0; y < CHUNK_HEIGHT; y++) {
                        for(int z = 0; z < CHUNK_DEPTH; z++) {
                            //if block is NOT visible, skip that particular block (function will return T or F)
                            if(IsBlockVisible(&chunk, x, y, z) == false) continue;
                            //may consider placing this in it's own function
                            if(chunk.blocks[x][y][z] == BLOCK_GRASS) {
                                DrawCubeTexture(grassTex, (Vector3) { x+0.5f, y + 0.5f, z+0.5f }, 1.0f, 1.0f, 1.0f, WHITE);
                                //DrawCubeWiresV((Vector3) { x+0.5f, y + 0.5f, z+0.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLACK);
                            }else if(chunk.blocks[x][y][z] == BLOCK_DIRT) {
                                // DrawCubeV((Vector3) { x+0.5f, y + 0.5f, z+0.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, DARKBROWN);
                                // DrawCubeWiresV((Vector3) { x+0.5f, y + 0.5f, z+0.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLACK);
                                DrawCubeTexture(dirtTex, (Vector3) { x+0.5f, y + 0.5f, z+0.5f }, 1.0f, 1.0f, 1.0f, WHITE);
                            }else {
                                // DrawCubeV((Vector3) { x+0.5f, y + 0.5f, z+0.5f}, (Vector3) { 1.0f, 1.0f, 1.0f }, DARKGRAY);
                                // DrawCubeWiresV((Vector3) { x+0.5f, y + 0.5f, z+0.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLACK);
                                DrawCubeTexture(stoneTex, (Vector3) { x+0.5f, y + 0.5f, z+0.5f }, 1.0f, 1.0f, 1.0f, WHITE);
                            }
                        }
                    }
                }
                // highlighting target block
                if(player.hasTargetBlock) {
                    DrawCubeWiresV((Vector3){ 
                        player.targetBlock.x + 0.5f, player.targetBlock.y + 0.5f, player.targetBlock.z + 0.5f}, 
                        (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE);
                }

            EndMode3D();

            // crosshairs
            DrawLine(GetScreenWidth() / 2, GetScreenHeight() / 2 - 10, GetScreenWidth() / 2, GetScreenHeight() / 2 + 10, WHITE);
            DrawLine(GetScreenWidth() / 2 - 10, GetScreenHeight() / 2, GetScreenWidth() / 2 + 10, GetScreenHeight() / 2, WHITE);

            // debugging target block
            if(player.hasTargetBlock) {
                DrawText(TextFormat("Target block: X: %d Y: %d Z: %d", 
                        (int)player.targetBlock.x, (int)player.targetBlock.y, (int)player.targetBlock.z),
                        600, 40, 20, WHITE);
            } else {
                DrawText("No target block", 300, 40, 20, RED);
            }

            //debugging player position
            DrawText(TextFormat("player pos: X: %.2f Y: %.2f FootY: %.2f Z: %.2f",
                                player.position.x, player.position.y, player.position.y - (PLAYER_HEIGHT * 0.5f), player.position.z),
                                300, 10, 20, RAYWHITE);

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
            chunk->blocks[x][y][z+1] != BLOCK_AIR) {
                return false;
        }
    }
    return true;
}


void SetLookDirection(CameraController* camCtrl){
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
    camCtrl->direction = (Vector3) {
        cosf(camCtrl->pitch) * sinf(camCtrl->yaw),
        sinf(camCtrl->pitch),
        cosf(camCtrl->pitch) * cosf(camCtrl->yaw)
    };
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

float GetGroundY(Vector3 pos, Chunk* chunk) {
    int blockX = (int)floorf(pos.x);
    int blockZ = (int)floorf(pos.z);

    //will need to change this when adding more chunks
    if(blockX < 0) blockX = 0;
    if(blockX >= CHUNK_WIDTH) blockX = CHUNK_WIDTH - 1;
    if(blockZ < 0) blockZ = 0;
    if(blockZ >= CHUNK_DEPTH) blockZ = CHUNK_DEPTH - 1;

    for(int y = (int)floor(pos.y); y >= 0; y--) {
        if(chunk->blocks[blockX][y][blockZ] != BLOCK_AIR) {
            return (float) y + 1.0f;
        }
    }
    //no block found means i'm over void, snap to y = 0;
    return 0.0f;
}

bool CheckOnGround(Player* player, Chunk* chunk) {
    float footY = player->nextPosition.y - (PLAYER_HEIGHT / 2);

    float groundY = GetGroundY(player->nextPosition, chunk);

    player->floorBlock = (Vector3) { player->nextPosition.x, groundY, player->nextPosition.z };

    const float epsilon = 0.01f;

    return (footY <= groundY + epsilon);

}


void UpdatePlayer(Player* player, Camera3D* camera, CameraController* camCtrl, Chunk* chunk, float deltaTime) {
    if(player->hasTargetBlock) {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int x = (int)player->targetBlock.x;
            int y = (int)player->targetBlock.y;
            int z = (int)player->targetBlock.z;

            if(x >= 0 && x < CHUNK_WIDTH && 
            y >= 0 && y < CHUNK_HEIGHT && 
            z >= 0 && z < CHUNK_DEPTH) {
                chunk->blocks[x][y][z] = BLOCK_AIR;
            }


        }
    }
    // horizontal movement
    Vector3 forward = { camCtrl->direction.x, 0.0f, camCtrl->direction.z };
    Vector3 right = { -forward.z, 0.0f, forward.x };
    Vector3 move = Vector3Zero();
    float speed = 5.0f;
    // moving x and z
    if (IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
    if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
    if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);
    if (IsKeyDown(KEY_D)) move = Vector3Add(move, right);

    if (Vector3Length(move) > 0.0f)
        move = Vector3Scale(Vector3Normalize(move), speed * deltaTime);
    
    //jumping
    if(player->isOnGround && IsKeyPressed(KEY_SPACE)) {
        player->velocity.y = 5.0f;
        player->isOnGround = false;
    }
    player->velocity.y += GRAVITY * deltaTime;

    //build next position
    player->nextPosition = player->position;
    player->nextPosition.x += move.x;
    player->nextPosition.z += move.z;
    player->nextPosition.y += player->velocity.y * deltaTime;
    


    //if player is in the air, fall down. (may update this to add free mode)
    // float footY = player->nextPosition.y - (PLAYER_HEIGHT * 0.5f);
    // float groundY = GetGroundY(player->nextPosition, chunk);
    // const float eps = 0.01f;

    // if(footY <= groundY + eps) {
    //     player->nextPosition.y = groundY + (PLAYER_HEIGHT * 0.5f);
    //     player->velocity.y = 0;
    //     player->isOnGround = true;
    // } else {
    //     player->isOnGround = false;
    // }

    if(CheckOnGround(player, chunk)){
        player->nextPosition.y = player->floorBlock.y + (PLAYER_HEIGHT / 2);
        player->velocity.y = 0;
        player->isOnGround = true;
    } else {
        player->isOnGround = false;
    }


    //figuring out wall collision
    //could make this better buy making a radius around player pos, checking that rather than the single player pos point
    int footY = (int)floorf(player->nextPosition.y - (PLAYER_HEIGHT / 2.0f) + 0.1f);
    int bx = (int)floorf(player->nextPosition.x);
    int bz = (int)floorf(player->nextPosition.z);
    if (chunk->blocks[bx][footY][bz] == BLOCK_AIR) {
        player->position = player->nextPosition;
    }








    camera->position = (Vector3) { 
        player->position.x, 
        //player position is at CENTER of player, so at 1, plus almost a full block for eye level
        player->position.y + PLAYER_HEIGHT * 0.4f,
        player->position.z
    };
    camera->target = Vector3Add(camera->position, camCtrl->direction);
    camera->up = (Vector3) { 0.0f, 1.0f, 0.0f };

}

// yoinked from one of the Raylib examples
void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // Vertex data transformation can be defined with the commented lines,
    // but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
    //rlPushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        //rlTranslatef(2.0f, 0.0f, 0.0f);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(2.0f, 2.0f, 2.0f);

        rlBegin(RL_QUADS);
            rlColor4ub(color.r, color.g, color.b, color.a);
            // Front Face
            rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
            // Back Face
            rlNormal3f(0.0f, 0.0f, - 1.0f);     // Normal Pointing Away From Viewer
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
            // Top Face
            rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            // Bottom Face
            rlNormal3f(0.0f, - 1.0f, 0.0f);     // Normal Pointing Down
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            // Right face
            rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            // Left Face
            rlNormal3f( - 1.0f, 0.0f, 0.0f);    // Normal Pointing Left
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
        rlEnd();
    //rlPopMatrix();

    rlSetTexture(0);
}