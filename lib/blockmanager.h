#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include "include.h"

typedef enum {
    FACE_FRONT  = (1 << 0),  // 0x01
    FACE_BACK   = (1 << 1),  // 0x02
    FACE_TOP    = (1 << 2),  // 0x04
    FACE_BOTTOM = (1 << 3),  // 0x08
    FACE_RIGHT  = (1 << 4),  // 0x10
    FACE_LEFT   = (1 << 5)   // 0x20
} FaceFlags;

bool IsBlockAir(ChunkTable* chunkTable, Chunk* chunk, int x, int y, int z);

BlockType DecideBlockType(Chunk* new_chunk, int absolute_x, int absolute_y, int absolute_z);

/*converts world block coords to chunk-relative index*/
Vector3 ConvWorldCoords(ChunkTable* chunkTable, Vector3 pos);

Vector3 RayCastTargetBlock(Player* player, ChunkTable* chunkTable);

void DrawCubeTextureCulled(Texture2D texture, Vector3 position, float width, float height, float length, Color color, uint8_t visibleFaces);

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);

#endif
