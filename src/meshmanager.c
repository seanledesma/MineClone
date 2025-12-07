#include "include.h"

// FRONT Face (Z+ direction)
// Vertices (6 vertices * 3 floats = 18 total)
const float FRONT_VERTICES[] = {
    0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f, // Tri 1: BL, TR, TL
    0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f  // Tri 2: BL, BR, TR
};

// Normals (6 vertices * 3 floats = 18 total)
const float FRONT_NORMALS[] = {
    0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f
};

// UVs (6 vertices * 2 floats = 12 total)
const float FRONT_UVS[] = {
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // BL(0,1), TR(1,0), TL(0,0)
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f  // BL(0,1), BR(1,1), TR(1,0)
};

// BACK Face (Z- direction)
const float BACK_VERTICES[] = {
    0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f, // Tri 1: BL, TL, TR
    0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f  // Tri 2: BL, TR, BR
};

const float BACK_NORMALS[] = {
    0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f
};

const float BACK_UVS[] = {
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // BL(0,1), TL(0,0), TR(1,0)
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f  // BL(0,1), TR(1,0), BR(1,1)
};

// TOP Face (Y+ direction)
const float TOP_VERTICES[] = {
    0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, // Tri 1: BL, FR, BR
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f  // Tri 2: BL, FL, FR
};

const float TOP_NORMALS[] = {
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f
};

const float TOP_UVS[] = {
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

// BOTTOM Face (Y- direction)
const float BOTTOM_VERTICES[] = {
    0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, // Tri 1: BL, BR, FR
    0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f  // Tri 2: BL, FR, FL
};

const float BOTTOM_NORMALS[] = {
    0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f
};

const float BOTTOM_UVS[] = {
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
};

// RIGHT Face (X+ direction)
const float RIGHT_VERTICES[] = {
    1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, // Tri 1: BD, TR, BR
    1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f  // Tri 2: BD, TD, TR
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
    // Tri 1: BB, TF, TB (CW Order)
    0.0f, 0.0f, 0.0f,  // V1: BB
    0.0f, 1.0f, 1.0f,  // V2: TF  <- SWAPPED
    0.0f, 1.0f, 0.0f,  // V3: TB  <- SWAPPED

    // Tri 2: BB, BF, TF (Your original broken winding)
    0.0f, 0.0f, 0.0f,  // V4: BB
    0.0f, 0.0f, 1.0f,  // V5: BF
    0.0f, 1.0f, 1.0f   // V6: TF
};

const float LEFT_NORMALS[] = {
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f
};

const float LEFT_UVS[] = {
    // Tri 1: V1(BB), V2(TF), V3(TB)
    0.0f, 1.0f, // V1: BB
    1.0f, 0.0f, // V2: TF  <- SWAPPED
    0.0f, 0.0f, // V3: TB  <- SWAPPED

    // Tri 2: V4(BB), V5(BF), V6(TF)
    0.0f, 1.0f, // V4: BB
    1.0f, 1.0f, // V5: BF
    1.0f, 0.0f  // V6: TF
};

void AddFaceData(float* verts, float* uvs, float* normals, unsigned char* colors, int* vert_count, Vector3 block_pos, int face_id, const unsigned char face_colors[24]) {    
    const float *base_verts;
    const float *base_normals;
    const float *base_uvs;

    switch (face_id) {
        case FACE_FRONT:
            base_verts = FRONT_VERTICES;
            base_normals = FRONT_NORMALS;
            base_uvs = FRONT_UVS;
            break;
        case FACE_BACK:
            base_verts = BACK_VERTICES; 
            base_normals = BACK_NORMALS; 
            base_uvs = BACK_UVS;
            break;
        case FACE_TOP:
            base_verts = TOP_VERTICES; 
            base_normals = TOP_NORMALS; 
            base_uvs = TOP_UVS;
            break;
        case FACE_BOTTOM:
            base_verts = BOTTOM_VERTICES; 
            base_normals = BOTTOM_NORMALS; 
            base_uvs = BOTTOM_UVS;
            break;
        case FACE_LEFT:
            base_verts = LEFT_VERTICES; 
            base_normals = LEFT_NORMALS; 
            base_uvs = LEFT_UVS;
            break;
        case FACE_RIGHT:
            base_verts = RIGHT_VERTICES; 
            base_normals = RIGHT_NORMALS; 
            base_uvs = RIGHT_UVS;
            break;
        default: return;
    }

    // The number of floats to process (6 vertices * 3 components)
    const int NUM_FLOATS = 6 * 3;
    const int NUM_COLOR_BYTES = 6 * 4;
    const int vert_start_index = *vert_count * 3; 
    const int color_start_index = *vert_count * 4;

    // LOOP 1: Vertices (6 * 3 = 18 floats)
    for (int i = 0; i < NUM_FLOATS; i += 3)
    {
        // Add the block's position (X, Y, Z) to the base vertex coordinates
        verts[vert_start_index + i]     = base_verts[i] + block_pos.x;     // X + block_pos.x
        verts[vert_start_index + i + 1] = base_verts[i + 1] + block_pos.y; // Y + block_pos.y
        verts[vert_start_index + i + 2] = base_verts[i + 2] + block_pos.z; // Z + block_pos.z
    }

    // LOOP 2: Normals and UVs
    // Normals (6 * 3 = 18 floats) and UVs (6 * 2 = 12 floats) are simply copied
    // (A more advanced implementation would transform these for different faces)
    memcpy(&normals[vert_start_index], base_normals, 6 * 3 * sizeof(float));
    memcpy(&uvs[*vert_count * 2], base_uvs, 6 * 2 * sizeof(float));
    memcpy(&colors[color_start_index], face_colors, NUM_COLOR_BYTES * sizeof(unsigned char));

    // Update the vertex count for the next call
    *vert_count += 6;
}

// void FinalizeAndUploadMesh(Mesh* mesh, Model* model, float* temp_verts, float* temp_normals, float* temp_uvs, int vert_count, Texture2D texture) {
//     if (vert_count == 0) {
//         *model = LoadModelFromMesh(*mesh);
//         return;
//     }

//     // 1. Set Mesh properties
//     mesh->vertexCount = vert_count;
//     mesh->triangleCount = vert_count / 3;

//     // 2. Allocate memory for Raylib's Mesh (MUST be allocated via malloc/calloc)
//     mesh->vertices = (float *)malloc(vert_count * 3 * sizeof(float));
//     mesh->normals = (float *)malloc(vert_count * 3 * sizeof(float));
//     mesh->texcoords = (float *)malloc(vert_count * 2 * sizeof(float)); // Raylib uses 'texcoords'

//     // 3. Copy data from the temporary arrays
//     memcpy(mesh->vertices, temp_verts, vert_count * 3 * sizeof(float));
//     memcpy(mesh->normals, temp_normals, vert_count * 3 * sizeof(float));
//     memcpy(mesh->texcoords, temp_uvs, vert_count * 2 * sizeof(float));

//     // 4. Upload to GPU
//     UploadMesh(mesh, false);

//     // 5. Create the Model
//     *model = LoadModelFromMesh(*mesh); 
    
//     // IMPORTANT: Raylib expects the Mesh data to remain allocated by you (step 2)
//     // after LoadModelFromMesh is called. Do NOT free mesh->vertices, etc., here.

//     // 6. Assign the Texture to the Model's Material
//     // The texture map is stored in model.materials[0].maps[MAP_ALBEDO].texture
//     // MAP_ALBEDO (or MAP_DIFFUSE) is the standard color texture.
//     model->materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;
// }

// Function Signature in meshmanager.h should be updated to:
// void FinalizeAndUploadMesh(Mesh* mesh, Model* model, float* temp_verts, float* temp_normals, float* temp_uvs, unsigned char* temp_colors, int vert_count, Texture2D texture);

void FinalizeAndUploadMesh(Mesh* mesh, Model* model, 
                           float* temp_verts, float* temp_normals, float* temp_uvs, 
                           unsigned char* temp_colors, // <--- NEW ARGUMENT
                           int vert_count, Texture2D texture) {
    
    // Handle the case where no faces were generated
    if (vert_count == 0) {
        // We must still free the temporary arrays passed to this function!
        free(temp_verts);
        free(temp_normals);
        free(temp_uvs);
        free(temp_colors); // <--- NEW: Free unused temp color buffer
        
        // Initialize the model structure to prevent crashes when DrawModel() is called on it
        *model = LoadModelFromMesh(*mesh); 
        return;
    }

    // 1. Set Mesh properties
    mesh->vertexCount = vert_count;
    mesh->triangleCount = vert_count / 3;

    // 2. Allocate memory for Raylib's Mesh (MUST be allocated via malloc/calloc)
    
    mesh->vertices = (float *)malloc(vert_count * 3 * sizeof(float));
    if (mesh->vertices == NULL) return; 

    mesh->normals = (float *)malloc(vert_count * 3 * sizeof(float));
    if (mesh->normals == NULL) { 
        free(mesh->vertices);
        return;
    }

    mesh->texcoords = (float *)malloc(vert_count * 2 * sizeof(float)); 
    if (mesh->texcoords == NULL) { 
        free(mesh->vertices);
        free(mesh->normals);
        return;
    }
    
    // NEW: Allocate space for vertex colors (4 components: R, G, B, A)
    mesh->colors = (unsigned char *)malloc(vert_count * 4 * sizeof(unsigned char));
    if (mesh->colors == NULL) { 
        free(mesh->vertices);
        free(mesh->normals);
        free(mesh->texcoords);
        return;
    }

    // 3. Copy data from the temporary arrays
    memcpy(mesh->vertices, temp_verts, vert_count * 3 * sizeof(float));
    memcpy(mesh->normals, temp_normals, vert_count * 3 * sizeof(float));
    memcpy(mesh->texcoords, temp_uvs, vert_count * 2 * sizeof(float));
    
    unsigned char face_colors[24];
    // NEW: Copy color data
   //memcpy(mesh->colors, temp_colors, vert_count * 4 * sizeof(unsigned char));
    unsigned char gray_value = 100;
    for (int i = 0; i < 24; i += 4) {
        face_colors[i] = gray_value;       // R
        face_colors[i+1] = gray_value;     // G
        face_colors[i+2] = gray_value;     // B
        face_colors[i+3] = 255;            // A
    }


    // 4. Upload to GPU
    UploadMesh(mesh, false);

    // 5. Create the Model
    *model = LoadModelFromMesh(*mesh); 
    
    // 6. Assign the Texture
    model->materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;

    // NEW FIX: Explicitly set the material's diffuse color to WHITE.
    // If the vertex color exists, many default shaders will automatically
    // perform multiplication: FinalColor = VertexColor * TextureColor * MaterialColor.
    // Setting MaterialColor to WHITE (255,255,255,255) ensures only VertexColor darkens the Texture.
    model->materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;

    // !!! CRITICAL FIXES (Cleanup of Pointers) !!!
    // The data was freed by UploadMesh(..., false). Set pointers to NULL
    // to prevent UnloadMesh from trying to free them again later.
    mesh->vertices = NULL;
    mesh->normals = NULL;
    mesh->texcoords = NULL;
    mesh->colors = NULL; // <--- NEW: Clear the color pointer too

    // NOTE: Free the temporary buffers passed to this function
    free(temp_verts);
    free(temp_normals);
    free(temp_uvs);
    free(temp_colors); // <--- NEW: Free the temporary color buffer
}

void InitChunkMesh(ChunkTable* chunkTable, Chunk* chunk) {

    // track how many vertices have been added to each array
    int grass_vert_count = 0;
    int dirt_vert_count = 0;
    int stone_vert_count = 0;

    // max number of possilbe faces (6 faces and 4 possible vertices)
    const int MAX_VERTICES = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6;
    
    // --- GRASS ALLOCATIONS ---
    float* temp_grass_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_grass_vertices == NULL) return;
    float* temp_grass_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_grass_normals == NULL) { free(temp_grass_vertices); return; }
    float* temp_grass_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (temp_grass_uvs == NULL) { free(temp_grass_vertices); free(temp_grass_normals); return; }
    
    // NEW: Allocate temporary buffer for vertex colors (4 components: R, G, B, A)
    unsigned char* temp_grass_colors = (unsigned char *)malloc(MAX_VERTICES * 4 * sizeof(unsigned char));
    if (temp_grass_colors == NULL) { 
        free(temp_grass_vertices); free(temp_grass_normals); free(temp_grass_uvs); 
        return; 
    }

    // --- DIRT ALLOCATIONS ---
    float* temp_dirt_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_dirt_vertices == NULL) { free(temp_grass_colors); return; } 
    float* temp_dirt_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_dirt_normals == NULL) { free(temp_dirt_vertices); free(temp_grass_colors); return; }
    float* temp_dirt_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (temp_dirt_uvs == NULL) { free(temp_dirt_vertices); free(temp_dirt_normals); free(temp_grass_colors); return; }

    // NEW: Allocate temporary buffer for dirt colors
    unsigned char* temp_dirt_colors = (unsigned char *)malloc(MAX_VERTICES * 4 * sizeof(unsigned char));
    if (temp_dirt_colors == NULL) { 
        free(temp_dirt_vertices); free(temp_dirt_normals); free(temp_dirt_uvs);
        free(temp_grass_colors); 
        return; 
    }

    // --- STONE ALLOCATIONS ---
    float* temp_stone_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_stone_vertices == NULL) { free(temp_dirt_colors); free(temp_grass_colors); return; } 
    float* temp_stone_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_stone_normals == NULL) { free(temp_stone_vertices); free(temp_dirt_colors); free(temp_grass_colors); return; }
    float* temp_stone_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (temp_stone_uvs == NULL) { free(temp_stone_vertices); free(temp_stone_normals); free(temp_dirt_colors); free(temp_grass_colors); return; }

    // NEW: Allocate temporary buffer for stone colors
    unsigned char* temp_stone_colors = (unsigned char *)malloc(MAX_VERTICES * 4 * sizeof(unsigned char));
    if (temp_stone_colors == NULL) { 
        free(temp_stone_vertices); free(temp_stone_normals); free(temp_stone_uvs);
        free(temp_dirt_colors); free(temp_grass_colors);
        return;
    }


    // loop through all faces in chunk, if their neighbor is air, proceed to next step
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                //skip air blocks entirely 
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
                    
                    Vector3 block_pos = { 
                        (float)x - HALF_CHUNK, 
                        (float)y - HALF_CHUNK, 
                        (float)z - HALF_CHUNK 
                    };
                    float* current_vertices = NULL;
                    float* current_normals = NULL;
                    float* current_uvs = NULL;
                    unsigned char* current_colors = NULL; // NEW: Pointer to the main color buffer
                    int* current_count_ptr = NULL;

                    // Placeholder for calculated face colors (6 vertices * 4 components = 24 bytes)
                    unsigned char face_colors[24]; 
                    
                    // TODO: Implement the AO calculation here, replacing this simple memset.
                    // For now, set to solid white (255, 255, 255, 255) to prove the color pipeline works.
                    memset(face_colors, 255, 24); 


                    // --- ASSIGN CURRENT POINTERS ---
                    if (chunk->blocks[x][y][z].blockType == BLOCK_GRASS) {
                        current_vertices = temp_grass_vertices;
                        current_normals = temp_grass_normals;
                        current_uvs = temp_grass_uvs;
                        current_colors = temp_grass_colors; // NEW: Color pointer
                        current_count_ptr = &grass_vert_count;

                    } else if (chunk->blocks[x][y][z].blockType == BLOCK_DIRT) {
                        current_vertices = temp_dirt_vertices;
                        current_normals = temp_dirt_normals;
                        current_uvs = temp_dirt_uvs;
                        current_colors = temp_dirt_colors; // NEW: Color pointer
                        current_count_ptr = &dirt_vert_count;

                    } else if (chunk->blocks[x][y][z].blockType == BLOCK_STONE) {
                        current_vertices = temp_stone_vertices;
                        current_normals = temp_stone_normals;
                        current_uvs = temp_stone_uvs;
                        current_colors = temp_stone_colors; // NEW: Color pointer
                        current_count_ptr = &stone_vert_count;

                    } else {
                        continue; //unknown block type
                    }

                    // check which faces are visible
                    if (visibleFaces & FACE_FRONT) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_FRONT, face_colors);
                    }
                    if (visibleFaces & FACE_BACK) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_BACK, face_colors);
                    }
                    if (visibleFaces & FACE_RIGHT) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_RIGHT, face_colors);
                    }
                    if (visibleFaces & FACE_LEFT) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_LEFT, face_colors);
                    }
                    if (visibleFaces & FACE_TOP) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_TOP, face_colors);
                    }
                    if (visibleFaces & FACE_BOTTOM) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_colors, current_count_ptr, block_pos, FACE_BOTTOM, face_colors);
                    }
                    
                }
            }
        }
    }
    // Finalize GRASS mesh and model
    FinalizeAndUploadMesh(&chunk->grassMesh, &chunk->grassModel,
                          temp_grass_vertices, temp_grass_normals, temp_grass_uvs, temp_grass_colors, grass_vert_count, grassTex); // NEW ARG

    // Finalize DIRT mesh and model
    FinalizeAndUploadMesh(&chunk->dirtMesh, &chunk->dirtModel,
                          temp_dirt_vertices, temp_dirt_normals, temp_dirt_uvs, temp_dirt_colors, dirt_vert_count, dirtTex); // NEW ARG

    // Finalize STONE mesh and model
    FinalizeAndUploadMesh(&chunk->stoneMesh, &chunk->stoneModel,
                          temp_stone_vertices, temp_stone_normals, temp_stone_uvs, temp_stone_colors, stone_vert_count, stoneTex); // NEW ARG


    // No explicit frees needed here, as they were moved to FinalizeAndUploadMesh
}

void RegenerateChunk(ChunkTable* chunkTable, Chunk* chunk) {
    if (!chunk) return;

    if (chunk->grassMesh.vaoId != 0) {
        //UnloadMesh(chunk->grassMesh);
        UnloadModel(chunk->grassModel); // UnloadModel relies on UnloadMesh success/state
    }
    
    if (chunk->dirtMesh.vaoId != 0) {
        //UnloadMesh(chunk->dirtMesh);
        UnloadModel(chunk->dirtModel);
    }
    
    if (chunk->stoneMesh.vaoId != 0) {
        //UnloadMesh(chunk->stoneMesh);
        UnloadModel(chunk->stoneModel);
    }

    // zero out Models and Meshes
    memset(&chunk->grassMesh, 0, sizeof(Mesh));
    memset(&chunk->grassModel, 0, sizeof(Model));

    memset(&chunk->dirtMesh, 0, sizeof(Mesh));
    memset(&chunk->dirtModel, 0, sizeof(Model));

    memset(&chunk->stoneMesh, 0, sizeof(Mesh));
    memset(&chunk->stoneModel, 0, sizeof(Model));

    // generate new mesh
    InitChunkMesh(chunkTable, chunk);

}

void UpdateNeighborChunkMesh(ChunkTable* table, Chunk* currentChunk, int bx, int by, int bz) {
    int cx = (int)currentChunk->table_pos.x;
    int cy = (int)currentChunk->table_pos.y;
    int cz = (int)currentChunk->table_pos.z;

    // If block is at X=0, update the Left Chunk
    if (bx == 0) {
        Chunk* neighbor = get_chunk(table, cx - 1, cy, cz);
        if (neighbor) RegenerateChunk(table, neighbor);
    }
    // If block is at X=15, update the Right Chunk
    else if (bx == CHUNK_SIZE - 1) {
        Chunk* neighbor = get_chunk(table, cx + 1, cy, cz);
        if (neighbor) RegenerateChunk(table, neighbor);
    }

    // Repeat for Y (Top/Bottom)
    if (by == 0) {
        Chunk* neighbor = get_chunk(table, cx, cy - 1, cz);
        if (neighbor) RegenerateChunk(table, neighbor);
    }
    else if (by == CHUNK_SIZE - 1) {
        Chunk* neighbor = get_chunk(table, cx, cy + 1, cz);
        if (neighbor) RegenerateChunk(table, neighbor);
    }

    // Repeat for Z (Front/Back)
    if (bz == 0) {
        Chunk* neighbor = get_chunk(table, cx, cy, cz - 1);
        if (neighbor) RegenerateChunk(table, neighbor);
    }
    else if (bz == CHUNK_SIZE - 1) {
        Chunk* neighbor = get_chunk(table, cx, cy, cz + 1);
        if (neighbor) RegenerateChunk(table, neighbor);
    }
}
