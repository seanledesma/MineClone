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
    Vector3 position;
    Vector3 velocity;
    float height;
} Player;

// PROTOTYPES
void InitializeChunk(Chunk* chunk);
bool IsBlockVisible(Chunk*, int x, int y, int z);
void UpdatePlayer(Player* player, Camera3D* camera, Chunk* chunk, float deltaTime);
Vector3 BlockSelector(Camera3D camera, Chunk* chunk);



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
        .position = (Vector3) { 0.0f, 60.0f, 5.0f },
        .velocity = (Vector3) { 0 },
        .height = 2.0f,
    };

    Camera3D camera = { 0 };
    camera.position = (Vector3) { player.position.x, player.position.y + player.height * 0.9f, player.position.z };
    camera.target = (Vector3) { 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;






    DisableCursor();
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        //UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        UpdatePlayer(&player, &camera, &chunk, deltaTime);


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

            DrawText(TextFormat("player pos: X: %.2f Y: %.2f Z: %.2f",
                                player.position.x, player.position.y, player.position.z),
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

void UpdatePlayer(Player* player, Camera3D* camera, Chunk* chunk, float deltaTime) {
    UpdateCamera(camera, CAMERA_FIRST_PERSON);
    player->position.x = camera->position.x;
    player->position.z = camera->position.z;
    //camera->position = (Vector3) { player->position.x, player->position.y + 1.8, player->position.x };

    //next, need to figure out what block we are over, and if we are right on top of it
    Vector3 floorBlock = { 0 };
    for(int i = (int) player->position.y; i >= 0; i--) {
        //TODO: MAKE SURE WE ARE ONLY PASSING THE CURRENT CHUNK TO THIS FUNC
        // bugs could come from all this type casting :( watch out)
        if(player->position.x > CHUNK_WIDTH || player->position.x < 0 || player->position.z > CHUNK_DEPTH || player->position.z < 0) {
            //if we went off the chunk, (this will probably need to change when we add more chunks)
            player->isOnGround = false;
        } else if(chunk->blocks[(int) player->position.x][(int) player->position.y][(int) player->position.z] != BLOCK_AIR) {
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
    camera->position.y = player->position.y + player->height * 0.9f;

}

Vector3 BlockSelector(Camera3D camera, Chunk* chunk) {
    
}