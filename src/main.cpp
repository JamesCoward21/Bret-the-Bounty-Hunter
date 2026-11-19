#include "raylib.h"
#include <vector>

struct Player {
  Vector2 position;
  Vector2 velocity;
  Vector2 size;
  Rectangle collider;
  bool isGrounded;
  Vector2 spawnPoint;
};

// Program main entry point

int main(void) {

  // Initialization

  const int screenWidth = 800;
  const int screenHeight = 500;

  std::vector<Rectangle> platforms{
      {0, 400, 600, 400},
      {200, 250, 200, 30},
  };

  InitWindow(screenWidth, screenHeight, "Bret The Bounty Hunter");

  Player player;
  player.position = {100, 350};
  player.velocity = {0, 0};
  player.size = {50, 50};
  player.isGrounded = false; // is false until player touches platform
  player.spawnPoint = {100, 350};

  const float speed = 250.0f;
  const float jump = -600.0f;
  const float gravity = 1000.0f;

  SetTargetFPS(60); // Set game to run at 60 FPS

  // Camera

  // Camera2D camera = { 0 };
  // camera.target = (Vector2){ PlayerPosition.x + 20.0f, PlayerPosition.y
  // + 20.0f }; camera.offset = (Vector2){}

  // Main Game Loop

  while (!WindowShouldClose()) // Detect Window close button or ESC key
  {
    // Insert varibles here

    // Player Movement

    float deltaTime = GetFrameTime(); // temporary variables outside struct
    float previousRightSideOfPlayer = player.position.x + player.size.x;
    float previousLeftSideOfPlayer = player.position.x;

    if (IsKeyDown(KEY_RIGHT))
      player.position.x = player.position.x + deltaTime * speed;
    if (IsKeyDown(KEY_LEFT))
      player.position.x = player.position.x + deltaTime * -speed;

    player.collider.x = player.position.x;
    player.collider.y = player.position.y;
    player.collider.width = player.size.x;
    player.collider.height = player.size.y;

    // Bottom Edge of player for detection with platform
    if (player.position.x <= 0) { // sets left boundary
      player.position.x = 0;
    }

    player.isGrounded = false;

    float currentRightSideOfPlayer = player.position.x + player.size.x;
    float currentLeftSideOfPlayer = player.position.x;

    for (Rectangle platform : platforms) {
      if (CheckCollisionRecs(player.collider, platform)) {
        if (previousRightSideOfPlayer <= platform.x &&
            currentRightSideOfPlayer >= platform.x &&
            player.position.y + player.size.y >= platform.y &&
            player.position.y <= platform.y + platform.height &&
            player.position.x >= 0) {

          player.position.x = platform.x - player.size.x;
        }
      }
      if (CheckCollisionRecs(player.collider, platform)) {
        if (previousLeftSideOfPlayer >= platform.x + platform.width &&
            currentLeftSideOfPlayer <= platform.x + platform.width &&
            player.position.y + player.size.y >= platform.y &&
            player.position.y <= platform.y + platform.height &&
            player.position.x >= 0) {

          player.position.x = platform.x + platform.width;
        }
      }
    }

    float previousBottomOfPlayer = player.position.y + player.size.y;
    float previousTopOfPlayer = player.position.y;

    // Gravity
    player.velocity.y =
        player.velocity.y +
        (deltaTime * gravity); // sets constant downward Velocity.

    player.position.y = player.position.y + (player.velocity.y * deltaTime);

    float currentBottomOfPlayer = player.position.y + player.size.y;
    float currentTopOfPlayer = player.position.y;

    player.collider.x = player.position.x;
    player.collider.y = player.position.y;
    player.collider.width = player.size.x;
    player.collider.height = player.size.y;

    for (Rectangle platform : platforms) {
      if (CheckCollisionRecs(player.collider, platform)) {
        if (previousBottomOfPlayer <= platform.y &&
            currentBottomOfPlayer >= platform.y &&
            player.position.x + player.size.x >= platform.x &&
            player.position.x <= platform.x + platform.width &&
            player.velocity.y >= 0) {
          player.velocity.y = 0;
          player.position.y = platform.y - player.size.y;
          player.isGrounded = true;
        }
      }
      if (CheckCollisionRecs(player.collider, platform)) {
        if (previousTopOfPlayer >= platform.y + platform.height &&
            currentTopOfPlayer <= platform.y + platform.height &&
            player.position.x + player.size.x >= platform.x &&
            player.position.x <= platform.x + platform.width &&
            player.velocity.y <= 0) {

          player.velocity.y = 0;
          player.position.y = platform.y + platform.height;
          player.isGrounded = false;
        }
      }
    }

    if (IsKeyPressed(KEY_SPACE) && player.isGrounded) {
      player.velocity.y = jump;
      player.isGrounded = false;
    }

    if (player.position.y + player.size.y > 1000) {
      player.position = player.spawnPoint;
      player.velocity.y = 0;
    }
    // Camera target follows player

    // camera.target = (Vector2)

    // Draw

    BeginDrawing();

    Color backgroundColor = {239, 179, 208, 255};

    ClearBackground(backgroundColor);

    DrawText("Planet Flergon", 50, 30, 20, GRAY);

    for (Rectangle platform : platforms) {
      DrawRectangleRec(platform, DARKGRAY);
    }

    DrawRectangleV(
        player.position, player.size,
        BLUE); // Draws Chaeacter as rectangle. need V at the end as
               // we've put. also note the {50, 50}. Because we used one
               // the vector Player position for the position on the
               // screen, the characters size also needs to be one
               // argument. the position in as the vector PlayerPosition

    EndDrawing();
  }
  // De-Initialization

  CloseWindow();

  return 0;
}
