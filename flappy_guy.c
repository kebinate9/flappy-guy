/*
* TODO
* 1. Add obstacle sprites
*   Obstacle sprites have been added however we need a way to automate
*   this and randomise their locations within an upper and lower bound.
* 2. Add collisions
*   This only needs to be between flapper and pipe obstacles. Clouds
*   do not count as obstacles and we can ignore this.
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

const int WINWIDTH   = 1200;
const int WINHEIGHT  = 600;

int main(void) {
  InitWindow(WINWIDTH, WINHEIGHT, "Flappy Guy");
  SetTargetFPS(60);

  Texture2D flapper_img1 = LoadTexture("./assets/flapper1.png");
  Texture2D flapper_img2 = LoadTexture("./assets/flapper2.png");
  Texture2D cloud1       = LoadTexture("./assets/cloud1.png");
  Texture2D obstacle     = LoadTexture("./assets/obstacle.png");

  int flapper_index = 0;
  Texture2D flappers[] = {flapper_img1, flapper_img2};

  int flapper_posx = 50;
  int flapper_posy = 200;
  float rotation = 180;
  while (!WindowShouldClose()) {

    if (flapper_posy >= WINHEIGHT-110){
      CloseWindow();
      return EXIT_SUCCESS;
    }else { // this makes the flapper move horizontally and fall
      flapper_posy +=2;
      flapper_posx ++;
    }

    BeginDrawing(); // drawing loop
      ClearBackground(BLUE);
      DrawTexture(flappers[flapper_index], flapper_posx, flapper_posy, WHITE);
      DrawTexture(cloud1, 30, 10, WHITE);
      DrawTexture(obstacle, 400, 300, WHITE);
      DrawTexture(obstacle, 600, 400, WHITE);

      Rectangle source = {0, 0, obstacle.width, obstacle.height};
      Rectangle dest = {400, 300, obstacle.width, obstacle.height};
      Vector2 origin = {obstacle.width/2.0f, obstacle.height/2.0f};

      DrawTexturePro(obstacle, source, dest, origin, rotation, WHITE);

      if (IsKeyPressed(KEY_SPACE)) {
        flapper_posy -= 40; // this makes the flapper jump, would like to turn this into an easing animation
        if (flapper_index == 0) {
          flapper_index = 1;
      } else {
          flapper_index = 0;
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return EXIT_SUCCESS;
}
      // DrawText("Hello World!", 375, 250, 40, GREEN);
      // BeginMode3D(camera);
      // DrawCube((Vector3){-4.0f,0.0f,2.0f}, 2.0f, 5.0f,2.0f, RED);
      // DrawCubeWires ( ( Vector3 ) { -4.0f, 0.0f, 2.0f }, 2.0f, 5.0f, 2.0f, GOLD );
      // DrawFPS(25, 25);
      // EndMode3D();
