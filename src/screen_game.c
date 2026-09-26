#include "raylib.h"
#include "stdint.h"
#include <raymath.h>
#include "screens.h"

const Color FONT_COLOR = { 0xe8, 0xe6, 0xe3, 0xff };
const Color WRONG_COLOR = { 0x5d, 0x64, 0x68, 0xff };
const Color BACKROUND_COLOR = { 0x18, 0x1a, 0x1b, 0xff };
const Color MISPLACED_COLOR = { 0x68, 0x5b, 0x22, 0xff };
const Color CORRECT_COLOR = { 0x57, 0x7e, 0x45, 0xff };
const Color OUTLINE_COLOR = { 0x3b, 0x40, 0x43, 0xff };

Vector2 BOX_SIZE = { 140, 140 };
char text[5];
int textIndex;
int turn;
int end;
char correct[5];

// Boxes that will contain the letters
// ------------------------------------------
typedef struct {
  Vector2 pos;
  char letter;
  Color color;
  int outline;
} Box;

void DrawBox(Box box) {
  DrawRectangleV(box.pos, BOX_SIZE, BACKROUND_COLOR);
  DrawRectangleV(Vector2Add(box.pos, Vector2Scale(BOX_SIZE, .05)), Vector2Scale(BOX_SIZE, 0.90), box.color);
  if (box.outline == 1) {
    DrawRectangleV(Vector2Add(box.pos, Vector2Scale(BOX_SIZE, 0.1)), Vector2Scale(BOX_SIZE, 0.80), BACKROUND_COLOR);
  }
  if (box.letter != 0) {
    char text[2];
    text[0] = box.letter;
    DrawTextEx(font, text, Vector2Add(box.pos, Vector2Scale(BOX_SIZE, 0.5)), BOX_SIZE.x * 0.4, 15, FONT_COLOR);
  }
}

// ------------------------------------------
static Box Boxes[30];

// Checking the word input
// ------------------------------------------

void CheckUpdateBox(char intext[5]) {
  uint8_t cmask = 0;
  int should_end = 1;

  for (int i = 0; i < 5; i++) {
    if (intext[i] == correct[i]) {
      Boxes[turn * 5 + i].color = CORRECT_COLOR;
      Boxes[turn * 5 + i].outline = 0;
      cmask |= 1 << i;
    }
    else {
      Boxes[turn * 5 + i].color = WRONG_COLOR;
      should_end = 0;
    };
  }
  
  for (int i = 0; i < 5; i++) {
    // Handing Misplaced
    for (int n = 0; n < 5; n++) {
      if (n == i) continue;
      
      // Checking if the letter is already used to mark
      if ((cmask & (1 << n)) == 0 && (Boxes[turn * 5 + i].outline != 0)) {
        if (intext[i] == correct[n]) {
          Boxes[turn * 5 + i].color = MISPLACED_COLOR;
          cmask |= 1 << n;
          continue;
        }
      }
    }
    Boxes[turn * 5 + i].outline = 0;
  }
  end = should_end;
}

void InitGameScreen(void) {
  char cr[6] = "HELLO";
  for (int i = 0; i < 5; i++) {
    if (cr[i] >= 'a' && cr[i] <= 'z') {
      correct[i] = cr[i] + 'A' - 'a';
    }
    else {
      correct[i] = cr[i];
    }
  }

  end = 0;
  textIndex = 0;
  turn = 0;
  for (int i = 0; i < 5; i++) {
    text[i] = 0;
  }
  for (int y = 0; y < 6 ;y++) {
    for (int x = 0; x < 5 ;x++) {
      Vector2 pos = { BOX_SIZE.x * x, BOX_SIZE.y * y};
      Box tmpBox = { pos, 0, OUTLINE_COLOR, 1 };
      Boxes[y * 5 + x] = tmpBox;
    }
  }
};

void UpdateGameScreen(void) {
  DrawGameScreen();

  // Checks if game has ended
  if ((end == 1) || (turn >= 6)) return;
  
  if (text[4] == 0){
    int c = GetCharPressed();
    if (c != 0) {
      if (c >= 'A' && c <= 'Z') {
        text[textIndex] = c;
        Boxes[turn * 5 + textIndex].letter = c;
        textIndex += 1;
      }
      else if (c >= 'a' && c <= 'z') {
        c = c + 'A' - 'a';
        text[textIndex] = c;
        Boxes[turn * 5 + textIndex].letter = c;
        textIndex += 1;
      }
    }
  }
  if (IsKeyPressed(KEY_ENTER) && (text[4] != 0)) {
    CheckUpdateBox(text);
    for (int i = 0; i < 5; i++) {
      text[i] = 0;
    }
    textIndex = 0;
    turn += 1;
  }

  if (IsKeyPressed(KEY_BACKSPACE) && (text[0] != 0)) {
    textIndex -= 1;
    text[textIndex] = 0;
    Boxes[turn * 5 + textIndex].letter = 0;
  }

};

void DrawGameScreen(void) {
  ClearBackground(BACKROUND_COLOR);
  for (int i = 0; i < 30 ; i++) {
    DrawBox(Boxes[i]);
  }
};
