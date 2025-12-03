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
    player->moveSpeed = 5.0f;  
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
                
                if(!IsBlockAir(chunkTable, targetChunk, blockX, blockY, blockZ)) {
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
                
                if(IsBlockAir(chunkTable, targetChunk, blockX, blockY, blockZ)) {
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

bool CheckCollisionAtPosition(ChunkTable* chunkTable, Vector3 position, Vector3 size) {
    // lets treat the player as a rectangular prism, check all end points + some mid points

    Vector3 checkPoints[] = {
        //bottom four corners
        { position.x - size.x / 2, position.y - size.y, position.z - size.z / 2 },
        { position.x - size.x / 2, position.y - size.y, position.z + size.z / 2 },
        { position.x + size.x / 2, position.y - size.y, position.z - size.z / 2 },
        { position.x + size.x / 2, position.y - size.y, position.z + size.z / 2 },
        //top four corners
        { position.x - size.x / 2, position.y, position.z - size.z / 2 },
        { position.x - size.x / 2, position.y, position.z + size.z / 2 },
        { position.x + size.x / 2, position.y, position.z - size.z / 2 },
        { position.x + size.x / 2, position.y, position.z + size.z / 2 },
        //middle point just in case
        { position.x, position.y - size.y / 2, position.z },
    };
    // now check all check points, lookup world block to chunk index to see if air block or not
    for (int i = 0; i < 9; i++) {
        Vector3 point = checkPoints[i];

        //convert to block coords in world space
        int worldBlockX = (int)floor(point.x);
        int worldBlockY = (int)floor(point.y);
        int worldBlockZ = (int)floor(point.z);
        //figure out which chunk this block is in
        int chunkX = (int)floor((point.x + HALF_CHUNK) / CHUNK_SIZE);
        int chunkY = (int)floor((point.y + HALF_CHUNK) / CHUNK_SIZE);
        int chunkZ = (int)floor((point.z + HALF_CHUNK) / CHUNK_SIZE);
        // try to get that chunk
        Chunk* targetChunk = get_chunk(chunkTable, chunkX, chunkY, chunkZ);
        if(!targetChunk) continue; //skip if that chunk doesn't exist yet
        //convert world coords to chunk-relative coords
        int blockX = worldBlockX - (int)floor(targetChunk->world_pos.x) + HALF_CHUNK;
        int blockY = worldBlockY - (int)floor(targetChunk->world_pos.y) + HALF_CHUNK;
        int blockZ = worldBlockZ - (int)floor(targetChunk->world_pos.z) + HALF_CHUNK;

        if(blockX >= 0 && blockX < CHUNK_SIZE && 
            blockY >= 0 && blockY < CHUNK_SIZE && 
            blockZ >= 0 && blockZ < CHUNK_SIZE) {
            
            if(!IsBlockAir(chunkTable, targetChunk, blockX, blockY, blockZ)) {
                return true; //collision detected
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

    //calculate desired velocity
    Vector3 desiredVelocity = {
        moveInput.x * currentSpeed,
        player->velocity.y,
        moveInput.z * currentSpeed
    };

    //jump
    if(IsKeyDown(KEY_SPACE) && player->isOnGround) {
        desiredVelocity.y = player->jumpForce;
        player->isOnGround = false;
    }

    //gravity
    if(!player->isOnGround) {
        desiredVelocity.y += player->gravity * dt;
    }

    /* actual collision detection here */
    /*
    * Here is what I'm thinking for a collision system: we only really have two spots to check, the top and the bottom of the player.
    * Neither should be in any block that is not air. We already know how to get a blocks real world position, so all we need to do
    * is set some sort of bounds check to make sure the block the player is entering is air, and if not reset the players position just outside. 
    *
    */
    Vector3 newPos = player->camera.position;
    // try to move on x axis
    newPos.x += desiredVelocity.x * dt;
    if (CheckCollisionAtPosition(chunkTable, newPos, player->collisionSize)) {
        desiredVelocity.x = 0; //stop x movement
        newPos.x = player->camera.position.x; // reset position to what it was before
    }

    //now try on y axis
    newPos.y = player->camera.position.y + desiredVelocity.y * dt;
    if (CheckCollisionAtPosition(chunkTable, newPos, player->collisionSize)) {
        if(desiredVelocity.y < 0) {
            player->isOnGround = true; //hit ground
        }
        desiredVelocity.y = 0; 
        newPos.y = player->camera.position.y; // reset y position
    } else {
        player->isOnGround = false; //we're aireborne 
    }

    //try to move on z axis
    newPos.z += desiredVelocity.z * dt;
    if (CheckCollisionAtPosition(chunkTable, newPos, player->collisionSize)) {
        desiredVelocity.z = 0; //stop x movement
        newPos.z = player->camera.position.z; // reset position to what it was before
    }

    player->camera.position = newPos;
    player->velocity = desiredVelocity;

    //update camera target
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
    }

    UpdatePlayerInput(player, chunkTable);
}
