#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "include.h"
extern Texture2D grassTex;
extern Texture2D dirtTex;
extern Texture2D stoneTex;

extern int facesDrawn;

static int GetAOShade(ChunkTable* table, Chunk* chunk, int x, int y, int z, int dx1, int dy1, int dz1, int dx2, int dy2, int dz2);
void AddFaceData(float* verts, float* uvs, float* normals, unsigned char* colors, int* vert_count, Vector3 block_pos, int face_id, const unsigned char face_colors[24]);  
void FinalizeAndUploadMesh(Mesh* mesh, Model* model, 
                           float* temp_verts, float* temp_normals, float* temp_uvs, 
                           unsigned char* temp_colors,
                           int vert_count, Texture2D texture);
JobResult* GenerateRawMesh(ChunkTable* chunkTable, Chunk* chunk);
void InitChunkMesh(ChunkTable* chunkTable, Chunk* chunk);
void RegenerateChunk(ChunkTable* chunkTable, Chunk* chunk);
void UpdateNeighborChunkMesh(ChunkTable* table, Chunk* currentChunk, int bx, int by, int bz);


#endif
