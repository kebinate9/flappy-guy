#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define MAX_COLORS_COUNT   21

#define PIPE_GAP           200
#define PIPE_SPACING       300
#define PIPE_COUNT         10

const int SCREENHEIGHT  = 600;
const int SCREENWIDTH   = 1200;

Color colors[MAX_COLORS_COUNT] = {
  DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
  GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
  GREEN, SKYBLUE, PURPLE, BEIGE
};

// init flapper
typedef struct {
  float x, y;
  float velocity;
  int alive; // 0 - dead, 1 - alive, 2 - dying
  Rectangle flapperRect;
  Texture2D texture;
} Flapper;

// init pipes
typedef struct {
  float x, y;
  Rectangle pipeRect;
  Texture texture;
  bool active;
} Pipe;

Flapper Flapper_Init(float x, float y) {
  Flapper f = {0};
  f.x = x;
  f.y = y;
  f.velocity = 0.0f;
  f.alive    = 1;
  f.texture = LoadTexture("./assets/flapper1.png");
  f.flapperRect = (Rectangle){
    .x = x,
    .y = y,
    .width  = f.texture.width,
    .height = f.texture.height,
  };
  if(!f.alive){
    UnloadTexture(f.texture);
  }
  return f;
}

void Init_Pipes(Pipe pipes[], int count, float startX) {
  for(int i=0; i < count; i++){
    pipes[i].pipeRect.x = startX + i * PIPE_SPACING;
    pipes[i].pipeRect.y = GetRandomValue(SCREENHEIGHT-300, SCREENHEIGHT-100);
    pipes[i].texture = LoadTexture("./assets/obstacle.png");
    pipes[i].pipeRect.width = pipes[i].texture.width;
    pipes[i].pipeRect.height = pipes[i].texture.height;
    pipes[i].active = true;
  }
}

void flapper_movement(Flapper *flapper) {
  static float velY     = 0.0f;
  const float gravity   = 0.5f;
  const float jumpForce = -10.0f;

  while(flapper->alive){
    flapper->flapperRect.x += 1;

    if(IsKeyPressed(KEY_SPACE)){
      velY = jumpForce;
    }

    velY += gravity;
    flapper->y += velY;
    flapper->flapperRect.y = flapper->y;
    flapper->x = flapper->flapperRect.x;
    break;
  }
}

void collisions(Flapper *flapper) {
  //collisions with the sky and floor
  if(flapper->flapperRect.y <= -70 || flapper->flapperRect.y >= SCREENHEIGHT-115) { // the subtractions are because the models rectangle "hitbox" is bigger than the texture
    flapper->alive = 0;
  }
}

int main(void) {

  bool playing = false;

  InitWindow(SCREENWIDTH, SCREENHEIGHT, "Flappy Guy");
  SetTargetFPS(60);

  // init music
  InitAudioDevice();
  Music soundtrack = LoadMusicStream("./assets/soundtrack.mp3");
  bool pause = false;
  SetMusicVolume(soundtrack, 1.0f);
  PlayMusicStream(soundtrack);

  // test cloud: so i have a frame of reference for the flappers location on the screen
  Texture2D cloud = LoadTexture("./assets/cloud1.png");

  // init flapper
  Flapper flapper = Flapper_Init(25, SCREENHEIGHT/2);

  // init pipes
  Pipe pipes[PIPE_COUNT];
  Init_Pipes(pipes, PIPE_COUNT, 0);

  // camera settings
  Camera2D camera = {0};
  camera.offset.x = SCREENWIDTH/2;
  camera.offset.y = SCREENHEIGHT/2;
  camera.target.y = SCREENHEIGHT/2;
  camera.rotation = 0;
  camera.zoom     = 1.0f;

  while (!WindowShouldClose()) { // game loop

    camera.target.x = flapper.x+500;

    // soundtrack settings
    UpdateMusicStream(soundtrack);
    if (IsKeyPressed(KEY_M)) {
      pause = !pause;
      pause ? StopMusicStream(soundtrack) : PlayMusicStream(soundtrack);
    }

    BeginDrawing();
    ClearBackground(SKYBLUE);

    if(!playing){
      DrawText("Press space to start", 50, 100, 50, BLACK);
      if(IsKeyPressed(KEY_SPACE)) playing = true;
      EndDrawing();
    }else{
      flapper_movement(&flapper);
      collisions(&flapper);

      BeginMode2D(camera);
      DrawTexture(cloud, 40, 20, WHITE);

      // draw pipes
      for(int i=0; i<PIPE_COUNT; i++){
        if(!pipes[i].active) continue;

        DrawTexture(pipes[i].texture, pipes[i].pipeRect.x, pipes[i].pipeRect.y, WHITE);
      }

      // printf("flapper.x=%f flapper.y=%f\n flapperrect.x%f flapperrect.height%f\n flapper.alive=%i\n", flapper.x, flapper.y, flapper.flapperRect.x, flapper.flapperRect.height, flapper.alive); // debug line
      DrawTexture(flapper.texture, flapper.x, flapper.y, WHITE);

      EndMode2D();
      EndDrawing();
    }
  }
  UnloadMusicStream(soundtrack);
  CloseAudioDevice();
  CloseWindow();
}

