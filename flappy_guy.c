#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define MAX_COLORS_COUNT 21
#define OBSTACLE_COUNT 100

const int SCREENHEIGHT =  600;
const int SCRENWIDTH   =  1200;

int main(void) {
  InitWindow(SCREENWIDTH, SCREENHEIGHT, "Flappy Guy");
  InitAudioDevice();
  SetMusicVolume(soundtrack, 1.0f);
  PlayMusicStream(soundtrack);

  while (!WindowShouldClose()) {

    UpdateMusicStream(soundtrack);

    Camera2D camera = {0};
    camera.offset.x = SCREENWIDTH/2;
    camera.offset.y = SCREENHEIGHT/2;
    camera.target.x = SCREENWIDTH/2;
    camera.target.y = SCREENHEIGHT/2;
    camera.rotation = 0;
    camera.zoom     = 1.0f;

    BeginDrawing();
    ClearBackground(SKYBLUE);

    BeginMode2D(camera);
    EndMode2D();
    EndDrawing();
  }
}

