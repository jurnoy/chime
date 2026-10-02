#include "raylib.h"
#include "screens.h"
#include "confs.h"

int WIDTH = 840;
int HEIGHT = 840;

// General Data
// ------------------------------------------

GameScreen currentScreen = TITLE;
Font font2 = { 0 };

// ------------------------------------------

// Game Data
// ------------------------------------------
Font font = { 0 };

const Color FONT_COLOR = { 0xe8, 0xe6, 0xe3, 0xff };
const Color WRONG_COLOR = { 0x5d, 0x64, 0x68, 0xff };
const Color BACKROUND_COLOR = { 0x18, 0x1a, 0x1b, 0xff };
const Color MISPLACED_COLOR = { 0x68, 0x5b, 0x22, 0xff };
const Color CORRECT_COLOR = { 0x57, 0x7e, 0x45, 0xff };
const Color OUTLINE_COLOR = { 0x3b, 0x40, 0x43, 0xff };

// ------------------------------------------

static void UpdateDrawFrame(void);

int main(void) {

  // Initializaiton
  // ------------------------------------------
  InitWindow(WIDTH, HEIGHT, "Chime");
  SetTargetFPS(60);   // Set our game to run at 60 frames-per-second

  InitTitleScreen();

  // ------------------------------------------


  // Loading Fonts
  // ------------------------------------------
  font = LoadFontEx("assets/Raleway-Regular.ttf", 140 * 0.4, 0, 0);
  font2 = LoadFontEx("assets/Ubuntu-Regular.ttf", 55, 0, 0);

  // ------------------------------------------


  // Main game loop
  // ------------------------------------------
  while (!WindowShouldClose())    // Detect window close button or ESC key
  {
    BeginDrawing();
    UpdateDrawFrame();
    EndDrawing();
  }
  // ------------------------------------------

  CloseWindow();
}

void UpdateDrawFrame(void) {
  switch (currentScreen) {
    case TITLE: { UpdateTitleScreen(); } break;
    case GAMEPLAY: { UpdateGameScreen(); } break;
    case ENDING: { UpdateEndingScreen(); } break;
    default: break;
  }
}
