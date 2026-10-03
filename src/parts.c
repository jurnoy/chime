#include "raylib.h"
#include "parts.h"
#include "screens.h"


void DrawSimpleButton(Button b) {
  int font_size = b.size.x * 0.3;
  int text_size = MeasureText(b.text, font_size);
  Vector2 text_rec = { text_size * 2, font_size * 2};
  Rectangle rec = {b.pos.x + (b.size.x - text_rec.x) / 2.0, b.pos.y + (b.size.y - text_rec.y) / 2.0, text_rec.x, text_rec.y};
  if (CheckCollisionPointRec(GetMousePosition(), rec)) {
    // Draw outline
    DrawRectangle(b.pos.x, b.pos.y, b.size.x, b.size.y, b.fontColor);
    DrawRectangle(b.pos.x + b.size.x * 0.05, b.pos.y + b.size.y * 0.05, b.size.x * 0.9, b.size.y * 0.9, b.BackColor);

    // Draw icon
    DrawText(b.text, b.pos.x + (b.size.x - text_size) / 2.0 , b.pos.y + (b.size.y - font_size) / 2.0, font_size, b.fontColor);

    // Draw caption
    int caption_size = 20;
    int caption_space = 3;
    Vector2 caption_measure = MeasureTextEx(font2, b.caption, caption_size, caption_space);
    Vector2 caption_vector = {b.pos.x + (b.size.x - caption_measure.x) / 2.0, b.pos.y + b.size.y * 0.8 - caption_measure.y / 2.0};
    DrawTextEx(font2, b.caption, caption_vector, caption_size, caption_space, b.fontColor);
  }
  else {
    DrawText(b.text, b.pos.x + (b.size.x - text_size) / 2.0 , b.pos.y + (b.size.y - font_size) / 2.0, font_size, ColorAlpha(b.fontColor, 0.50));
  }
};

void checkClick(Button b) {
  int font_size = b.size.x * 0.3;
  int text_size = MeasureText(b.text, font_size);
  Vector2 text_rec = { text_size * 2, font_size * 2};
  Rectangle rec = {b.pos.x + (b.size.x - text_rec.x) / 2.0, b.pos.y + (b.size.y - text_rec.y) / 2.0, text_rec.x, text_rec.y};
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(GetMousePosition(), rec)) {
      b.callback();
    };
  }
}
