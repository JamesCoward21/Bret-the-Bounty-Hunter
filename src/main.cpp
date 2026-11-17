#include "raylib.h"

int main() {
  // Create Game Window
  InitWindow(1000, 800, "2D Platformer");

  // Set FPS
  SetTargetFPS(60);

  // Main Game Loop
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangle(100, 400, 50, 50, BLUE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
