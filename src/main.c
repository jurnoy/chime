#include "raylib.h"
#include "screens.h"

GameScreen currentScreen = TITLE;
Font font = { 0 };
Font font2 = { 0 };

int WIDTH = 840;
int HEIGHT = 840;

static void UpdateDrawFrame(void);

int main(void) {
  InitWindow(WIDTH, HEIGHT, "Chime");
  font = LoadFont("assets/Raleway-Regular.ttf");
  font2 = LoadFont("assets/Ubuntu-Regular.ttf");

  
  SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose())    // Detect window close button or ESC key
  {
    BeginDrawing();
    UpdateDrawFrame();
    EndDrawing();
  }
  
  CloseWindow();
}

void UpdateDrawFrame(void) {
  switch (currentScreen) {
    case TITLE: { UpdateTitleScreen(); } break;
    case GAMEPLAY: { UpdateGameScreen(); } break;
    default: break;
  }
}
