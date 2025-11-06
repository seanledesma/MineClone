#include "include.h"

void InitPlayer(Player* player) {

    player->hasTargetBlock = false;
    player->targetBlockWorld = (Vector3) { 0 };
    player->prevTargetBlockPos = (Vector3) { 0 };

    // Define the camera to look into our 3d world (position, target, up vector)
    //player->camera;
    player->camera.position = (Vector3){ 0.0f, 2.0f, 5.0f };    // Camera position
    player->camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    player->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    player->camera.fovy = 60.0f;                                // Camera field-of-view Y
    player->camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type


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
