#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "include.h"
Texture2D grassTex;
Texture2D dirtTex;
Texture2D stoneTex;

void AddFaceData(float* verts, float* uvs, float* normals, unsigned char* colors, int* vert_count, Vector3 block_pos, int face_id, const unsigned char face_colors[24]);  
void FinalizeAndUploadMesh(Mesh* mesh, Model* model, 
                           float* temp_verts, float* temp_normals, float* temp_uvs, 
                           unsigned char* temp_colors,
                           int vert_count, Texture2D texture);
void InitChunkMesh(ChunkTable* chunkTable, Chunk* chunk);
void RegenerateChunk(ChunkTable* chunkTable, Chunk* chunk);
void UpdateNeighborChunkMesh(ChunkTable* table, Chunk* currentChunk, int bx, int by, int bz);

Mesh GenMeshChunk();

#endif
