#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define MAX_COLORS_COUNT 21

const int WINWIDTH   = 1200;
const int WINHEIGHT  = 600;

Color colors[MAX_COLORS_COUNT] = {
  DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
  GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
  GREEN, SKYBLUE, PURPLE, BEIGE
};

int falling(int wh, int ww, int *fy, int *fx) {
  if (*fy >= wh-110){
    BeginDrawing();
    DrawText("FLAPPY GUY IS DEAD", ww/3, 100, 50, RED);
    EndDrawing();
  }else { // this makes the flapper move horizontally and fall
    (*fy) +=2;
    (*fx) ++;
  }
}

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

  float rotation_top    = 0;
  float rotation_bottom = 180;

  

  while (!WindowShouldClose()) {

    falling(WINHEIGHT, WINWIDTH, &flapper_posy, &flapper_posx);
    
    printf("x=%d, y=%d\n", flapper_posx, flapper_posy);
    BeginDrawing(); // drawing loop
      ClearBackground(BLUE);
      DrawTexture(flappers[flapper_index], flapper_posx, flapper_posy, WHITE);
      DrawTexture(cloud1, 30, 10, WHITE);

      Rectangle source = {0, 0, obstacle.width, obstacle.height};
      Rectangle dest = {400, 300, obstacle.width*2, obstacle.height*2};
      Vector2 origin = {obstacle.width/2.0f, obstacle.height/2.0f};

      DrawTexturePro(obstacle, source, dest, origin, rotation_top, WHITE);
      DrawTexturePro(obstacle, source, dest, origin, rotation_bottom, WHITE);
      DrawTexturePro(obstacle, source, dest, origin, rotation_top, WHITE);
      DrawTexturePro(obstacle, source, dest, origin, rotation_bottom, WHITE);
      DrawTexturePro(obstacle, source, dest, origin, rotation_top, WHITE);

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
