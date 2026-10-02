#include "raylib.h"
#include "screens.h"
#include "game_state.h"

extern END end;
char correct[5];
Color cl;
const char* cheer_text;

void InitEndingScreen(void) {
  switch (end) {
    case WIN: {
      cl = GREEN;
      cheer_text = "You won!";
    } break;
    case LOSS: {
      cl = RED;
      cheer_text = correct;
    } break;
    case ABORTED: {
      cl = GRAY;
      cheer_text = correct;
    } break;
    default: {
      cl = PINK;
      cheer_text = "Sorry!";
    } break;
  }
};

void UpdateEndingScreen(void) {
  DrawEndingScreen();
  if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    currentScreen = TITLE;
    end = PAUSED;
  }
};

void DrawEndingScreen(void) {
  ClearBackground(cl);
  int font_size = 55;
  DrawText(cheer_text, (GetScreenWidth() - MeasureText(cheer_text, font_size)) / 2.0, (GetScreenHeight() - font_size) / 2.0, font_size, BLACK);
};
