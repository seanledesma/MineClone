#include "include.h"

void InitPlayer(Player* player) {
    //state
    player->hasTargetBlock = false;
    player->targetBlockWorld = (Vector3) { 0 };
    player->prevTargetBlockPos = (Vector3) { 0 };
    player->isOnGround = false;
    player->isRunning = false;

    // Define the camera to look into our 3d world (position, target, up vector)
    //player->camera;
    player->camera.position = (Vector3){ 0.0f, 2.0f, 5.0f };    // Camera position
    player->camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    player->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    player->camera.fovy = 60.0f;                                // Camera field-of-view Y
    player->camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    //physics
    player->velocity = (Vector3){ 0.0f, 0.0f, 0.0f };
    player->gravity = -20.0f;  // blocks per second squared
    player->moveSpeed = 4.3f;  
    player->sprintMultiplier = 3.3f;
    player->jumpForce = 8.0f;
    player->mouseSensitivity = 0.003f;

    player->collisionSize = (Vector3){ 0.6f, 1.8f, 0.6f };
}


void UpdatePlayerInput(Player* player, ChunkTable* chunkTable){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if(player->hasTargetBlock) {
            //figure out which chunk this block is in
            int chunkX = (int)floor((player->targetBlockWorld.x + HALF_CHUNK) / CHUNK_SIZE);
            int chunkY = (int)floor((player->targetBlockWorld.y + HALF_CHUNK) / CHUNK_SIZE);
            int chunkZ = (int)floor((player->targetBlockWorld.z + HALF_CHUNK) / CHUNK_SIZE);
            // try to get that chunk
            Chunk* targetChunk = get_chunk(chunkTable, chunkX, chunkY, chunkZ);
            if(!targetChunk) return; //skip if that chunk doesn't exist yet
            //convert world coords to chunk-relative coords
            int blockX = player->targetBlockWorld.x - (int)floor(targetChunk->world_pos.x) + HALF_CHUNK;
            int blockY = player->targetBlockWorld.y - (int)floor(targetChunk->world_pos.y) + HALF_CHUNK;
            int blockZ = player->targetBlockWorld.z - (int)floor(targetChunk->world_pos.z) + HALF_CHUNK;

            if(blockX >= 0 && blockX < CHUNK_SIZE && 
                blockY >= 0 && blockY < CHUNK_SIZE && 
                blockZ >= 0 && blockZ < CHUNK_SIZE) {
                
                if(!IsBlockAir(targetChunk, blockX, blockY, blockZ)) {
                    targetChunk->blocks[blockX][blockY][blockZ].blockType = BLOCK_AIR;
                }
            }
        }
    }
    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if(player->hasTargetBlock) {
            //convert ray to block coords in world space
            int worldBlockX = (int)floor(player->prevTargetBlockPos.x);
            int worldBlockY = (int)floor(player->prevTargetBlockPos.y);
            int worldBlockZ = (int)floor(player->prevTargetBlockPos.z);
            //figure out which chunk this block is in
            int chunkX = (int)floor((player->prevTargetBlockPos.x + HALF_CHUNK) / CHUNK_SIZE);
            int chunkY = (int)floor((player->prevTargetBlockPos.y + HALF_CHUNK) / CHUNK_SIZE);
            int chunkZ = (int)floor((player->prevTargetBlockPos.z + HALF_CHUNK) / CHUNK_SIZE);
            // try to get that chunk
            Chunk* targetChunk = get_chunk(chunkTable, chunkX, chunkY, chunkZ);
            if(!targetChunk) return; //skip if that chunk doesn't exist yet
            //convert world coords to chunk-relative coords
            int blockX = worldBlockX - (int)floor(targetChunk->world_pos.x) + HALF_CHUNK;
            int blockY = worldBlockY - (int)floor(targetChunk->world_pos.y) + HALF_CHUNK;
            int blockZ = worldBlockZ - (int)floor(targetChunk->world_pos.z) + HALF_CHUNK;

            if(blockX >= 0 && blockX < CHUNK_SIZE && 
                blockY >= 0 && blockY < CHUNK_SIZE && 
                blockZ >= 0 && blockZ < CHUNK_SIZE) {
                
                if(IsBlockAir(targetChunk, blockX, blockY, blockZ)) {
                    targetChunk->blocks[blockX][blockY][blockZ].blockType = BLOCK_DIRT;
                }
            }
        }
    }
}

void UpdatePlayerCamera(Player* player, float dt) {
    // yoinked from Claude :)
    // mouse look (rotation)
    Vector2 mouseDelta = GetMouseDelta();

    //yaw
    float yaw = mouseDelta.x * player->mouseSensitivity;
    Vector3 forward = Vector3Subtract(player->camera.target, player->camera.position);
    forward = Vector3RotateByAxisAngle(forward, (Vector3) { 0,1,0 }, -yaw);

    //pitch
    float pitch = mouseDelta.y * player->mouseSensitivity;
    Vector3 right = Vector3CrossProduct(forward, player->camera.up);
    right = Vector3Normalize(right);

    Vector3 newForward = Vector3RotateByAxisAngle(forward, right, -pitch);

    //clamp pitch
    float angle = Vector3Angle((Vector3) { 0,1,0 }, newForward);
    if(angle > 0.1f && angle < PI - 0.1f) {
        forward = newForward;
    }

    player->camera.target = Vector3Add(player->camera.position, forward);
}

void UpdatePlayerMovement(Player* player, float dt) {
    Vector3 forward = Vector3Subtract(player->camera.target, player->camera.position);
    forward.y = 0;
    forward = Vector3Normalize(forward);

    Vector3 right = Vector3CrossProduct(forward, (Vector3) { 0,1,0 });
    right = Vector3Normalize(right);

    //input
    Vector3 moveInput = {0};

    if (IsKeyDown(KEY_W)) moveInput = Vector3Add(moveInput, forward);
    if (IsKeyDown(KEY_S)) moveInput = Vector3Subtract(moveInput, forward);
    if (IsKeyDown(KEY_D)) moveInput = Vector3Add(moveInput, right);
    if (IsKeyDown(KEY_A)) moveInput = Vector3Subtract(moveInput, right);

    //normalize diagonal movement 
    if(Vector3Length(moveInput) > 0) {
        moveInput = Vector3Normalize(moveInput);
    }

    //sprint
    player->isRunning = IsKeyDown(KEY_W) && IsKeyDown(KEY_LEFT_SHIFT);
    float currentSpeed = player->moveSpeed;
    if(player->isRunning) {
        currentSpeed *= player->sprintMultiplier;
    }

    //apply horizontal movement
    player->velocity.x = moveInput.x * currentSpeed;
    player->velocity.z = moveInput.z * currentSpeed;

    //jump
    if(IsKeyPressed(KEY_SPACE) && player->isOnGround) {
        player->velocity.y = player->jumpForce;
        player->isOnGround = false;
    }

    //gravity
    if(!player->isOnGround) {
        player->velocity.y += player->gravity * dt;
    }

    //apply velocity to position
    player->camera.position.x += player->velocity.x * dt;
    player->camera.position.y += player->velocity.y * dt;
    player->camera.position.z += player->velocity.z * dt;
    
    //update camera target
    Vector3 forward3d = Vector3Subtract(player->camera.target, 
                                        Vector3Subtract(player->camera.position, 
                                        Vector3Scale(player->velocity, dt)));
    player->camera.target = Vector3Add(player->camera.position, 
                                       Vector3Normalize(forward3d));
}

void UpdatePlayer(Player* player, float dt) {
    UpdatePlayerCamera(player, dt);
    UpdatePlayerMovement(player, dt);

    // TODO: Add collision detection here
    // For now, simple ground check
    if (player->camera.position.y <= 1.8f) {  // Assuming ground at y=0
        player->camera.position.y = 1.8f;
        player->velocity.y = 0;
        player->isOnGround = true;
    }
}
