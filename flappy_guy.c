#include <raylib.h>
#include <stdio.h>

#define PIPES_COUNT 100
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

static void initGame(void);
static void updateGame(void);
static void drawMenu(void);
static int  closeGame(void);

static Flapper flapper                = {0};
static Pipes   pipes[PIPES_COUNT*2]   = {0};

static Texture2D flapperTexture;
static Texture2D flapperTexture2;
static Texture2D pipesTexture;
static Texture2D pipesTexture2;
static Texture2D cloudTexture;

static Camera2D camera;

int main (void)
{
  InitWindow(WINWIDTH, WINHEIGHT, "Flappy Guy");
  SetTargetFPS(60);

  flapperTexture  = LoadTexture("./assets/flapper1.png");
  flapperTexture2 = LoadTexture("./assets/flapper2.png");
  pipesTexture    = LoadTexture("./assets/obstacle.png");
  pipesTexture2   = LoadTexture("./assets/obstacle2.png");
  cloudTexture    = LoadTexture("./assets/cloud1.png");

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
  // score = 0;

  flapper.pos = (Vector2){25, 100};
  flapper.rec.x      = flapper.pos.x;
  flapper.rec.y      = flapper.pos.y;
  flapper.rec.width  = flapperTexture.width/7;
  flapper.rec.height = flapperTexture.height/5;
  flapper.tex = flapperTexture;

  for (int i = 0; i < PIPES_COUNT/2; i++)
  {
    pipes[i].x = i * 200;
    pipes[i].y = GetRandomValue(WINHEIGHT-550, WINHEIGHT-200);
    pipes[i].rec.x = pipes[i].x + 260.0f;
    pipes[i].rec.y = pipes[i].y + 240.0f;
    pipes[i].rec.width = pipesTexture.width/4;
    pipes[i].rec.height = pipesTexture.height/2;
    pipes[i].tex = pipesTexture;
    pipes[i].tex.width  *= 2;
    pipes[i].tex.height *= 2;
  }

  for (int i = PIPES_COUNT/2, j = 0; i < PIPES_COUNT; i++, j++)
  {
    pipes[i].x = j * 200;
    pipes[i].y = GetRandomValue(-200, 0);
    pipes[i].rec.x = pipes[i].x + 300.0f;
    pipes[i].rec.y = pipes[i].y + 240.0f;
    pipes[i].rec.width = pipesTexture2.width/4;
    pipes[i].rec.height = pipesTexture2.height/2;
    pipes[i].tex = pipesTexture2;
    pipes[i].tex.width  *= 2;
    pipes[i].tex.height *= 2;
  }

  camera.offset.x = WINWIDTH/2;
  camera.offset.y = WINHEIGHT/2;
  camera.target.y = WINHEIGHT/2;
  camera.rotation = 0;
  camera.zoom     = 1.0f;
}

void flapperMovement()
{
  static float velocity        = 0.0f;
  static const float gravity   = 0.5f;
  static const float jumpForce = -10.0f;

  flapper.pos.x += 3;

  if(IsKeyPressed(KEY_SPACE))
  {
    velocity = jumpForce;
  }

  if(IsKeyDown(KEY_SPACE))
  {
    flapper.tex = flapperTexture2;
    velocity      += gravity/2;
    flapper.pos.y += velocity/8;
    flapper.rec.x  = flapper.pos.x + 100;
    flapper.rec.y  = flapper.pos.y + 70;
  }
  else
  {
    flapper.tex = flapperTexture;
    velocity      += gravity;
    flapper.pos.y += velocity;
    flapper.rec.x  = flapper.pos.x + 100;
    flapper.rec.y  = flapper.pos.y + 70;
  }
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
    BeginMode2D(camera);
    ClearBackground(SKYBLUE);
    DrawTexture(flapper.tex, flapper.pos.x, flapper.pos.y, WHITE);

    for (int i = 0; i < PIPES_COUNT; i++)
    {
      DrawTexture(pipes[i].tex, pipes[i].x, pipes[i].y, WHITE);
    }

    EndMode2D();
  EndDrawing();
}

void updateGame()
{
  if (playing)
  {
    flapperMovement();
    drawGame();
    camera.target.x = flapper.pos.x + 500;
    checkCollision();
    if(IsKeyPressed(KEY_P)) playing = false;
  }
  else
  {
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
  static int menuCount   = 2;

  if(IsKeyPressed(KEY_DOWN)){
    currentItem ++;
    if(currentItem >= menuCount) currentItem = 0;
  }

  if(IsKeyPressed(KEY_UP)){
    currentItem --;
    if(currentItem < 0) currentItem = menuCount - 1;
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

  if(currentItem == 1 && IsKeyPressed(KEY_ENTER)) closeGame();

  BeginDrawing();
  
    ClearBackground(SKYBLUE);
    DrawText("PLAY",     WINWIDTH/2, WINHEIGHT/4, 50, currentItem == 0 ? WHITE : BLACK);
    DrawText("QUIT",     WINWIDTH/2, WINHEIGHT/2, 50, currentItem == 1 ? WHITE : BLACK);

  EndDrawing();
}
