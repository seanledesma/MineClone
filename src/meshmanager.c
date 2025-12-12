#include "include.h"

Texture2D grassTex;
Texture2D dirtTex;
Texture2D stoneTex;

int facesDrawn = 0;

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

// const float LEFT_VERTICES[] = {
//     // Tri 1: BB, TF, TB (CW Order)
//     0.0f, 0.0f, 0.0f,  // V1: BB
//     0.0f, 1.0f, 1.0f,  // V2: TF  <- SWAPPED
//     0.0f, 1.0f, 0.0f,  // V3: TB  <- SWAPPED

//     // Tri 2: BB, BF, TF (Your original broken winding)
//     0.0f, 0.0f, 0.0f,  // V4: BB
//     0.0f, 0.0f, 1.0f,  // V5: BF
//     0.0f, 1.0f, 1.0f   // V6: TF
// };

const float LEFT_VERTICES[] = {
    // --- Reversed Winding for Left Face (X-) ---

    // Tri 1: C0, C3, C2 -> (Bottom Back, Top Front, Top Back)
    0.0f, 0.0f, 0.0f,  // V0: Bottom Back (BB)
    0.0f, 1.0f, 1.0f,  // V1: Top Front (TF)
    0.0f, 1.0f, 0.0f,  // V2: Top Back (TB)

    // Tri 2: C0, C1, C3 -> (Bottom Back, Bottom Front, Top Front)
    0.0f, 0.0f, 0.0f,  // V3: Bottom Back (BB)
    0.0f, 0.0f, 1.0f,  // V4: Bottom Front (BF)
    0.0f, 1.0f, 1.0f   // V5: Top Front (TF)
};

const float LEFT_NORMALS[] = {
    // Ensure all 6 vertices use the correct normal: -1, 0, 0
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f
};

// const float LEFT_UVS[] = {
//     // Tri 1: V1(BB), V2(TF), V3(TB)
//     0.0f, 1.0f, // V1: BB
//     1.0f, 0.0f, // V2: TF  <- SWAPPED
//     0.0f, 0.0f, // V3: TB  <- SWAPPED

//     // Tri 2: V4(BB), V5(BF), V6(TF)
//     0.0f, 1.0f, // V4: BB
//     1.0f, 1.0f, // V5: BF
//     1.0f, 0.0f  // V6: TF
// };

const float LEFT_UVS[] = {
    // V0 (BB), V1 (TF), V2 (TB)
    0.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
    // V3 (BB), V4 (BF), V5 (TF)
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


// Defines the maximum shade level (0 = full light, 3 = max shadow)
// This uses a simple sum of 1 for each solid neighbor block.
static int GetAOShade(ChunkTable* table, Chunk* chunk, int x, int y, int z, int dx1, int dy1, int dz1, int dx2, int dy2, int dz2) {
    // Determine if each of the three adjacent blocks is solid (not air)
    // d1: Block along axis 1
    // d2: Block along axis 2
    // d_corner: Block at the corner
    
    // Check neighbors: d1, d2, and d_corner
    int d1_is_solid = !IsBlockAir(table, chunk, x + dx1, y + dy1, z + dz1);
    int d2_is_solid = !IsBlockAir(table, chunk, x + dx2, y + dy2, z + dz2);
    int d_corner_is_solid = !IsBlockAir(table, chunk, x + dx1 + dx2, y + dy1 + dy2, z + dz1 + dz2);

    // Sum the occlusion factors. Standard AO algorithm:
    // If only one neighbor is solid: AO = 1
    // If two neighbors are solid: AO = 2
    // If three neighbors are solid: AO = 3 (max shadow)
    
    // The check for d_corner is conditional based on d1 and d2 to smooth the shadow.
    if (d1_is_solid && d2_is_solid) {
        return 3;
    }
    
    return d1_is_solid + d2_is_solid + d_corner_is_solid;
}

// Maps the AO factor (0-3) to a vertex color value (e.g., 255 down to 180)
static unsigned char AO_TO_COLOR(int ao_factor) {
    // 0: max light (255)
    // 1: slight shadow (230)
    // 2: medium shadow (200)
    // 3: max shadow (180)
    
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
    memcpy(&colors[color_start_index], face_colors, NUM_COLOR_BYTES);

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

    memcpy(mesh->colors, temp_colors, vert_count * 4 * sizeof(unsigned char));
    
//     unsigned char face_colors[24];
//     // NEW: Copy color data
//    //memcpy(mesh->colors, temp_colors, vert_count * 4 * sizeof(unsigned char));
//     unsigned char gray_value = 100;
//     for (int i = 0; i < 24; i += 4) {
//         face_colors[i] = gray_value;       // R
//         face_colors[i+1] = gray_value;     // G
//         face_colors[i+2] = gray_value;     // B
//         face_colors[i+3] = 255;            // A
//     }


    // 4. Upload to GPU
    UploadMesh(mesh, false);

    // 5. Create the Model
    *model = LoadModelFromMesh(*mesh); 
    
    // 6. Assign the Texture
    model->materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;

    model->materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;

    // NEW FIX: Explicitly set the material's diffuse color to WHITE.
    // If the vertex color exists, many default shaders will automatically
    // perform multiplication: FinalColor = VertexColor * TextureColor * MaterialColor.
    // Setting MaterialColor to WHITE (255,255,255,255) ensures only VertexColor darkens the Texture.
    //model->materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;

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

// RENAME and REFACTOR: This function now performs only CPU-heavy work (meshing) 
// and returns the raw data buffers via a JobResult struct.
JobResult* GenerateRawMesh(ChunkTable* chunkTable, Chunk* chunk) {
    
    // 0. Allocate the JobResult struct that will hold the buffers
    JobResult* result = (JobResult*)malloc(sizeof(JobResult));
    if (result == NULL) return NULL;
    
    // Set chunk and table pointers
    result->chunk = chunk;
    result->table = chunkTable;
    
    // Initialize counts to zero
    int grass_vert_count = 0;
    int dirt_vert_count = 0;
    int stone_vert_count = 0;
    
    // max number of possilbe vertices (6 faces * 6 vertices = 36 vertices/block max)
    const int MAX_VERTICES = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36;
    
    // --- GRASS ALLOCATIONS ---
    result->grass_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->grass_vertices == NULL) { CleanupJobResult(result); return NULL; }
    result->grass_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->grass_normals == NULL) { CleanupJobResult(result); return NULL; }
    result->grass_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (result->grass_uvs == NULL) { CleanupJobResult(result); return NULL; }
    result->grass_colors = (unsigned char *)malloc(MAX_VERTICES * 4 * sizeof(unsigned char));
    if (result->grass_colors == NULL) { CleanupJobResult(result); return NULL; }
    
    // --- DIRT ALLOCATIONS ---
    result->dirt_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->dirt_vertices == NULL) { CleanupJobResult(result); return NULL; } 
    result->dirt_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->dirt_normals == NULL) { CleanupJobResult(result); return NULL; }
    result->dirt_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (result->dirt_uvs == NULL) { CleanupJobResult(result); return NULL; }
    result->dirt_colors = (unsigned char *)malloc(MAX_VERTICES * 4 * sizeof(unsigned char));
    if (result->dirt_colors == NULL) { CleanupJobResult(result); return NULL; } 

    // --- STONE ALLOCATIONS ---
    result->stone_vertices = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->stone_vertices == NULL) { CleanupJobResult(result); return NULL; } 
    result->stone_normals = (float *)malloc(MAX_VERTICES * 3 * sizeof(float));
    if (result->stone_normals == NULL) { CleanupJobResult(result); return NULL; }
    result->stone_uvs = (float *)malloc(MAX_VERTICES * 2 * sizeof(float));
    if (result->stone_uvs == NULL) { CleanupJobResult(result); return NULL; }
    result->stone_colors = (unsigned char *)malloc(MAX_VERTICES * 4 * sizeof(unsigned char));
    if (result->stone_colors == NULL) { CleanupJobResult(result); return NULL; }

    // loop through all blocks in chunk
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
                    facesDrawn += __builtin_popcount(visibleFaces); // use popcount for accurate face count
                    
                    Vector3 block_pos = { 
                        (float)x - HALF_CHUNK, 
                        (float)y - HALF_CHUNK, 
                        (float)z - HALF_CHUNK 
                    };
                    
                    // Pointers now point to the JobResult buffers
                    float* current_vertices = NULL;
                    float* current_normals = NULL;
                    float* current_uvs = NULL;
                    unsigned char* current_colors = NULL;
                    int* current_count_ptr = NULL;

                    // Placeholder for calculated face colors (6 vertices * 4 components = 24 bytes)
                    unsigned char face_colors[24]; 
                    
                    // Array to hold 4 AO factors (0-3) for the face's 4 corners.
                    int ao[4]; 
                    int ao_factors[6]; // Holds the AO value (0-3) for each of the 6 vertices.


                    // --- ASSIGN CURRENT POINTERS ---
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
                        // This block type is visible but not handled by the meshes, continue
                        continue; 
                    }

                    // ----------------------------------------------------------------
                    // NOTE: Your AO logic is mostly sound and is carried over here. 
                    // ----------------------------------------------------------------

                    // check which faces are visible
                    // --- FACE_FRONT (Z+) AO ---
                    if (visibleFaces & FACE_FRONT) {
                        // Corner Definitions (Relative to Block: X, Y, Z)
                        // C0: Bottom-Left (x=0, y=0) | Neighbors: (-1, 0, 0) and (0, -1, 0)
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, -1, 0); 
                        // C1: Bottom-Right (x=1, y=0) | Neighbors: (+1, 0, 0) and (0, -1, 0)
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, -1, 0);
                        // C2: Top-Left (x=0, y=1) | Neighbors: (-1, 0, 0) and (0, +1, 0)
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, +1, 0); 
                        // C3: Top-Right (x=1, y=1) | Neighbors: (+1, 0, 0) and (0, +1, 0)
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, +1, 0); 
                        
                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 

                        // Front Vertices: (BL, TR, TL) and (BL, BR, TR) -> (C0, C3, C2) and (C0, C1, C3)
                        if (d1_sum > d2_sum) {
                             // FLIPPED Triangulation: (C1, C2, C3) and (C1, C0, C2)
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            // STANDARD Triangulation: (C0, C3, C2) and (C0, C1, C3)
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
                    
                    // --- FACE_BACK (Z-) AO ---
                    if (visibleFaces & FACE_BACK) {
                        // C0: Bottom-Left (x=0, y=0) | Neighbors: (+1, 0, 0) and (0, -1, 0)
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, -1, 0); 
                        // C1: Bottom-Right (x=1, y=0) | Neighbors: (-1, 0, 0) and (0, -1, 0)
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, -1, 0);
                        // C2: Top-Left (x=0, y=1) | Neighbors: (+1, 0, 0) and (0, +1, 0)
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, +1, 0); 
                        // C3: Top-Right (x=1, y=1) | Neighbors: (-1, 0, 0) and (0, +1, 0)
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, +1, 0); 

                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 
                        
                        // Back Vertices: (BL, TL, TR) and (BL, TR, BR) -> (C0, C2, C3) and (C0, C3, C1)
                        if (d1_sum > d2_sum) {
                             // FLIPPED Triangulation: (C1, C2, C3) and (C1, C0, C2)
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            // STANDARD Triangulation: (C0, C2, C3) and (C0, C3, C1)
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

                    // --- FACE_RIGHT (X+) AO ---
                    if (visibleFaces & FACE_RIGHT) {
                        // C0: Bottom-Back (y=0, z=0) | Neighbors: (0, -1, 0) and (0, 0, -1)
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, 0, -1, 0, 0, 0, -1); 
                        // C1: Bottom-Front (y=0, z=1) | Neighbors: (0, -1, 0) and (0, 0, +1)
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, 0, -1, 0, 0, 0, +1);
                        // C2: Top-Back (y=1, z=0) | Neighbors: (0, +1, 0) and (0, 0, -1)
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, 0, +1, 0, 0, 0, -1); 
                        // C3: Top-Front (y=1, z=1) | Neighbors: (0, +1, 0) and (0, 0, +1)
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, 0, +1, 0, 0, 0, +1); 

                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 

                        // Right Vertices: (BB, TF, BF) and (BB, TB, TF)
                        // This seems off based on your provided RIGHT_VERTICES array (BD, TR, BR, BD, TD, TR)
                        // Assuming the triangle order: (C0, C3, C1) and (C0, C2, C3)
                        if (d1_sum > d2_sum) {
                            // FLIPPED Triangulation: (C1, C2, C3) and (C1, C0, C2)
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            // STANDARD Triangulation: (C0, C3, C1) and (C0, C2, C3)
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

                    // --- FACE_LEFT (X-) AO ---
                    if (visibleFaces & FACE_LEFT) {
                        // C0: Bottom-Back (y=0, z=0) | Neighbors: (0, -1, 0) and (0, 0, -1)
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, 0, -1, 0, 0, 0, -1); 
                        // C1: Bottom-Front (y=0, z=1) | Neighbors: (0, -1, 0) and (0, 0, +1)
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, 0, -1, 0, 0, 0, +1);
                        // C2: Top-Back (y=1, z=0) | Neighbors: (0, +1, 0) and (0, 0, -1)
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, 0, +1, 0, 0, 0, -1); 
                        // C3: Top-Front (y=1, z=1) | Neighbors: (0, +1, 0) and (0, 0, +1)
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, 0, +1, 0, 0, 0, +1); 

                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 

                        // Left Vertices: (BB, TF, TB) and (BB, BF, TF) -> (C0, C3, C2) and (C0, C1, C3)
                        if (d1_sum > d2_sum) {
                            // FLIPPED Triangulation: (C1, C2, C3) and (C1, C0, C2)
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            // STANDARD Triangulation: (C0, C3, C2) and (C0, C1, C3)
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


                    // --- FACE_TOP (Y+) AO (Unchanged from your logic) ---
                    if (visibleFaces & FACE_TOP) {
                        // 1. Calculate 4 Corners AO
                        int c0_ao = GetAOShade(chunkTable, chunk, x, y, z, 0, 0, -1, -1, 0, 0); 
                        int c1_ao = GetAOShade(chunkTable, chunk, x, y, z, 0, 0, -1, +1, 0, 0);
                        int c2_ao = GetAOShade(chunkTable, chunk, x, y, z, 0, 0, +1, -1, 0, 0); 
                        int c3_ao = GetAOShade(chunkTable, chunk, x, y, z, 0, 0, +1, +1, 0, 0); 

                        ao[0] = c0_ao; ao[1] = c1_ao; ao[2] = c2_ao; ao[3] = c3_ao; 

                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 
                        
                        // TOP_VERTICES order: (V0=C0, V1=C3, V2=C1) and (V3=C0, V4=C2, V5=C3)
                        if (d1_sum > d2_sum) { 
                            // FLIPPED Triangulation: (C1, C2, C3) and (C1, C0, C2)
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            // STANDARD Triangulation: (C0, C3, C1) and (C0, C2, C3)
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
                    // --- END FACE_TOP AO ---

                    // --- FACE_BOTTOM (Y-) AO (Unchanged from your logic) ---
                    if (visibleFaces & FACE_BOTTOM) {
                        // 1. Calculate 4 Corners AO
                        ao[0] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, 0, -1); 
                        ao[1] = GetAOShade(chunkTable, chunk, x, y, z, +1, 0, 0, 0, 0, +1);
                        ao[2] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, 0, -1); 
                        ao[3] = GetAOShade(chunkTable, chunk, x, y, z, -1, 0, 0, 0, 0, +1); 
                        
                        int d1_sum = ao[0] + ao[3]; 
                        int d2_sum = ao[1] + ao[2]; 
                        
                        // BOTTOM_VERTICES order: (V0=C0, V1=C2, V2=C3) and (V3=C0, V4=C3, V5=C1)

                        if (d1_sum > d2_sum) { 
                            // FLIPPED Triangulation: (C1, C2, C3) and (C1, C0, C2)
                            ao_factors[0] = ao[1]; ao_factors[1] = ao[2]; ao_factors[2] = ao[3]; 
                            ao_factors[3] = ao[1]; ao_factors[4] = ao[0]; ao_factors[5] = ao[2]; 
                        } else {
                            // STANDARD Triangulation: (C0, C2, C3) and (C0, C3, C1)
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
                    // --- END FACE_BOTTOM AO ---
                }
            }
        }
    }

    // 1. Save final vertex counts
    result->grass_vert_count = grass_vert_count;
    result->dirt_vert_count = dirt_vert_count;
    result->stone_vert_count = stone_vert_count;

    // 2. Shrink/reallocate the buffers to the exact size used (optional, but good practice)
    // NOTE: This can be a huge performance boost if your MAX_VERTICES is much larger than needed.
    // However, for minimal alterations, we skip this step and let the main thread handle the size via vert_count.

    // 3. Return the result package (containing the chunk and the raw buffers)
    return result; 
}

void InitChunkMesh(ChunkTable* chunkTable, Chunk* chunk) {
    // This function should no longer be called directly, but we keep it to prevent compile errors.
    TraceLog(LOG_WARNING, "InitChunkMesh should not be called directly. Use threading pipeline.");
}

// In RegenerateChunk, you'll need to update the logic to re-enqueue a job.
void RegenerateChunk(ChunkTable* chunkTable, Chunk* chunk) {
    // Instead of calling InitChunkMesh, we now submit a job to re-mesh the chunk.
    Job* job = create_job(chunkTable, chunk->table_pos.x, chunk->table_pos.y, chunk->table_pos.z);
    job_push(job);
    cnd_signal(&job_available);
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
