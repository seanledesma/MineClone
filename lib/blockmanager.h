#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include "include.h"

bool IsBlockAir(Chunk* chunk, int x, int y, int z);

BlockType DecideBlockType(Chunk* new_chunk, int absolute_x, int absolute_y, int absolute_z);

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);

#endif
