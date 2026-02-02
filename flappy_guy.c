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
  Rectangle rect;
  Texture2D texture;
} Flapper;

// init pipes
typedef struct {
  float x, y;
  Rectangle rect;
  Texture texture;
  bool active;
} Pipe;

void close_game(){
 // UnloadMusicStream(*soundtrack);
  CloseAudioDevice();
  CloseWindow();
}

void main_menu(bool *playing, bool *exit_game){
  static int currentItem = 0;
  static int menuCount = 3;

  if(IsKeyPressed(KEY_DOWN)){
    currentItem++;
    if(currentItem>=menuCount) currentItem = 0;
    printf("currentitem=%d\n", currentItem);
  }

  if(IsKeyPressed(KEY_UP)){
    currentItem--;
    if(currentItem<0) currentItem = menuCount-1;
    printf("currentitem=%d\n", currentItem);
  }

  if(currentItem == 0 && IsKeyPressed(KEY_ENTER)) *playing = true;
  
  if(currentItem == 2 && IsKeyPressed(KEY_ENTER)) *exit_game = true;

  ClearBackground(SKYBLUE);
  DrawText("PLAY", SCREENWIDTH/2, SCREENHEIGHT/4, 50, currentItem == 0 ? WHITE : BLACK);
  DrawText("SETTINGS", SCREENWIDTH/2, SCREENHEIGHT/3, 50, currentItem == 1 ? WHITE : BLACK);
  DrawText("QUIT", SCREENWIDTH/2, SCREENHEIGHT/2, 50, currentItem == 2 ? WHITE : BLACK);
}


Flapper Flapper_Init(float x, float y) {
  Flapper f = {0};
  f.x = x;
  f.y = y;
  f.velocity = 0.0f;
  f.alive    = 1;
  f.texture = LoadTexture("./assets/flapper1.png");
  f.rect = (Rectangle){
    .x = f.texture.width  * 0.1f,
    .y = f.texture.height * 0.25f,
    .width  = f.texture.width * 0.8f,
    .height = f.texture.height * 0.5f,
  };
  if(!f.alive){
    UnloadTexture(f.texture);
  }
  return f;
}

void Init_Pipes(Pipe pipes[], int count, float startX, Texture2D pipeTexture) {
  for(int i=0; i < count; i++){
    pipes[i].rect.x = startX + i * PIPE_SPACING;
    pipes[i].rect.y = GetRandomValue(SCREENHEIGHT-300, SCREENHEIGHT-100);
    pipes[i].texture = pipeTexture;
    pipes[i].rect.width = pipes[i].texture.width;
    pipes[i].rect.height = pipes[i].texture.height;
    pipes[i].active = true;
  }
}

void flapper_movement(Flapper *flapper) {
  static float velY     = 0.0f;
  const float gravity   = 0.5f;
  const float jumpForce = -10.0f;

  while(flapper->alive){
    flapper->rect.x += 1;

    if(IsKeyPressed(KEY_SPACE)){
      velY = jumpForce;
    }

    velY += gravity;
    flapper->y += velY;
    flapper->rect.y = flapper->y;
    flapper->x = flapper->rect.x;
    break;
  }
}

void collisions(Flapper *flapper, Pipe *pipes) {
  //collisions with the sky and floor
  if(flapper->rect.y <= 0 || flapper->rect.y+flapper->rect.height >= SCREENHEIGHT) { // the subtractions are because the models rectangle "hitbox" is bigger than the texture
    flapper->alive = 0;
  }
  printf("Flapper.topedge=%.1f -- flapper.bottomedge=%.1f\n", flapper->rect.y, flapper->rect.y+256);

  // collisions with pipes
  for(int i=0; i<PIPE_COUNT; i++){
    if(CheckCollisionRecs(flapper->rect, pipes[i].rect)){
      flapper->alive = 0;
      printf("Alive=%d\n", flapper->alive);
    }
  }
}

int main(void) {

  bool playing   = false;
  bool exit_game = false;

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
  Flapper flapper = Flapper_Init(25, 100);

  // init pipes
  Texture2D pipeTexture = LoadTexture("./assets/obstacle.png");
  Pipe pipes[PIPE_COUNT];
  Init_Pipes(pipes, PIPE_COUNT, 0, pipeTexture);

  // camera settings
  Camera2D camera = {0};
  camera.offset.x = SCREENWIDTH/2;
  camera.offset.y = SCREENHEIGHT/2;
  camera.target.y = SCREENHEIGHT/2;
  camera.rotation = 0;
  camera.zoom     = 1.0f;

  while (!WindowShouldClose() && !exit_game) { // game loop

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
      main_menu(&playing, &exit_game);
      EndDrawing();
    }else{
      flapper_movement(&flapper);
      collisions(&flapper, &pipes);
      
      BeginMode2D(camera);
      
      DrawTexture(cloud, 40, 20, WHITE);
      DrawRectangleLines(flapper.rect.x, flapper.rect.y, flapper.rect.width, flapper.rect.height, BLACK);
      
      // draw pipes
      for(int i=0; i<PIPE_COUNT; i++){
        if(!pipes[i].active) continue;
        DrawTexture(pipes[i].texture, pipes[i].rect.x, pipes[i].rect.y, WHITE);
        DrawRectangleLines(pipes[i].rect.x, pipes[i].rect.y, pipes[i].rect.width/2, pipes[i].rect.height/2, BLACK);
      }

      if(!flapper.alive){
        playing = false;

      }

      // printf("flapper.x=%f flapper.y=%f\n flapperrect.x%f flapperrect.height%f\n flapper.alive=%i\n", flapper.x, flapper.y, flapper.rect.x, flapper.rect.height, flapper.alive); // debug line
      DrawTexture(flapper.texture, flapper.x, flapper.y, WHITE);

      EndMode2D();
      EndDrawing();
    }
  }
  close_game();
}

