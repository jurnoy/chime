#include "raylib.h"
#include "screens.h"

Texture2D logo;
Texture2D logo1;
Texture2D logo2;

void InitTitleScreen(void) {
  currentScreen = TITLE;
  logo1 = LoadTexture("assets/logo.png");
  logo2 = LoadTexture("assets/lighthouse.png");
  logo = logo1;
}

void UpdateTitleScreen(void) {
  DrawTitleScreen();
  if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
    currentScreen = GAMEPLAY;
    InitGameScreen();
  }
  if (IsKeyPressed(KEY_UP)) {
    logo = logo1;
  }
  if (IsKeyPressed(KEY_DOWN)) {
    logo = logo2;
  }
}

void DrawTitleScreen(void) {
  ClearBackground(GetColor(0xc18c5dff));
  DrawCircle(GetScreenWidth() / 2.0, GetScreenHeight() / 2.0, logo.width, RAYWHITE);
  DrawTexture(logo, (GetScreenWidth() - logo.width) / 2.0, (GetScreenHeight() - logo.height) / 2.0, WHITE);
  char text[] = "Click to Start.";
  Vector2 text_size = MeasureTextEx(font2, text, 35, 15);
  Vector2 text_pos = {(GetScreenWidth() - text_size.x) / 2.0, GetScreenHeight() * 0.9 - text_size.y / 2.0};
  DrawTextEx(font2, text, text_pos, 35, 15, BLACK);
}
