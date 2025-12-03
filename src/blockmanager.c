#include "include.h"


bool IsBlockAir(ChunkTable* chunkTable, Chunk* chunk, int x, int y, int z) {
    if (!chunk) return true; //treat null chunks as air
    // need to see if block is even in current chunk
    // will need to handle this different when making more chunks visible
    //if (x <)
    // gonna do some safeguarding to prevent checking outside current chunk
    if (x < 0 || x >= CHUNK_SIZE ||
        y < 0 || y >= CHUNK_SIZE ||
        z < 0 || z >= CHUNK_SIZE) {

        // I am taking the easy way out, instead of properly checking surrounding chunks, i'll return true
        // what I mean is checking out of bounds doesn't take into account neighboring chunk block types.
        //return false; //treat everything outside chunk as air? turn true?
        //return true;
        //return chunk->blocks[x][y][z].blockType == BLOCK_AIR;
        if (x == -1 || x == CHUNK_SIZE  ||
            y == -1 || y == CHUNK_SIZE  ||
            z == -1 || z == CHUNK_SIZE ) {
            /* if the block we want to check is outside this current chunk by just one block,
            *  then I'll get the world pos, use that to get adjacent block info, then pass that back recursively 
            */
            int worldX = chunk->world_pos.x + x;
            int worldY = chunk->world_pos.y + y;
            int worldZ = chunk->world_pos.z + z;
            
            //figure out which chunk this block is in
            int chunkX = (int)floor((worldX + HALF_CHUNK) / CHUNK_SIZE);
            int chunkY = (int)floor((worldY + HALF_CHUNK) / CHUNK_SIZE);
            int chunkZ = (int)floor((worldZ + HALF_CHUNK) / CHUNK_SIZE);
            // try to get that chunk
            Chunk* targetChunk = get_chunk(chunkTable, chunkX, chunkY, chunkZ);
            //if(!targetChunk) return (Vector3) {0}; //skip if that chunk doesn't exist yet
            //convert world coords to chunk-relative coords
            int blockX = worldX - (int)floor(targetChunk->world_pos.x) + HALF_CHUNK;
            int blockY = worldY - (int)floor(targetChunk->world_pos.y) + HALF_CHUNK;
            int blockZ = worldZ - (int)floor(targetChunk->world_pos.z) + HALF_CHUNK;

            return IsBlockAir(chunkTable, targetChunk, blockX, blockY, blockZ);

        }
        return false;

    }

    if (chunk->blocks[x][y][z].blockType == BLOCK_AIR) {
        return true;
    }
    return false;
}

BlockType DecideBlockType(Chunk* new_chunk, int absolute_x, int absolute_y, int absolute_z) {
    // to shut up pedantic errors
    (void)new_chunk;
    //(void)absolute_x;
    //(void)absolute_z;
    fnl_state noise = fnlCreateState();
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    float heightF = fnlGetNoise2D(&noise, absolute_x, absolute_z);

    int height = (int) floor(heightF * 5);

    if (absolute_y == height) {
        return BLOCK_GRASS;
        //return BLOCK_DIRT;
    }else if (absolute_y < height && absolute_y > -20) {
        //new_chunk->blocks[x][y][z].blockType = BLOCK_DIRT;
        return BLOCK_DIRT;
    }else if (absolute_y <= -20) {
        return BLOCK_STONE;
    } else {
        //new_chunk->blocks[x][y][z].blockType = BLOCK_AIR;
        return BLOCK_AIR;
    }
}

/*converts world block coords to chunk-relative index*/
// this is not going to work due to not knowing which chunk the indexs' belong to
Vector3 ConvWorldCoords(ChunkTable* chunkTable, Vector3 pos) {
    //convert pos to block coords in world space
    int worldBlockX = (int)floor(pos.x);
    int worldBlockY = (int)floor(pos.y);
    int worldBlockZ = (int)floor(pos.z);
    //figure out which chunk this block is in
    int chunkX = (int)floor((pos.x + HALF_CHUNK) / CHUNK_SIZE);
    int chunkY = (int)floor((pos.y + HALF_CHUNK) / CHUNK_SIZE);
    int chunkZ = (int)floor((pos.z + HALF_CHUNK) / CHUNK_SIZE);
    // try to get that chunk
    Chunk* targetChunk = get_chunk(chunkTable, chunkX, chunkY, chunkZ);
    if(!targetChunk) return (Vector3) {0}; //skip if that chunk doesn't exist yet
    //convert world coords to chunk-relative coords
    int blockX = worldBlockX - (int)floor(targetChunk->world_pos.x) + HALF_CHUNK;
    int blockY = worldBlockY - (int)floor(targetChunk->world_pos.y) + HALF_CHUNK;
    int blockZ = worldBlockZ - (int)floor(targetChunk->world_pos.z) + HALF_CHUNK;

    return (Vector3) {0};
}


Vector3 RayCastTargetBlock(Player* player, ChunkTable* chunkTable) {
    float maxDistance = 8.0f;
    float stepSize = 0.05f; //how fine (?) to step through space, how frequent we are checking if we hit
    
    Vector3 rayDir = Vector3Normalize(Vector3Subtract(player->camera.target, player->camera.position));
    Vector3 rayPos = Vector3Add(player->camera.position, Vector3Scale(rayDir, 0.1f));

    for (float t = 0; t  < maxDistance; t += stepSize) {
        Vector3 pos = Vector3Add(rayPos, Vector3Scale(rayDir, t));
        //convert ray to block coords in world space
        int worldBlockX = (int)floor(pos.x);
        int worldBlockY = (int)floor(pos.y);
        int worldBlockZ = (int)floor(pos.z);
        //figure out which chunk this block is in
        int chunkX = (int)floor((pos.x + HALF_CHUNK) / CHUNK_SIZE);
        int chunkY = (int)floor((pos.y + HALF_CHUNK) / CHUNK_SIZE);
        int chunkZ = (int)floor((pos.z + HALF_CHUNK) / CHUNK_SIZE);
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
                return (Vector3) { 
                    worldBlockX, 
                    worldBlockY, 
                    worldBlockZ };
            }
        }
        player->prevTargetBlockPos = pos;
    }
    return (Vector3) {-1000,-1000,-1000}; // return invalid (?) pos if nothing hits
}

Vector3 RayCastRelativeTargetBlock(Camera* camera, Chunk* targetChunk) {
    float maxDistance = 8.0f;
    float stepSize = 0.05f; //how fine (?) to step through space, how frequent we are checking if we hit
    
    Vector3 rayDir = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    Vector3 rayPos = Vector3Add(camera->position, Vector3Scale(rayDir, 0.1f));

    for (float t = 0; t  < maxDistance; t += stepSize) {
        Vector3 pos = Vector3Add(rayPos, Vector3Scale(rayDir, t));
        //convert ray to block coords in world space
        int worldBlockX = (int)floor(pos.x);
        int worldBlockY = (int)floor(pos.y);
        int worldBlockZ = (int)floor(pos.z);

        if(!targetChunk) continue; //skip if that chunk doesn't exist yet
        //convert world coords to chunk-relative coords
        int blockX = worldBlockX - (int)floor(targetChunk->world_pos.x) + HALF_CHUNK;
        int blockY = worldBlockY - (int)floor(targetChunk->world_pos.y) + HALF_CHUNK;
        int blockZ = worldBlockZ - (int)floor(targetChunk->world_pos.z) + HALF_CHUNK;

        if(blockX >= 0 && blockX < CHUNK_SIZE && 
            blockY >= 0 && blockY < CHUNK_SIZE && 
            blockZ >= 0 && blockZ < CHUNK_SIZE) {
            
            // if(!IsBlockAir(targetChunk, blockX, blockY, blockZ)) {
            //     return (Vector3) { 
            //         blockX, 
            //         blockY, 
            //         blockZ };
            // }
        }
    }
    return (Vector3) {-1000,-1000,-1000}; // return invalid (?) pos if nothing hits
}

void DrawCubeTextureCulled(Texture2D texture, Vector3 position, float width, float height, float length, Color color, uint8_t visibleFaces)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);
    rlColor4ub(color.r, color.g, color.b, color.a);

    // Front Face (positive Z)
    if (visibleFaces & FACE_FRONT) {
        rlNormal3f(0.0f, 0.0f, 1.0f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);
    }

    // Back Face (negative Z)
    if (visibleFaces & FACE_BACK) {
        rlNormal3f(0.0f, 0.0f, -1.0f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);
    }

    // Top Face (positive Y)
    if (visibleFaces & FACE_TOP) {
        rlNormal3f(0.0f, 1.0f, 0.0f);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);
    }

    // Bottom Face (negative Y)
    if (visibleFaces & FACE_BOTTOM) {
        rlNormal3f(0.0f, -1.0f, 0.0f);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);
    }

    // Right Face (positive X)
    if (visibleFaces & FACE_RIGHT) {
        rlNormal3f(1.0f, 0.0f, 0.0f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);
    }

    // Left Face (negative X)
    if (visibleFaces & FACE_LEFT) {
        rlNormal3f(-1.0f, 0.0f, 0.0f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);
    }

    rlEnd();
    rlSetTexture(0);
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
