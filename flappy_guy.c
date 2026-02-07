#include <raylib.h>
#include <stdio.h>

#define PIPES_COUNT 10
#define WINWIDTH    1200
#define WINHEIGHT   800

typedef struct Flapper {
  Vector2   pos;
  Rectangle rec;
  Texture2D tex;
} Flapper;

typedef struct Pipes {
  int x;
  int y;
  Rectangle rec;
  Texture2D tex;
} Pipes;

static bool playing;
static bool shouldQuit;
static bool shouldPause;

static void initGame(void);
static void updateGame(void);
static void drawMenu(void);
static int  closeGame(void);

static Flapper flapper               = {0};
static Pipes   pipes[PIPES_COUNT*2]    = {0};

static Texture2D flapperTexture;
static Texture2D flapperTexture2;
static Texture2D pipesTexture;
static Texture2D cloudTexture;

int main (void)
{
  InitWindow(WINWIDTH, WINHEIGHT, "Flappy Guy");
  SetTargetFPS(60);

  flapperTexture = LoadTexture("./assets/flapper1.png");
  flapperTexture2 = LoadTexture("./assets/flapper2.png");
  pipesTexture   = LoadTexture("./assets/obstacle.png");
  cloudTexture   = LoadTexture("./assets/cloud1.png");

  initGame();

  while(!WindowShouldClose())
  {
    updateGame();
  }
  closeGame();
}

void initGame()
{
  playing = true;

  // ----------------------------
  // initialise flapper and pipes
  // ----------------------------
  flapper.pos = (Vector2){25, 100};
  flapper.rec.x      = flapper.pos.x;
  flapper.rec.y      = flapper.pos.y;
  flapper.rec.width  = flapperTexture.width/7;
  flapper.rec.height = flapperTexture.height/5;
  flapper.tex = flapperTexture;

  for (int i = 0; i < PIPES_COUNT; i++)
  {
    pipes[i].x = i * 200;
    pipes[i].y = GetRandomValue(WINHEIGHT-300, WINHEIGHT);
    pipes[i].rec.x = pipes[i].x + 130.0f;
    pipes[i].rec.y = pipes[i].y + 120.0f;
    pipes[i].rec.width = pipesTexture.width/8;
    pipes[i].rec.height = pipesTexture.height/4;
    pipes[i].tex = pipesTexture;
  }

}

void flapperMovement()
{
  static float velocity        = 0.0f;
  static const float gravity   = 0.5f;
  static const float jumpForce = -10.0f;

  flapper.pos.x += 1;

  if(IsKeyPressed(KEY_SPACE)) velocity = jumpForce;

  if(IsKeyDown(KEY_SPACE)) {
    flapper.tex = flapperTexture2;
  } else {
    flapper.tex = flapperTexture;
  }

  velocity      += gravity;
  flapper.pos.y += velocity;
  flapper.rec.x  = flapper.pos.x + 100;
  flapper.rec.y  = flapper.pos.y + 70;
}

void checkCollision()
{
  if(flapper.rec.y > WINHEIGHT || flapper.rec.y < 0)
  {
    playing = false;
  }
  for (int i = 0; i < PIPES_COUNT; i++){
    if(CheckCollisionRecs(flapper.rec, pipes[i].rec))
      playing = false;
  }
}

void drawGame()
{
  BeginDrawing();

    ClearBackground(SKYBLUE);

    DrawTexture(flapper.tex, flapper.pos.x, flapper.pos.y, WHITE);
    DrawRectangleLines(flapper.rec.x, flapper.rec.y, flapper.rec.width, flapper.rec.height, BLACK);

    for (int i = 0; i < PIPES_COUNT; i++)
    {
      DrawTexture(pipes[i].tex, pipes[i].x, pipes[i].y, WHITE);
      DrawRectangleLines(pipes[i].rec.x, pipes[i].rec.y, pipes[i].rec.width, pipes[i].rec.height, BLACK);
    }

  EndDrawing();
}

void updateGame()
{
  if(playing)
  {
    flapperMovement();
    drawGame();
    checkCollision();
    if(IsKeyPressed(KEY_P)) playing = false;
  } 
  else{
    drawMenu();
  }
}

int closeGame()
{
  UnloadTexture(pipesTexture);
  UnloadTexture(cloudTexture);
  UnloadTexture(flapperTexture);
  UnloadTexture(flapperTexture2);
  CloseWindow();
  return 0;
}

void drawMenu(){
  static int currentItem = 0;
  static int menuCount   = 3;

  if(IsKeyPressed(KEY_DOWN)){
    currentItem ++;
    if(currentItem >= menuCount) currentItem = 0;
  }

  if(IsKeyPressed(KEY_UP)){
    currentItem --;
    if(currentItem<0) currentItem = menuCount - 1;
  }

  if(currentItem == 0 && IsKeyPressed(KEY_ENTER))
  {
    if(playing)
    {
      drawGame();
    }else {
      initGame();
    }
  }

  if(currentItem == 2 && IsKeyPressed(KEY_ENTER)) closeGame();

  BeginDrawing();
  
    ClearBackground(SKYBLUE);
    DrawText("PLAY",     WINWIDTH/2, WINHEIGHT/4, 50, currentItem == 0 ? WHITE : BLACK);
    DrawText("SETTINGS", WINWIDTH/2, WINHEIGHT/3, 50, currentItem == 1 ? WHITE : BLACK);
    DrawText("QUIT",     WINWIDTH/2, WINHEIGHT/2, 50, currentItem == 2 ? WHITE : BLACK);

  EndDrawing();
}
