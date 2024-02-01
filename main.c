/*******************************************************************************************
 *
 *   raylib [core] example - Basic window
 *
 *   Welcome to raylib!
 *
 *   To test examples, just press F6 and execute raylib_compile_execute script
 *   Note that compiled executable is placed in the same folder as .c file
 *
 *   You can find all basic examples on C:\raylib\raylib\examples folder or
 *   raylib official webpage: www.raylib.com
 *
 *   Enjoy using raylib. :)
 *
 *   Example originally created with raylib 1.0, last time updated with
 *raylib 1.0
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an
 *OSI-certified, BSD-like license that allows static linking with closed source
 *software
 *
 *   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "rlgl.h"
#include <stdio.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 128;
  const int screenHeight = 128;

  // structure pour représenter le pointeur
  struct {
    Vector2 pos;
  } ptr = {{0}};

  // structure pour représenter le jokari
  struct {
    Vector2 pos; // position
    Vector2 vel; // vélocité
  } jok = {
      // valeurs initiales
      {(float)screenWidth / 2, (float)screenHeight / 2},
      {0, 0},
  };

  Vector2 force = {0, 0};
  const Vector2 gravite = {0, 10};

  InitWindow(screenWidth, screenHeight, "Extreme Jokari");
  ToggleFullscreen();

  Camera2D camera = {0};
  camera.zoom = 1;

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    ptr.pos = GetScreenToWorld2D(GetMousePosition(), camera);

    force = Vector2Subtract(ptr.pos, jok.pos);
    force = Vector2Scale(force, 0.05);
    jok.vel = Vector2Add(jok.vel, force);
    jok.vel = Vector2Add(jok.vel, gravite);
    jok.vel = Vector2Scale(jok.vel, 0.9);
    jok.pos = Vector2Add(jok.pos, jok.vel);

    // mouvement de l'écran
    if (IsKeyDown(KEY_RIGHT)) {
      camera.offset.x -= 10;
    }

    if (IsKeyDown(KEY_LEFT)) {
      camera.offset.x += 10;
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode2D(camera);

    // je dessine une grille pour voir le mouvement
    rlPushMatrix();
    rlTranslatef(0, 50 * 200, 0);
    rlRotatef(90, 1, 0, 0);
    DrawGrid(100, 200);
    rlPopMatrix();
    DrawLineV(ptr.pos, jok.pos, BLUE);
    DrawCircleV(ptr.pos, 10, RAYWHITE);
    DrawCircleV(jok.pos, 30, RED);

    EndMode2D();

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
