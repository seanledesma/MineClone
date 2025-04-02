#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

// Constants
#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 64
#define CHUNK_SIZE_Z 16
#define BLOCK_SIZE 1.0f
#define PLAYER_SPEED 5.0f
#define PLAYER_JUMP 4.0f
#define GRAVITY 9.8f

// Block types
typedef enum {
    BLOCK_AIR,
    BLOCK_DIRT,
    BLOCK_GRASS,
    BLOCK_STONE,
    BLOCK_COUNT
} BlockType;

// Chunk structure
typedef struct {
    BlockType blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    Vector3 position;
} Chunk;

// Player structure
typedef struct {
    Vector3 position;
    Vector3 velocity;
    bool isOnGround;
    BlockType selectedBlock;
    bool hasTargetBlock;
    Vector3 targetBlock;
    Vector3 targetFace;
} Player;

// Function prototypes
void InitializeChunk(Chunk* chunk, Vector3 position);
void DrawChunk(Chunk* chunk, Texture2D atlas);
bool IsBlockVisible(Chunk* chunk, int x, int y, int z);
void UpdatePlayer(Player* player, Camera* camera, Chunk* chunk, float deltaTime);
void ProcessInput(Player* player, Camera* camera, Chunk* chunk, float deltaTime);
void CastRay(Player* player, Camera camera, Chunk* chunk);
Color GetBlockColor(BlockType type);

// Main function
int main(void)
{
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Minecraft Clone");
    SetTargetFPS(60);
    
    // Create camera
    Camera camera = { 0 };
    camera.position = (Vector3){ 8.0f, 20.0f, 8.0f };
    camera.target = (Vector3){ 8.0f, 20.0f, 9.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Setup textures (simplified - just colors for now)
    Image img = GenImageChecked(32, 32, 8, 8, RED, GREEN);
    Texture2D atlas = LoadTextureFromImage(img);
    UnloadImage(img);
    
    // Create a single chunk
    Chunk chunk;
    InitializeChunk(&chunk, (Vector3){ 0.0f, 0.0f, 0.0f });
    
    // Create player
    Player player = {0};
    player.position = (Vector3){ 8.0f, 20.0f, 8.0f };
    player.velocity = (Vector3){ 0.0f, 0.0f, 0.0f };
    player.isOnGround = false;
    player.selectedBlock = BLOCK_DIRT;
    player.hasTargetBlock = false;
    
    // Mouse for camera
    DisableCursor();
    
    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        float deltaTime = GetFrameTime();
        
        // Camera controls
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        
        // Player update
        UpdatePlayer(&player, &camera, &chunk, deltaTime);
        
        // Process player input for block interaction
        ProcessInput(&player, &camera, &chunk, deltaTime);
        
        // Ray casting for block selection
        CastRay(&player, camera, &chunk);
        
        // Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);
            
            BeginMode3D(camera);
                // Draw chunk
                DrawChunk(&chunk, atlas);
                
                // Draw selection box
                if (player.hasTargetBlock) {
                    DrawCubeWires(
                        (Vector3){ 
                            player.targetBlock.x + 0.5f, 
                            player.targetBlock.y + 0.5f, 
                            player.targetBlock.z + 0.5f 
                        },
                        1.0f, 1.0f, 1.0f, BLACK);
                }
            EndMode3D();
            
            // UI
            DrawFPS(10, 10);
            DrawText("WASD to move, Space to jump", 10, 30, 20, BLACK);
            DrawText("Left click to remove, Right click to place", 10, 50, 20, BLACK);
            DrawText("1-3 to select block type", 10, 70, 20, BLACK);
            
            // Crosshair
            DrawLine(screenWidth/2 - 10, screenHeight/2, screenWidth/2 + 10, screenHeight/2, BLACK);
            DrawLine(screenWidth/2, screenHeight/2 - 10, screenWidth/2, screenHeight/2 + 10, BLACK);
            
        EndDrawing();
    }
    
    // Cleanup
    UnloadTexture(atlas);
    CloseWindow();
    
    return 0;
}

// Initialize chunk with terrain
void InitializeChunk(Chunk* chunk, Vector3 position) {
    chunk->position = position;
    
    // Initialize with air
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                chunk->blocks[x][y][z] = BLOCK_AIR;
            }
        }
    }
    
    // Generate simple terrain
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            // Simple height formula
            int height = 12 + sin(x*0.5f)*2 + cos(z*0.5f)*2;
            
            // Fill up to height
            for (int y = 0; y < height; y++) {
                if (y == height - 1) {
                    chunk->blocks[x][y][z] = BLOCK_GRASS;
                } else if (y > height - 4) {
                    chunk->blocks[x][y][z] = BLOCK_DIRT;
                } else {
                    chunk->blocks[x][y][z] = BLOCK_STONE;
                }
            }
        }
    }
}

// Check if a block is visible (has at least one face exposed)
bool IsBlockVisible(Chunk* chunk, int x, int y, int z) {
    // Air blocks are not visible
    if (chunk->blocks[x][y][z] == BLOCK_AIR) return false;
    
    // Check all 6 faces
    // Check boundary cases first
    if (x == 0 || x == CHUNK_SIZE_X - 1 || 
        y == 0 || y == CHUNK_SIZE_Y - 1 || 
        z == 0 || z == CHUNK_SIZE_Z - 1) {
        return true;
    }
    
    // Check neighboring blocks
    if (chunk->blocks[x-1][y][z] == BLOCK_AIR) return true;
    if (chunk->blocks[x+1][y][z] == BLOCK_AIR) return true;
    if (chunk->blocks[x][y-1][z] == BLOCK_AIR) return true;
    if (chunk->blocks[x][y+1][z] == BLOCK_AIR) return true;
    if (chunk->blocks[x][y][z-1] == BLOCK_AIR) return true;
    if (chunk->blocks[x][y][z+1] == BLOCK_AIR) return true;
    
    return false;
}

// Draw a chunk
void DrawChunk(Chunk* chunk, Texture2D atlas) {
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                if (IsBlockVisible(chunk, x, y, z)) {
                    // Draw the block with appropriate color
                    DrawCube(
                        (Vector3){ 
                            chunk->position.x + x + 0.5f, 
                            chunk->position.y + y + 0.5f, 
                            chunk->position.z + z + 0.5f 
                        },
                        1.0f, 1.0f, 1.0f, GetBlockColor(chunk->blocks[x][y][z]));
                }
            }
        }
    }
}

// Get color for block type
Color GetBlockColor(BlockType type) {
    switch (type) {
        case BLOCK_DIRT: return BROWN;
        case BLOCK_GRASS: return GREEN;
        case BLOCK_STONE: return GRAY;
        default: return WHITE;
    }
}

// Update player physics and position
void UpdatePlayer(Player* player, Camera* camera, Chunk* chunk, float deltaTime) {
    // Apply gravity
    if (!player->isOnGround) {
        player->velocity.y -= GRAVITY * deltaTime;
    }
    
    // Apply velocity
    player->position.x += player->velocity.x * deltaTime;
    player->position.y += player->velocity.y * deltaTime;
    player->position.z += player->velocity.z * deltaTime;
    
    // Simple collision with ground
    int blockX = (int)floor(player->position.x);
    int blockY = (int)floor(player->position.y - 0.5f); // Check slightly below feet
    int blockZ = (int)floor(player->position.z);
    
    // Make sure we're within chunk boundaries
    if (blockX >= 0 && blockX < CHUNK_SIZE_X &&
        blockY >= 0 && blockY < CHUNK_SIZE_Y &&
        blockZ >= 0 && blockZ < CHUNK_SIZE_Z) {
        
        if (chunk->blocks[blockX][blockY][blockZ] != BLOCK_AIR) {
            player->position.y = blockY + 1.5f;
            player->velocity.y = 0;
            player->isOnGround = true;
        } else {
            player->isOnGround = false;
        }
    }
    
    // Update camera position to match player
    camera->position = player->position;
}

// Process player input
void ProcessInput(Player* player, Camera* camera, Chunk* chunk, float deltaTime) {
    // Reset horizontal velocity
    player->velocity.x = 0;
    player->velocity.z = 0;
    
    // Calculate forward and right vectors from camera
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    forward.y = 0; // No vertical component for movement
    forward = Vector3Normalize(forward);
    
    Vector3 right = Vector3Normalize(
        Vector3CrossProduct(forward, (Vector3){ 0, 1, 0 })
    );
    
    // Handle movement
    if (IsKeyDown(KEY_W)) {
        player->velocity.x += forward.x * PLAYER_SPEED;
        player->velocity.z += forward.z * PLAYER_SPEED;
    }
    if (IsKeyDown(KEY_S)) {
        player->velocity.x -= forward.x * PLAYER_SPEED;
        player->velocity.z -= forward.z * PLAYER_SPEED;
    }
    if (IsKeyDown(KEY_A)) {
        player->velocity.x -= right.x * PLAYER_SPEED;
        player->velocity.z -= right.z * PLAYER_SPEED;
    }
    if (IsKeyDown(KEY_D)) {
        player->velocity.x += right.x * PLAYER_SPEED;
        player->velocity.z += right.z * PLAYER_SPEED;
    }
    
    // Jump
    if (IsKeyDown(KEY_SPACE) && player->isOnGround) {
        player->velocity.y = PLAYER_JUMP;
        player->isOnGround = false;
    }
    
    // Block selection
    if (IsKeyPressed(KEY_ONE)) player->selectedBlock = BLOCK_DIRT;
    if (IsKeyPressed(KEY_TWO)) player->selectedBlock = BLOCK_GRASS;
    if (IsKeyPressed(KEY_THREE)) player->selectedBlock = BLOCK_STONE;
    
    // Block interaction
    if (player->hasTargetBlock) {
        // Remove block (left click)
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int x = (int)player->targetBlock.x;
            int y = (int)player->targetBlock.y;
            int z = (int)player->targetBlock.z;
            
            if (x >= 0 && x < CHUNK_SIZE_X &&
                y >= 0 && y < CHUNK_SIZE_Y &&
                z >= 0 && z < CHUNK_SIZE_Z) {
                chunk->blocks[x][y][z] = BLOCK_AIR;
            }
        }
        
        // Place block (right click)
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            int x = (int)(player->targetBlock.x + player->targetFace.x);
            int y = (int)(player->targetBlock.y + player->targetFace.y);
            int z = (int)(player->targetBlock.z + player->targetFace.z);
            
            if (x >= 0 && x < CHUNK_SIZE_X &&
                y >= 0 && y < CHUNK_SIZE_Y &&
                z >= 0 && z < CHUNK_SIZE_Z &&
                chunk->blocks[x][y][z] == BLOCK_AIR) {
                // Check if block would intersect with player
                if (x != (int)player->position.x ||
                    y != (int)player->position.y ||
                    z != (int)player->position.z) {
                    chunk->blocks[x][y][z] = player->selectedBlock;
                }
            }
        }
    }
}

// Cast ray from camera to find block
void CastRay(Player* player, Camera camera, Chunk* chunk) {
    player->hasTargetBlock = false;
    
    // Get ray direction from camera
    Vector3 rayDir = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    
    // Ray starting point
    Vector3 rayPos = camera.position;
    
    // Maximum distance
    float maxDist = 5.0f;
    float step = 0.1f;
    
    // Step along ray
    for (float t = 0; t < maxDist; t += step) {
        Vector3 pos = Vector3Add(rayPos, Vector3Scale(rayDir, t));
        int x = (int)floor(pos.x);
        int y = (int)floor(pos.y);
        int z = (int)floor(pos.z);
        
        // Check if in chunk bounds
        if (x >= 0 && x < CHUNK_SIZE_X &&
            y >= 0 && y < CHUNK_SIZE_Y &&
            z >= 0 && z < CHUNK_SIZE_Z) {
            
            // Check if hit a block
            if (chunk->blocks[x][y][z] != BLOCK_AIR) {
                player->hasTargetBlock = true;
                player->targetBlock = (Vector3){ x, y, z };
                
                // Calculate which face was hit (simplified)
                float dx = pos.x - floor(pos.x) - 0.5f;
                float dy = pos.y - floor(pos.y) - 0.5f;
                float dz = pos.z - floor(pos.z) - 0.5f;
                
                // Find largest component to determine face
                if (fabs(dx) > fabs(dy) && fabs(dx) > fabs(dz)) {
                    player->targetFace = (Vector3){ dx > 0 ? 1.0f : -1.0f, 0, 0 };
                } else if (fabs(dy) > fabs(dx) && fabs(dy) > fabs(dz)) {
                    player->targetFace = (Vector3){ 0, dy > 0 ? 1.0f : -1.0f, 0 };
                } else {
                    player->targetFace = (Vector3){ 0, 0, dz > 0 ? 1.0f : -1.0f };
                }
                
                break;
            }
        }
    }
}