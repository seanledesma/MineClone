#include "include.h"

void InitPlayer(Player* player) {
    player->hasTargetBlock = false;
    player->targetBlockWorld = (Vector3) { 0 };
}


void UpdatePlayerInput(Player* player, ChunkTable* chunkTable){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if(player->hasTargetBlock) {
            //chunk->blocks[blockX][blockY][blockZ].blockType = BLOCK_AIR;
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
}
