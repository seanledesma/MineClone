//basic window
#include "raylib.h" 

#define screenWidth 800
#define screenHeight 450

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raymine v0.0.1");

    SetTargetFPS(60);

    //main game loop here
    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);


        EndDrawing();
    }

    CloseWindow();

    return 0;       
}