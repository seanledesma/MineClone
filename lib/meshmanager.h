#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "include.h"
Texture2D grassTex;
Texture2D dirtTex;
Texture2D stoneTex;

void AddFaceData(float* verts, float* uvs, float* normals, int* vert_count, Vector3 block_pos, int face_id);
void FinalizeAndUploadMesh(Mesh* mesh, Model* model, float* temp_verts, float* temp_normals, float* temp_uvs, int vert_count, Texture2D texture);
void InitChunkMesh(ChunkTable* chunkTable, Chunk* chunk);

Mesh GenMeshChunk();

#endif
