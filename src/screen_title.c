#include "raylib.h"
#include "screens.h"

void InitTitleScreen(void) {
  currentScreen = TITLE;
}

void UpdateTitleScreen(void) {
  DrawTitleScreen();
  if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
    currentScreen = GAMEPLAY;
    InitGameScreen();
  }
}

void DrawTitleScreen(void) {
  ClearBackground(BLUE);
  Vector2 fpos = { WIDTH / 2, HEIGHT / 2};
  DrawTextEx(font2, "Hello World\n", fpos, 55, 15, YELLOW);
}
