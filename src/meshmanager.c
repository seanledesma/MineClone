#include "include.h"

Texture2D grassTex;
Texture2D dirtTex;
Texture2D stoneTex;

int facesDrawn = 0;

const float FRONT_VERTICES[] = {
    0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f
};

const float FRONT_NORMALS[] = {
    0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f
};

const float FRONT_UVS[] = {
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
};

const float BACK_VERTICES[] = {
    0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f
};

const float BACK_NORMALS[] = {
    0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f
};

const float BACK_UVS[] = {
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

const float TOP_VERTICES[] = {
    0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f
};

const float TOP_NORMALS[] = {
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f
};

const float TOP_UVS[] = {
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

const float BOTTOM_VERTICES[] = {
    0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f
};

const float BOTTOM_NORMALS[] = {
    0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f
};

const float BOTTOM_UVS[] = {
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
};

const float RIGHT_VERTICES[] = {
    1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f
};

const float RIGHT_NORMALS[] = {
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f
};

const float RIGHT_UVS[] = {
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f
};

const float LEFT_VERTICES[] = {
    0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f
};

const float LEFT_NORMALS[] = {
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f
};

const float LEFT_UVS[] = {
    0.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
    0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f
};


static void CleanupJobResult(JobResult* result) {
    if (!result) return;
    if (result->grass_vertices) free(result->grass_vertices);
    if (result->grass_normals) free(result->grass_normals);
    if (result->grass_uvs) free(result->grass_uvs);
    if (result->grass_colors) free(result->grass_colors);

    if (result->dirt_vertices) free(result->dirt_vertices);
    if (result->dirt_normals) free(result->dirt_normals);
    if (result->dirt_uvs) free(result->dirt_uvs);
    if (result->dirt_colors) free(result->dirt_colors);

    if (result->stone_vertices) free(result->stone_vertices);
    if (result->stone_normals) free(result->stone_normals);
    if (result->stone_uvs) free(result->stone_uvs);
    if (result->stone_colors) free(result->stone_colors);
    
    free(result);
}

static int GetAOShade(ChunkTable* table, Chunk* chunk, int x, int y, int z, int dx1, int dy1, int dz1, int dx2, int dy2, int dz2) {
    int d1_is_solid = !IsBlockAir(table, chunk, x + dx1, y + dy1, z + dz1);
    int d2_is_solid = !IsBlockAir(table, chunk, x + dx2, y + dy2, z + dz2);
    int d_corner_is_solid = !IsBlockAir(table, chunk, x + dx1 + dx2, y + dy1 + dy2, z + dz1 + dz2);

    if (d1_is_solid && d2_is_solid) {
        return 3;
    }
    
    return d1_is_solid + d2_is_solid + d_corner_is_solid;
}

static unsigned char AO_TO_COLOR(int ao_factor) {
    switch (ao_factor) {
        case 0: return 255; 
        case 1: return 230; 
        case 2: return 200; 
        case 3: return 180; 
        default: return 255;
    }
}

void AddFaceData(float* verts, float* uvs, float* normals, unsigned char* colors, int* vert_count, Vector3 block_pos, int face_id, const unsigned char face_colors[24]) {    
    const float *base_verts;
    const float *base_normals;
    const float *base_uvs;

    switch (face_id) {
        case FACE_FRONT: base_verts = FRONT_VERTICES; base_normals = FRONT_NORMALS; base_uvs = FRONT_UVS; break;
        case FACE_BACK: base_verts = BACK_VERTICES; base_normals = BACK_NORMALS; base_uvs = BACK_UVS; break;
        case FACE_TOP: base_verts = TOP_VERTICES; base_normals = TOP_NORMALS; base_uvs = TOP_UVS; break;
        case FACE_BOTTOM: base_verts = BOTTOM_VERTICES; base_normals = BOTTOM_NORMALS; base_uvs = BOTTOM_UVS; break;
        case FACE_LEFT: base_verts = LEFT_VERTICES; base_normals = LEFT_NORMALS; base_uvs = LEFT_UVS; break;
        case FACE_RIGHT: base_verts = RIGHT_VERTICES; base_normals = RIGHT_NORMALS; base_uvs = RIGHT_UVS; break;
        default: return;
    }

    const int NUM_FLOATS = 6 * 3;
    const int NUM_COLOR_BYTES = 6 * 4;
    const int vert_start_index = *vert_count * 3; 
    const int color_start_index = *vert_count * 4;

    for (int i = 0; i < NUM_FLOATS; i += 3) {
        verts[vert_start_index + i]     = base_verts[i] + block_pos.x;
        verts[vert_start_index + i + 1] = base_verts[i + 1] + block_pos.y;
        verts[vert_start_index + i + 2] = base_verts[i + 2] + block_pos.z;
    }

    memcpy(&normals[vert_start_index], base_normals, 6 * 3 * sizeof(float));
    memcpy(&uvs[*vert_count * 2], base_uvs, 6 * 2 * sizeof(float));
    memcpy(&colors[color_start_index], face_colors, NUM_COLOR_BYTES);

    *vert_count += 6;
}

void FinalizeAndUploadMesh(Mesh* mesh, Model* model, 
                           float* temp_verts, float* temp_normals, float* temp_uvs, 
                           unsigned char* temp_colors,
                           int vert_count, Texture2D texture) {
    
    // FIX: Initialize mesh to zero first
    *mesh = (Mesh){0};
    
    if (vert_count == 0) {
        free(temp_verts);
        free(temp_normals);
        free(temp_uvs);
        free(temp_colors);
        // FIX: Don't create model from empty mesh
        *model = (Model){0};
        return;
    }

    mesh->vertexCount = vert_count;
    mesh->triangleCount = vert_count / 3;

    // FIX: Directly assign the temp buffers to the mesh instead of copying
    // This avoids double-allocation and the mesh will own these buffers
    mesh->vertices = temp_verts;
    mesh->normals = temp_normals;
    mesh->texcoords = temp_uvs;
    mesh->colors = temp_colors;

    UploadMesh(mesh, false);

    *model = LoadModelFromMesh(*mesh); 
    
    model->materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;
    model->materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;

    // FIX: DON'T set mesh pointers to NULL or free them here
    // The mesh now owns these buffers, and UnloadMesh will free them properly
}

// Helper function to safely unload chunk meshes
void UnloadChunkMeshes(Chunk* chunk) {
    if (!chunk) return;
    
    // Unload models first (they reference the meshes)
    if (chunk->grassModel.meshCount > 0) {
        UnloadModel(chunk->grassModel);
        chunk->grassModel = (Model){0};
    }
    if (chunk->dirtModel.meshCount > 0) {
        UnloadModel(chunk->dirtModel);
        chunk->dirtModel = (Model){0};
    }
    if (chunk->stoneModel.meshCount > 0) {
        UnloadModel(chunk->stoneModel);
        chunk->stoneModel = (Model){0};
    }
    
    // Reset mesh structs (UnloadModel already freed the mesh data)
    chunk->grassMesh = (Mesh){0};
    chunk->dirtMesh = (Mesh){0};
    chunk->stoneMesh = (Mesh){0};
}

JobResult* GenerateRawMesh(ChunkTable* chunkTable, Chunk* chunk) {
    
    JobResult* result = (JobResult*)malloc(sizeof(JobResult));
    if (result == NULL) return NULL;
    
    memset(result, 0, sizeof(JobResult)); // FIX: Zero-initialize the result
    
    result->chunk = chunk;
    result->table = chunkTable;
    
    int grass_vert_count = 0;
    int dirt_vert_count = 0;
    int stone_vert_count = 0;
    
    const int MAX_VERTICES = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36;
    
    result->grass_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->grass_vertices == NULL) { CleanupJobResult(result); return NULL; }
    result->grass_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->grass_normals == NULL) { CleanupJobResult(result); return NULL; }
    result->grass_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (result->grass_uvs == NULL) { CleanupJobResult(result); return NULL; }
    result->grass_colors = (unsigned char *)malloc(MAX_VERTICES * 4 * sizeof(unsigned char));
    if (result->grass_colors == NULL) { CleanupJobResult(result); return NULL; }
    
    result->dirt_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->dirt_vertices == NULL) { CleanupJobResult(result); return NULL; } 
    result->dirt_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->dirt_normals == NULL) { CleanupJobResult(result); return NULL; }
    result->dirt_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (result->dirt_uvs == NULL) { CleanupJobResult(result); return NULL; }
    result->dirt_colors = (unsigned char *)malloc(MAX_VERTICES * 4 * sizeof(unsigned char));
    if (result->dirt_colors == NULL) { CleanupJobResult(result); return NULL; } 

    result->stone_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->stone_vertices == NULL) { CleanupJobResult(result); return NULL; } 
    result->stone_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->stone_normals == NULL) { CleanupJobResult(result); return NULL; }
    result->stone_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (result->stone_uvs == NULL) { CleanupJobResult(result); return NULL; }
    result->stone_colors = (unsigned char *)malloc(MAX_VERTICES * 4 * sizeof(unsigned char));
    if (result->stone_colors == NULL) { CleanupJobResult(result); return NULL; }

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                
                if (chunk->blocks[x][y][z].blockType == BLOCK_AIR) {
                    continue;
                }
                uint8_t visibleFaces = 0;

                if (IsBlockAir(chunkTable, chunk, x, y, z+1)) visibleFaces |= FACE_FRONT;
                if (IsBlockAir(chunkTable, chunk, x, y, z-1)) visibleFaces |= FACE_BACK;
                if (IsBlockAir(chunkTable, chunk, x, y+1, z)) visibleFaces |= FACE_TOP;
                if (IsBlockAir(chunkTable, chunk, x, y-1, z)) visibleFaces |= FACE_BOTTOM;
                if (IsBlockAir(chunkTable, chunk, x+1, y, z)) visibleFaces |= FACE_RIGHT;
                if (IsBlockAir(chunkTable, chunk, x-1, y, z)) visibleFaces |= FACE_LEFT;
                

                if (visibleFaces > 0) {
                    // FIX: Don't update facesDrawn here - it's not thread safe
                    // and this is called from worker threads
                    
                    Vector3 block_pos = { 
                        (float)x - HALF_CHUNK, 
                        (float)y - HALF_CHUNK, 
                        (float)z - HALF_CHUNK 
                    };
                    
                    float* current_vertices = NULL;
                    float* current_normals = NULL;
                    float* current_uvs = NULL;
                    unsigned char* current_colors = NULL;
                    int* current_count_ptr = NULL;

                    unsigned char face_colors[24]; 
                    int ao[4]; 
                    int ao_factors[6];

                    if (chunk->blocks[x][y][z].blockType == BLOCK_GRASS) {
                        current_vertices = result->grass_vertices;
                        current_normals = result->grass_normals;
                        current_uvs = result->grass_uvs;
                        current_colors = result->grass_colors;
                        current_count_ptr = &grass_vert_count;

                    } else if (chunk->blocks[x][y][z].blockType == BLOCK_DIRT) {
                        current_vertices = result->dirt_vertices;
                        current_normals = result->dirt_normals;
                        current_uvs = result->dirt_uvs;
                        current_colors = result->dirt_colors;
                        current_count_ptr = &dirt_vert_count;

                    } else if (chunk->blocks[x][y][z].blockType == BLOCK_STONE) {
                        current_vertices = result->stone_vertices;
                        current_normals = result->stone_normals;
                        current_uvs = result->stone_uvs;
                        current_colors = result->stone_colors;
                        current_count_ptr = &stone_vert_count;

                    } else {
                        continue; 
                    }

                    if (visibleFaces & FACE_FRONT) {
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, -1, 0); 
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, -1, 0);
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, +1, 0); 
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, +1, 0); 
                        
                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 

                        if (d1_sum > d2_sum) {
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            ao_factors[0] = ao[0]; ao_factors[1] = ao[3]; ao_factors[2] = ao[2]; 
                            ao_factors[3] = ao[0]; ao_factors[4] = ao[1]; ao_factors[5] = ao[3]; 
                        }

                        for (int i = 0; i < 6; i++) {
                            unsigned char color = AO_TO_COLOR(ao_factors[i]);
                            face_colors[i * 4 + 0] = color; face_colors[i * 4 + 1] = color; 
                            face_colors[i * 4 + 2] = color; face_colors[i * 4 + 3] = 255;   
                        }
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_FRONT, face_colors);
                    }
                    
                    if (visibleFaces & FACE_BACK) {
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, -1, 0); 
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, -1, 0);
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, +1, 0); 
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, +1, 0); 

                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 
                        
                        if (d1_sum > d2_sum) {
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            ao_factors[0] = ao[0]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[0]; ao_factors[4] = ao[3]; ao_factors[5] = ao[1]; 
                        }

                        for (int i = 0; i < 6; i++) {
                            unsigned char color = AO_TO_COLOR(ao_factors[i]);
                            face_colors[i * 4 + 0] = color; face_colors[i * 4 + 1] = color; 
                            face_colors[i * 4 + 2] = color; face_colors[i * 4 + 3] = 255;   
                        }
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_BACK, face_colors);
                    }

                    if (visibleFaces & FACE_RIGHT) {
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, 0, -1, 0, 0, 0, -1); 
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, 0, -1, 0, 0, 0, +1);
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, 0, +1, 0, 0, 0, -1); 
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, 0, +1, 0, 0, 0, +1); 

                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 

                        if (d1_sum > d2_sum) {
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            ao_factors[0] = ao[0]; ao_factors[1] = ao[3]; ao_factors[2] = ao[1]; 
                            ao_factors[3] = ao[0]; ao_factors[4] = ao[2]; ao_factors[5] = ao[3]; 
                        }

                        for (int i = 0; i < 6; i++) {
                            unsigned char color = AO_TO_COLOR(ao_factors[i]);
                            face_colors[i * 4 + 0] = color; face_colors[i * 4 + 1] = color; 
                            face_colors[i * 4 + 2] = color; face_colors[i * 4 + 3] = 255;   
                        }
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_RIGHT, face_colors);
                    }

                    if (visibleFaces & FACE_LEFT) {
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, 0, -1, 0, 0, 0, -1); 
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, 0, -1, 0, 0, 0, +1);
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, 0, +1, 0, 0, 0, -1); 
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, 0, +1, 0, 0, 0, +1); 

                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 

                        if (d1_sum > d2_sum) {
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            ao_factors[0] = ao[0]; ao_factors[1] = ao[3]; ao_factors[2] = ao[2]; 
                            ao_factors[3] = ao[0]; ao_factors[4] = ao[1]; ao_factors[5] = ao[3]; 
                        }

                        for (int i = 0; i < 6; i++) {
                            unsigned char color = AO_TO_COLOR(ao_factors[i]);
                            face_colors[i * 4 + 0] = color; face_colors[i * 4 + 1] = color; 
                            face_colors[i * 4 + 2] = color; face_colors[i * 4 + 3] = 255;   
                        }
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_LEFT, face_colors);
                    }

                    if (visibleFaces & FACE_TOP) {
                        int c0_ao = GetAOShade(chunkTable, chunk, x, y, z, 0, 0, -1, -1, 0, 0); 
                        int c1_ao = GetAOShade(chunkTable, chunk, x, y, z, 0, 0, -1, +1, 0, 0);
                        int c2_ao = GetAOShade(chunkTable, chunk, x, y, z, 0, 0, +1, -1, 0, 0); 
                        int c3_ao = GetAOShade(chunkTable, chunk, x, y, z, 0, 0, +1, +1, 0, 0);
                        ao[0] = c0_ao; ao[1] = c1_ao; ao[2] = c2_ao; ao[3] = c3_ao; 

                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 
                        
                        if (d1_sum > d2_sum) { 
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            ao_factors[0] = ao[0]; ao_factors[1] = ao[3]; ao_factors[2] = ao[1]; 
                            ao_factors[3] = ao[0]; ao_factors[4] = ao[2]; ao_factors[5] = ao[3]; 
                        }

                        for (int i = 0; i < 6; i++) {
                            unsigned char color = AO_TO_COLOR(ao_factors[i]);
                            face_colors[i * 4 + 0] = color; face_colors[i * 4 + 1] = color; 
                            face_colors[i * 4 + 2] = color; face_colors[i * 4 + 3] = 255;   
                        }
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_TOP, face_colors);
                    }

                    // BOTTOM FACE
                    if (visibleFaces & FACE_BOTTOM) {
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, 0, -1); 
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, 0, +1);
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, 0, -1); 
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, 0, +1); 
                        
                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 
                        
                        if (d1_sum > d2_sum) { 
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            ao_factors[0] = ao[0]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[0]; ao_factors[4] = ao[3]; ao_factors[5] = ao[1]; 
                        }

                        for (int i = 0; i < 6; i++) {
                            unsigned char color = AO_TO_COLOR(ao_factors[i]);
                            face_colors[i * 4 + 0] = color; face_colors[i * 4 + 1] = color; 
                            face_colors[i * 4 + 2] = color; face_colors[i * 4 + 3] = 255;   
                        }
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_BOTTOM, face_colors);
                    }
                }
            }
        }
    }

    result->grass_vert_count = grass_vert_count;
    result->dirt_vert_count = dirt_vert_count;
    result->stone_vert_count = stone_vert_count;

    return result;

}

void InitChunkMesh(ChunkTable* chunkTable, Chunk* chunk) {
    (void)chunkTable;
    (void)chunk;
    TraceLog(LOG_WARNING, "InitChunkMesh should not be called directly. Use threading pipeline.");
}
// FIXED RegenerateChunk function - replace the one in meshmanager.c

void RegenerateChunk(ChunkTable* chunkTable, Chunk* chunk) {
    // FIX: If already regenerating, mark for another remesh instead of skipping
    if (chunk->state == CHUNK_STATE_REGENERATING) {
        chunk->needsRemesh = true;  // Will be processed after current job completes
        return;
    }
    
    // Mark chunk as needing regeneration (but still drawable with old mesh)
    chunk->needsRemesh = false;
    chunk->state = CHUNK_STATE_REGENERATING;

    // Submit a job using the proper deduplication system
    int cx = (int)chunk->table_pos.x;
    int cy = (int)chunk->table_pos.y;
    int cz = (int)chunk->table_pos.z;
    
    mtx_lock(&queue_mutex);
    
    // Use the pending job tracking to prevent duplicates
    if (!is_job_pending(cx, cy, cz)) {
        Job* job = create_job(chunkTable, cx, cy, cz);
        if (job) {
            add_pending_job(cx, cy, cz);
            job_push(job);
            cnd_signal(&job_available);
        }
    }
    
    mtx_unlock(&queue_mutex);
}

void UpdateNeighborChunkMesh(ChunkTable* table, Chunk* currentChunk, int bx, int by, int bz) {
    int cx = (int)currentChunk->table_pos.x;
    int cy = (int)currentChunk->table_pos.y;
    int cz = (int)currentChunk->table_pos.z;
    
    // Helper macro to check if chunk can be regenerated
    #define CAN_REGENERATE(c) ((c) && ((c)->state == CHUNK_STATE_READY || (c)->state == CHUNK_STATE_REGENERATING))
    
    if (bx == 0) {
        Chunk* neighbor = get_chunk(table, cx - 1, cy, cz);
        if (CAN_REGENERATE(neighbor)) RegenerateChunk(table, neighbor);
    }
    else if (bx == CHUNK_SIZE - 1) {
        Chunk* neighbor = get_chunk(table, cx + 1, cy, cz);
        if (CAN_REGENERATE(neighbor)) RegenerateChunk(table, neighbor);
    }

    if (by == 0) {
        Chunk* neighbor = get_chunk(table, cx, cy - 1, cz);
        if (CAN_REGENERATE(neighbor)) RegenerateChunk(table, neighbor);
    }
    else if (by == CHUNK_SIZE - 1) {
        Chunk* neighbor = get_chunk(table, cx, cy + 1, cz);
        if (CAN_REGENERATE(neighbor)) RegenerateChunk(table, neighbor);
    }

    if (bz == 0) {
        Chunk* neighbor = get_chunk(table, cx, cy, cz - 1);
        if (CAN_REGENERATE(neighbor)) RegenerateChunk(table, neighbor);
    }
    else if (bz == CHUNK_SIZE - 1) {
        Chunk* neighbor = get_chunk(table, cx, cy, cz + 1);
        if (CAN_REGENERATE(neighbor)) RegenerateChunk(table, neighbor);
    }
    
    #undef CAN_REGENERATE
}