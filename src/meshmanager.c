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

// LEFT Face (X- direction)
const float LEFT_VERTICES[] = {
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f, // Tri 1: BL, FL, TL
    0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f  // Tri 2: BL, TL, TR
};

const float LEFT_NORMALS[] = {
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f
};

const float LEFT_UVS[] = {
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
};


void AddFaceData(float* verts, float* uvs, float* normals, int* vert_count, Vector3 block_pos, int face_id) {
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
    const int start_index = *vert_count * 3; // Position in the array to start writing

    // LOOP 1: Vertices (6 * 3 = 18 floats)
    for (int i = 0; i < NUM_FLOATS; i += 3)
    {
        // Add the block's position (X, Y, Z) to the base vertex coordinates
        verts[start_index + i]     = base_verts[i] + block_pos.x;     // X + block_pos.x
        verts[start_index + i + 1] = base_verts[i + 1] + block_pos.y; // Y + block_pos.y
        verts[start_index + i + 2] = base_verts[i + 2] + block_pos.z; // Z + block_pos.z
    }

    // LOOP 2: Normals and UVs
    // Normals (6 * 3 = 18 floats) and UVs (6 * 2 = 12 floats) are simply copied
    // (A more advanced implementation would transform these for different faces)
    memcpy(&normals[start_index], base_normals, 6 * 3 * sizeof(float));
    memcpy(&uvs[*vert_count * 2], base_uvs, 6 * 2 * sizeof(float));

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

void FinalizeAndUploadMesh(Mesh* mesh, Model* model, float* temp_verts, float* temp_normals, float* temp_uvs, int vert_count, Texture2D texture) {
    // Handle the case where no faces were generated
    if (vert_count == 0) {
        // Initialize the model structure to prevent crashes when DrawModel() is called on it
        // Note: raylib initializes the Mesh/Model structures to safe values here.
        *model = LoadModelFromMesh(*mesh); 
        return;
    }

    // 1. Set Mesh properties
    mesh->vertexCount = vert_count;
    mesh->triangleCount = vert_count / 3;

    // 2. Allocate memory for Raylib's Mesh (MUST be allocated via malloc/calloc)
    // CRITICAL: Must check for allocation failure (returns NULL)

    mesh->vertices = (float *)malloc(vert_count * 3 * sizeof(float));
    if (mesh->vertices == NULL) return; // Failure, cannot proceed

    mesh->normals = (float *)malloc(vert_count * 3 * sizeof(float));
    if (mesh->normals == NULL) { 
        free(mesh->vertices); // Cleanup prior success
        return;
    }

    mesh->texcoords = (float *)malloc(vert_count * 2 * sizeof(float)); 
    if (mesh->texcoords == NULL) { 
        free(mesh->vertices);  // Cleanup prior successes
        free(mesh->normals);
        return;
    }

    // 3. Copy data from the temporary arrays
    memcpy(mesh->vertices, temp_verts, vert_count * 3 * sizeof(float));
    memcpy(mesh->normals, temp_normals, vert_count * 3 * sizeof(float));
    memcpy(mesh->texcoords, temp_uvs, vert_count * 2 * sizeof(float));

    // 4. Upload to GPU
    UploadMesh(mesh, false);

    // 5. Create the Model
    *model = LoadModelFromMesh(*mesh); 
    
    // IMPORTANT: Raylib expects the Mesh data (mesh->vertices, etc.) to remain allocated by you
    // (step 2) after LoadModelFromMesh is called. Do NOT free them here.

    // 6. Assign the Texture to the Model's Material
    // MAP_ALBEDO is the standard color (diffuse) texture map index.
    // If MAP_ALBEDO is undefined, you can use the integer index 0 instead.
    model->materials[0].maps[0].texture = texture;
}

void InitChunkMesh(ChunkTable* chunkTable, Chunk* chunk) {

    // track how many vertices have been added to each array
    int grass_vert_count = 0;
    int dirt_vert_count = 0;
    int stone_vert_count = 0;

    // max number of possilbe faces (6 faces and 4 possible vertices)
    const int MAX_VERTICES = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6;
    // // allocate memory for arrays to hold mesh data
    // float* temp_grass_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    // if (temp_grass_vertices == NULL) return;
    // float* temp_grass_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    // if (temp_grass_normals == NULL) { free(temp_grass_vertices); return; }
    // float* temp_grass_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    // if (temp_grass_uvs == NULL) { free(temp_grass_vertices); free(temp_grass_normals); return; }
    // //continue this for other block types
    // float* temp_dirt_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    // if (temp_dirt_vertices == NULL) return;
    // float* temp_dirt_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    // if (temp_dirt_normals == NULL) { free(temp_dirt_vertices); return; }
    // float* temp_dirt_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    // if (temp_dirt_uvs == NULL) { free(temp_dirt_vertices); free(temp_dirt_normals); return; }

    // float* temp_stone_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    // if (temp_stone_vertices == NULL) return;
    // float* temp_stone_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    // if (temp_stone_normals == NULL) { free(temp_stone_vertices); return; }
    // float* temp_stone_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    // if (temp_stone_uvs == NULL) { free(temp_stone_vertices); free(temp_stone_normals); return; }

    // --- ALLOCATION PHASE WITH CRITICAL CHECKS ---
    
    // 1. Grass Vertices
    float* temp_grass_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_grass_vertices == NULL) return;
    
    // 2. Grass Normals
    float* temp_grass_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_grass_normals == NULL) { 
        free(temp_grass_vertices); 
        return; 
    }
    
    // 3. Grass UVs
    float* temp_grass_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (temp_grass_uvs == NULL) { 
        free(temp_grass_vertices); 
        free(temp_grass_normals); 
        return; 
    }
    
    // 4. Dirt Vertices
    float* temp_dirt_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_dirt_vertices == NULL) { 
        free(temp_grass_vertices); 
        free(temp_grass_normals); 
        free(temp_grass_uvs); 
        return; 
    }
    
    // 5. Dirt Normals
    float* temp_dirt_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_dirt_normals == NULL) { 
        free(temp_grass_vertices); 
        free(temp_grass_normals); 
        free(temp_grass_uvs); 
        free(temp_dirt_vertices); 
        return; 
    }
    
    // 6. Dirt UVs
    float* temp_dirt_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (temp_dirt_uvs == NULL) { 
        free(temp_grass_vertices); 
        free(temp_grass_normals); 
        free(temp_grass_uvs); 
        free(temp_dirt_vertices); 
        free(temp_dirt_normals); 
        return; 
    }

    // 7. Stone Vertices
    float* temp_stone_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_stone_vertices == NULL) { 
        free(temp_grass_vertices); 
        free(temp_grass_normals); 
        free(temp_grass_uvs); 
        free(temp_dirt_vertices); 
        free(temp_dirt_normals); 
        free(temp_dirt_uvs);
        return; 
    }
    
    // 8. Stone Normals
    float* temp_stone_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (temp_stone_normals == NULL) { 
        free(temp_grass_vertices); 
        free(temp_grass_normals); 
        free(temp_grass_uvs); 
        free(temp_dirt_vertices); 
        free(temp_dirt_normals); 
        free(temp_dirt_uvs);
        free(temp_stone_vertices);
        return; 
    }
    
    // 9. Stone UVs
    float* temp_stone_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (temp_stone_uvs == NULL) { 
        free(temp_grass_vertices); 
        free(temp_grass_normals); 
        free(temp_grass_uvs); 
        free(temp_dirt_vertices); 
        free(temp_dirt_normals); 
        free(temp_dirt_uvs);
        free(temp_stone_vertices);
        free(temp_stone_normals);
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
                    Vector3 block_pos = { (float)x, (float)y, (float)z };
                    float* current_vertices = NULL;
                    float* current_normals = NULL;
                    float* current_uvs = NULL;
                    int* current_count_ptr = NULL;

                    //Texture2D texture;
                    if (chunk->blocks[x][y][z].blockType == BLOCK_GRASS) {
                        current_vertices = temp_grass_vertices;
                        current_normals = temp_grass_normals;
                        current_uvs = temp_grass_uvs;
                        current_count_ptr = &grass_vert_count;

                    } else if (chunk->blocks[x][y][z].blockType == BLOCK_DIRT) {
                        current_vertices = temp_dirt_vertices;
                        current_normals = temp_dirt_normals;
                        current_uvs = temp_dirt_uvs;
                        current_count_ptr = &dirt_vert_count;

                    } else if (chunk->blocks[x][y][z].blockType == BLOCK_STONE) {
                        current_vertices = temp_stone_vertices;
                        current_normals = temp_stone_normals;
                        current_uvs = temp_stone_uvs;
                        current_count_ptr = &stone_vert_count;

                    } else {
                        continue; //unknown block type
                    }

                    // check which faces are visible
                    if (visibleFaces & FACE_FRONT) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_count_ptr, block_pos, FACE_FRONT);
                    }
                    if (visibleFaces & FACE_BACK) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_count_ptr, block_pos, FACE_BACK);
                    }
                    if (visibleFaces & FACE_RIGHT) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_count_ptr, block_pos, FACE_RIGHT);
                    }
                    if (visibleFaces & FACE_LEFT) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_count_ptr, block_pos, FACE_LEFT);
                    }
                    if (visibleFaces & FACE_TOP) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_count_ptr, block_pos, FACE_TOP);
                    }
                    if (visibleFaces & FACE_BOTTOM) {
                        AddFaceData(current_vertices, current_uvs, current_normals, current_count_ptr, block_pos, FACE_BOTTOM);
                    }
                    
                }
            }
        }
    }
    // Finalize GRASS mesh and model
    FinalizeAndUploadMesh(&chunk->grassMesh, &chunk->grassModel,
                          temp_grass_vertices, temp_grass_normals, temp_grass_uvs, grass_vert_count, grassTex);

    // Finalize DIRT mesh and model
    FinalizeAndUploadMesh(&chunk->dirtMesh, &chunk->dirtModel,
                          temp_dirt_vertices, temp_dirt_normals, temp_dirt_uvs, dirt_vert_count, dirtTex);

    // Finalize STONE mesh and model
    FinalizeAndUploadMesh(&chunk->stoneMesh, &chunk->stoneModel,
                          temp_stone_vertices, temp_stone_normals, temp_stone_uvs, stone_vert_count, stoneTex);

    free(temp_grass_vertices);
    free(temp_grass_normals);
    free(temp_grass_uvs);
    
    free(temp_dirt_vertices);
    free(temp_dirt_normals);
    free(temp_dirt_uvs);

    free(temp_stone_vertices);
    free(temp_stone_normals);
    free(temp_stone_uvs);
}
