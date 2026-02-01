#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define MAX_COLORS_COUNT 21
#define OBSTACLE_COUNT 100

typedef struct {
  Rectangle dest;
  float rotation;
} Obstacle;

Obstacle obstacles[OBSTACLE_COUNT];

Texture2D obstacle;

const int FPS        = 60;
const int WINWIDTH   = 1200;
const int WINHEIGHT  = 600;
const int CENTERX    = WINWIDTH/2;
const int CENTERY    = WINHEIGHT/2;
bool is_dead = false;

Color colors[MAX_COLORS_COUNT] = {
  DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
  GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
  GREEN, SKYBLUE, PURPLE, BEIGE
};

int jumping(float *fy, int *fx) {
  static float velY      =   0.0f;
  const  float gravity   =   0.5f;
  const  float jumpForce = -10.0f;

  if (IsKeyPressed(KEY_SPACE)) {
    velY = jumpForce;
  }

  velY += gravity;
  *fy += velY;
  *fx += 3;

  if(*fy > WINHEIGHT-110) {
    *fy = 490.0f;
    velY = 0.0f;
  }
  return 0;
}

void init_obstacles(void) {
  for(int i = 0; i < OBSTACLE_COUNT; i++) {
    float scale = GetRandomValue(15, 30) / 10.0f;

    obstacles[i].dest = (Rectangle) {
      (i+1) * 100,
      GetRandomValue(400, 500),
      obstacle.width * scale,
      obstacle.height * scale
    };

    obstacles[i].rotation = 0.0f;
  }
}

int main(void) {
  InitWindow(WINWIDTH, WINHEIGHT, "Flappy Guy");
  InitAudioDevice();
  SetTargetFPS(FPS);
  bool game_start = false;

  // load textures
  Texture2D cloud1       = LoadTexture("./assets/cloud1.png");
  Texture2D flapper_img1 = LoadTexture("./assets/flapper1.png");
  Texture2D flapper_img2 = LoadTexture("./assets/flapper2.png");
  obstacle               = LoadTexture("./assets/obstacle.png");
  int flapper_index = 0;
  Texture2D flappers[] = {flapper_img1, flapper_img2};
  Music soundtrack      = LoadMusicStream("./assets/soundtrack.mp3");
  SetMusicVolume(soundtrack, 1.0f);
  PlayMusicStream(soundtrack);

  // flapper initial position
  int flapper_posx = 50;
  float flapper_posy = 200;
  
  // controls the rotation for obstacle textures
  float rotation_top    = 0;
  float rotation_bottom = 180;
  
  while (!game_start) {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    DrawText("Press space to start", 20, WINHEIGHT/2, 70, BLACK);
    EndDrawing();
    if(IsKeyPressed(KEY_SPACE)) {
      game_start = true;
    }
  }
  init_obstacles();
  while (!WindowShouldClose() && game_start) {

  UpdateMusicStream(soundtrack);

    Camera2D camera = {0};
    camera.offset.x = CENTERX-500;
    camera.offset.y = CENTERY-100;
    camera.target.x = flapper_posx;
    camera.target.y = CENTERY-100;
    camera.rotation = 0;
    camera.zoom     = 1.0f;

    
    BeginDrawing(); // drawing loop
      BeginMode2D(camera);
      if(!is_dead) {
        jumping(&flapper_posy, &flapper_posx);
        printf("x=%d, y=%f fps=%i is_dead=%s\n", flapper_posx, flapper_posy, GetFPS(), is_dead ? "true" : "false"); // debug line
        if(flapper_posy == 490) {
          is_dead = true;
          SetTargetFPS(10); // slows down the fps so i can think
        }
      }else {
        DrawText("You are dead", flapper_posx+100, flapper_posy-400, 100, RED);
        DrawText("Press ESC to quit or space to play again", flapper_posx + 10, flapper_posy-200, 50, BLACK);
        if(IsKeyPressed(KEY_SPACE)) { 
          flapper_posy = 200;
          flapper_posx = 50;
          SetTargetFPS(60);
          is_dead = false;
        }
      }

      ClearBackground(SKYBLUE);
      DrawTexture(flappers[flapper_index], flapper_posx, flapper_posy, WHITE);
      DrawTexture(cloud1, 30, 10, WHITE);
      Rectangle source = { 0, 0, obstacle.width, obstacle.height };
      Vector2 origin = { obstacle.width / 2.0f, obstacle.height / 2.0f };

      for (int i = 0; i < OBSTACLE_COUNT; i++) {
        DrawTexturePro(
          obstacle,
          source,
          obstacles[i].dest,
          origin,
          obstacles[i].rotation,
          WHITE
      );
}
      EndMode2D();
    EndDrawing();
  }
  UnloadMusicStream(soundtrack);
  CloseAudioDevice();
  CloseWindow();
  return EXIT_SUCCESS;
}
