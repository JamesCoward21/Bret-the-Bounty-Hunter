#include "raylib.h"
#include <vector>

// Struct containing player variables
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

  // Initialization of game and player variables
  const int screenWidth = 800;
  const int screenHeight = 500;

  std::vector<Rectangle> platforms{
      {0, 400, 600, 1000},    {900, 400, 300, 1000},  {1400, 250, 200, 1100},
      {1800, 100, 200, 1300}, {2500, 800, 500, 500},  {3300, 750, 200, 600},
      {3800, 700, 200, 2000}, {4000, 700, 800, 500},  {4000, 1350, 2700, 1000},
      {4600, 1100, 600, 100}, {4800, 550, 200, 30},   {5200, 400, 200, 30},
      {5600, 250, 200, 30},   {6000, 250, 100, 1000}, {6100, 250, 700, 250},
      {6700, 600, 100, 1200}, {5200, 100, 200, 30},   {4800, -50, 200, 30},
      {4400, -150, 200, 30},  {4000, -250, 500, 50},  {6100, 1200, 500, 50},
      {6100, 1150, 125, 50},  {6500, 1000, 200, 50},  {6625, 880, 75, 120},
      {6500, 725, 100, 50},   {6100, 650, 75, 50},    {6800, 600, 300, 50},
      {6800, 450, 50, 50},    {7100, 350, 200, 50}};

  InitWindow(screenWidth, screenHeight, "Bret The Bounty Hunter");

  Player player;
  player.position = {100, 350};
  player.velocity = {0, 0};
  player.size = {50, 50};
  player.isGrounded = false; // is false until player touches platform
  player.spawnPoint = {100, 350};

  const float speed = 300.0f;
  const float jump = -600.0f;
  const float gravity = 1000.0f;

  SetTargetFPS(60); // Set game to run at 60 FPS

  // Camera Initialization
  Camera2D camera = {0};
  camera.target = (Vector2){400, player.position.y + 25};
  camera.offset = (Vector2){400, 250};
  camera.rotation = 0;
  camera.zoom = 1;

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

    // Bottom Edge of player for detection with platform
    if (player.position.x <= 0) { // sets left boundary
      player.position.x = 0;
    }

    // player collision detection
    player.collider.x = player.position.x;
    player.collider.y = player.position.y;
    player.collider.width = player.size.x;
    player.collider.height = player.size.y;

    player.isGrounded = false; // player only grounded on platform

    float currentRightSideOfPlayer = player.position.x + player.size.x;
    float currentLeftSideOfPlayer = player.position.x;

    // Conditions for not passing through sides of platform
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

    // Conditions for not passing through top and bottom of platform
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

    // Jumping
    if (IsKeyPressed(KEY_SPACE) && player.isGrounded) {
      player.velocity.y = jump;
      player.isGrounded = false;
    }

    // Respawn Condition
    if (player.position.y + player.size.y > 2000) {
      player.position = player.spawnPoint;
      player.velocity.y = 0;
    }
    // Camera target follows player. updated camera to work inside game loop
    camera.target = (Vector2){400, player.position.y + 25};
    camera.offset = (Vector2){400, 250};
    camera.rotation = 0;
    camera.zoom = 1;

    if (player.position.x >= 375) {
      camera.target.x = player.position.x + 25;
    }

    // Draw

    BeginDrawing();

    Color backgroundColor = {239, 179, 208, 255};

    ClearBackground(backgroundColor);

    DrawText("Planet Flergon", 50, 30, 20, GRAY);

    BeginMode2D(camera);

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
    EndMode2D();

    EndDrawing();
  }
  // De-Initialization

  CloseWindow();

  return 0;
}
