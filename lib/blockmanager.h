#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include "include.h"

bool IsBlockAir(Chunk* chunk, int x, int y, int z);

BlockType DecideBlockType(Chunk* new_chunk, int absolute_x, int absolute_y, int absolute_z);

/*converts world block coords to chunk-relative index*/
Vector3 ConvWorldCoords(ChunkTable* chunkTable, Vector3 pos);

Vector3 RayCastTargetBlock(Player* player, ChunkTable* chunkTable);

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);

#endif
