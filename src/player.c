#include "include.h"

void InitPlayer(Player* player) {
    player->hasTargetBlock = false;
    player->targetBlockWorld = (Vector3) { 0 };
    player->prevTargetBlockPos = (Vector3) { 0 };
    player->isOnGround = false;
    player->isRunning = false;
    player->isFlying = false;

    player->camera.position = (Vector3){ 0.0f, 100.0f, 5.0f };
    player->camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    player->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    player->camera.fovy = 60.0f;
    player->camera.projection = CAMERA_PERSPECTIVE;

    player->velocity = (Vector3){ 0.0f, 0.0f, 0.0f };
    player->gravity = -20.0f;
    player->moveSpeed = 5.0f;  
    player->sprintMultiplier = 3.3f;
    player->jumpForce = 8.0f;
    player->mouseSensitivity = 0.003f;

    player->collisionSize = (Vector3){ 0.6f, 1.8f, 0.6f };
}


void UpdatePlayerInput(Player* player, ChunkTable* chunkTable){
    // LEFT CLICK - BREAK BLOCK
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if(player->hasTargetBlock) {
            int chunkX = (int)floor((player->targetBlockWorld.x + HALF_CHUNK) / CHUNK_SIZE);
            int chunkY = (int)floor((player->targetBlockWorld.y + HALF_CHUNK) / CHUNK_SIZE);
            int chunkZ = (int)floor((player->targetBlockWorld.z + HALF_CHUNK) / CHUNK_SIZE);
            
            Chunk* targetChunk = get_chunk(chunkTable, chunkX, chunkY, chunkZ);
            if(!targetChunk) return;
            
            // FIX: Allow modification if chunk is READY or REGENERATING
            // If regenerating, the current worker will see old data, but we'll 
            // mark needsRemesh so it gets regenerated again with the new block data
            if(targetChunk->state != CHUNK_STATE_READY && 
               targetChunk->state != CHUNK_STATE_REGENERATING) return;
            
            int blockX = (int)player->targetBlockWorld.x - (int)floor(targetChunk->world_pos.x) + HALF_CHUNK;
            int blockY = (int)player->targetBlockWorld.y - (int)floor(targetChunk->world_pos.y) + HALF_CHUNK;
            int blockZ = (int)player->targetBlockWorld.z - (int)floor(targetChunk->world_pos.z) + HALF_CHUNK;

            if(blockX >= 0 && blockX < CHUNK_SIZE && 
                blockY >= 0 && blockY < CHUNK_SIZE && 
                blockZ >= 0 && blockZ < CHUNK_SIZE) {
                
                if(!IsBlockAir(chunkTable, targetChunk, blockX, blockY, blockZ)) {
                    // FIX: Lock the chunk table mutex while modifying block data
                    // This prevents workers from reading partially modified data
                    mtx_lock(&chunk_table_mutex);
                    targetChunk->blocks[blockX][blockY][blockZ].blockType = BLOCK_AIR;
                    mtx_unlock(&chunk_table_mutex);

                    // Update neighbor chunks if on edge
                    UpdateNeighborChunkMesh(chunkTable, targetChunk, blockX, blockY, blockZ);

                    // Regenerate the current chunk's mesh
                    RegenerateChunk(chunkTable, targetChunk);
                }
            }
        }
    }
    
    // RIGHT CLICK - PLACE BLOCK
    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if(player->hasTargetBlock) {
            int worldBlockX = (int)floor(player->prevTargetBlockPos.x);
            int worldBlockY = (int)floor(player->prevTargetBlockPos.y);
            int worldBlockZ = (int)floor(player->prevTargetBlockPos.z);
            
            int chunkX = (int)floor((player->prevTargetBlockPos.x + HALF_CHUNK) / CHUNK_SIZE);
            int chunkY = (int)floor((player->prevTargetBlockPos.y + HALF_CHUNK) / CHUNK_SIZE);
            int chunkZ = (int)floor((player->prevTargetBlockPos.z + HALF_CHUNK) / CHUNK_SIZE);
            
            Chunk* targetChunk = get_chunk(chunkTable, chunkX, chunkY, chunkZ);
            if(!targetChunk) return;
            
            // FIX: Allow modification if chunk is READY or REGENERATING
            if(targetChunk->state != CHUNK_STATE_READY && 
               targetChunk->state != CHUNK_STATE_REGENERATING) return;
            
            int blockX = worldBlockX - (int)floor(targetChunk->world_pos.x) + HALF_CHUNK;
            int blockY = worldBlockY - (int)floor(targetChunk->world_pos.y) + HALF_CHUNK;
            int blockZ = worldBlockZ - (int)floor(targetChunk->world_pos.z) + HALF_CHUNK;

            if(blockX >= 0 && blockX < CHUNK_SIZE && 
                blockY >= 0 && blockY < CHUNK_SIZE && 
                blockZ >= 0 && blockZ < CHUNK_SIZE) {
                
                if(IsBlockAir(chunkTable, targetChunk, blockX, blockY, blockZ)) {
                    // Check if player would collide with the new block
                    Vector3 newBlockWorldPos = (Vector3){
                        (float)worldBlockX + 0.5f,
                        (float)worldBlockY + 0.5f,
                        (float)worldBlockZ + 0.5f
                    };
                    
                    // Simple collision check: is player's position too close to the new block?
                    float dist = Vector3Distance(player->camera.position, newBlockWorldPos);
                    if (dist < 1.5f) {
                        // Block would be placed inside player, don't allow it
                        return;
                    }
                    
                    // FIX: Lock the chunk table mutex while modifying block data
                    mtx_lock(&chunk_table_mutex);
                    targetChunk->blocks[blockX][blockY][blockZ].blockType = BLOCK_DIRT;
                    mtx_unlock(&chunk_table_mutex);

                    // Update neighbor chunks if on edge
                    UpdateNeighborChunkMesh(chunkTable, targetChunk, blockX, blockY, blockZ);

                    // Regenerate the current chunk's mesh
                    RegenerateChunk(chunkTable, targetChunk);
                }
            }
        }
    }
}

void UpdatePlayerCamera(Player* player, float dt) {
    (void)dt; // Unused parameter
    
    Vector2 mouseDelta = GetMouseDelta();

    float yaw = mouseDelta.x * player->mouseSensitivity;
    Vector3 forward = Vector3Subtract(player->camera.target, player->camera.position);
    forward = Vector3RotateByAxisAngle(forward, (Vector3) { 0,1,0 }, -yaw);

    float pitch = mouseDelta.y * player->mouseSensitivity;
    Vector3 right = Vector3CrossProduct(forward, player->camera.up);
    right = Vector3Normalize(right);

    Vector3 newForward = Vector3RotateByAxisAngle(forward, right, -pitch);

    float angle = Vector3Angle((Vector3) { 0,1,0 }, newForward);
    if(angle > 0.1f && angle < PI - 0.1f) {
        forward = newForward;
    }

    player->camera.target = Vector3Add(player->camera.position, forward);
}

bool CheckCollisionAtPosition(ChunkTable* chunkTable, Vector3 position, Vector3 size) {
    Vector3 checkPoints[] = {
        { position.x - size.x / 2, position.y - size.y, position.z - size.z / 2 },
        { position.x - size.x / 2, position.y - size.y, position.z + size.z / 2 },
        { position.x + size.x / 2, position.y - size.y, position.z - size.z / 2 },
        { position.x + size.x / 2, position.y - size.y, position.z + size.z / 2 },
        { position.x - size.x / 2, position.y, position.z - size.z / 2 },
        { position.x - size.x / 2, position.y, position.z + size.z / 2 },
        { position.x + size.x / 2, position.y, position.z - size.z / 2 },
        { position.x + size.x / 2, position.y, position.z + size.z / 2 },
        { position.x, position.y - size.y / 2, position.z },
    };
    
    for (int i = 0; i < 9; i++) {
        Vector3 point = checkPoints[i];

        int worldBlockX = (int)floor(point.x);
        int worldBlockY = (int)floor(point.y);
        int worldBlockZ = (int)floor(point.z);
        
        int chunkX = (int)floor((point.x + HALF_CHUNK) / CHUNK_SIZE);
        int chunkY = (int)floor((point.y + HALF_CHUNK) / CHUNK_SIZE);
        int chunkZ = (int)floor((point.z + HALF_CHUNK) / CHUNK_SIZE);
        
        Chunk* targetChunk = get_chunk(chunkTable, chunkX, chunkY, chunkZ);
        if(!targetChunk) continue;
        
        int blockX = worldBlockX - (int)floor(targetChunk->world_pos.x) + HALF_CHUNK;
        int blockY = worldBlockY - (int)floor(targetChunk->world_pos.y) + HALF_CHUNK;
        int blockZ = worldBlockZ - (int)floor(targetChunk->world_pos.z) + HALF_CHUNK;

        if(blockX >= 0 && blockX < CHUNK_SIZE && 
            blockY >= 0 && blockY < CHUNK_SIZE && 
            blockZ >= 0 && blockZ < CHUNK_SIZE) {
            
            if(!IsBlockAir(chunkTable, targetChunk, blockX, blockY, blockZ)) {
                return true;
            }
        }
    }
    return false;
}

void UpdatePlayerMovement(ChunkTable* chunkTable, Player* player, float dt) {
    Vector3 forward = Vector3Subtract(player->camera.target, player->camera.position);
    forward.y = 0;
    forward = Vector3Normalize(forward);

    Vector3 right = Vector3CrossProduct(forward, (Vector3) { 0,1,0 });
    right = Vector3Normalize(right);

    Vector3 moveInput = {0};

    if (IsKeyDown(KEY_W)) moveInput = Vector3Add(moveInput, forward);
    if (IsKeyDown(KEY_S)) moveInput = Vector3Subtract(moveInput, forward);
    if (IsKeyDown(KEY_D)) moveInput = Vector3Add(moveInput, right);
    if (IsKeyDown(KEY_A)) moveInput = Vector3Subtract(moveInput, right);

    if(Vector3Length(moveInput) > 0) {
        moveInput = Vector3Normalize(moveInput);
    }

    player->isRunning = IsKeyDown(KEY_W) && IsKeyDown(KEY_LEFT_SHIFT);
    float currentSpeed = player->moveSpeed;
    if(player->isRunning) {
        currentSpeed *= player->sprintMultiplier;
    }

    Vector3 desiredVelocity = {
        moveInput.x * currentSpeed,
        player->velocity.y,
        moveInput.z * currentSpeed
    };

    if(IsKeyDown(KEY_SPACE) && player->isOnGround) {
        desiredVelocity.y = player->jumpForce;
        player->isOnGround = false;
    }

    if(IsKeyDown(KEY_F)) {
        player->isFlying = true;
    }

    if(IsKeyDown(KEY_G)) {
        player->isFlying = false;
    }

    if(IsKeyDown(KEY_SPACE) && player->isFlying) {
        desiredVelocity.y += 0.1f;
    } else if (!IsKeyDown(KEY_SPACE) && player->isFlying) {
        desiredVelocity.y = 0;
    }

    if(!player->isOnGround && !player->isFlying) {
        desiredVelocity.y += player->gravity * dt;
    }

    Vector3 newPos = player->camera.position;
    
    newPos.x += desiredVelocity.x * dt;
    if (CheckCollisionAtPosition(chunkTable, newPos, player->collisionSize)) {
        desiredVelocity.x = 0;
        newPos.x = player->camera.position.x;
    }

    newPos.y = player->camera.position.y + desiredVelocity.y * dt;
    if (CheckCollisionAtPosition(chunkTable, newPos, player->collisionSize)) {
        if(desiredVelocity.y < 0) {
            player->isOnGround = true;
        }
        desiredVelocity.y = 0; 
        newPos.y = player->camera.position.y;
    } else if (player->isFlying) {
        // Flying - no changes needed
    } else {
        player->isOnGround = false;
    }

    newPos.z += desiredVelocity.z * dt;
    if (CheckCollisionAtPosition(chunkTable, newPos, player->collisionSize)) {
        desiredVelocity.z = 0;
        newPos.z = player->camera.position.z;
    }

    player->camera.position = newPos;
    player->velocity = desiredVelocity;

    Vector3 forward3d = Vector3Subtract(player->camera.target, 
                                        Vector3Subtract(player->camera.position, 
                                        Vector3Scale(player->velocity, dt)));
    player->camera.target = Vector3Add(player->camera.position, 
                                       Vector3Normalize(forward3d));
}

void UpdatePlayer(ChunkTable* chunkTable, Player* player, float dt) {
    UpdatePlayerCamera(player, dt);
    UpdatePlayerMovement(chunkTable, player, dt);

    player->targetBlockWorld = RayCastTargetBlock(player, chunkTable);
    if (player->targetBlockWorld.x != -1000) {
        player->hasTargetBlock = true;
    } else {
        player->hasTargetBlock = false;
    }

    UpdatePlayerInput(player, chunkTable);
}