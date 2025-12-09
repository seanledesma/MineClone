#include "include.h"

fnl_state GLOBAL_NOISE_2D;
fnl_state GLOBAL_NOISE_3D;

void InitWorld(World* world) {
    //GLOBAL_NOISE_3D = fnlCreateState();
    //noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    //GLOBAL_NOISE_3D.noise_type = FNL_NOISE_CELLULAR;
    //noise.fractal_type = FNL_FRACTAL_PINGPONG;
    //noise.frequency = 0.007;

    // === Setting up global noise ===
    GLOBAL_NOISE_2D = fnlCreateState();
    GLOBAL_NOISE_2D.noise_type = FNL_NOISE_OPENSIMPLEX2;
    GLOBAL_NOISE_2D.fractal_type = FNL_FRACTAL_FBM;
    GLOBAL_NOISE_2D.frequency = 0.005f;
    GLOBAL_NOISE_2D.octaves = 4;
    GLOBAL_NOISE_2D.lacunarity = 2.0f;
    GLOBAL_NOISE_2D.gain = 0.5f;

    // === Setting up cave noise ===
    GLOBAL_NOISE_3D = fnlCreateState();
    GLOBAL_NOISE_3D.noise_type = FNL_NOISE_OPENSIMPLEX2;
    GLOBAL_NOISE_3D.fractal_type = FNL_FRACTAL_RIDGED;
    GLOBAL_NOISE_3D.frequency = 0.02f;
    GLOBAL_NOISE_3D.octaves = 2;
}

