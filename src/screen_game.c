#include "raylib.h"
#include "raymath.h"
#include "stdint.h"
#include "stdio.h"

#include "screens.h"
#include "confs.h"
#include "parts.h"
#include "loader.h"
#include "game_state.h"

// Global Game State and Data
// ------------------------------------------
extern END end;
extern char correct[5];

Vector2 BOX_SIZE = { 140, 140 };

// ------------------------------------------


// Local Game Data
// ------------------------------------------
char text[5];
int textIndex;
int turn;

// ------------------------------------------


// Boxes and Buttons
// ------------------------------------------

// Boxes that will contain the letters
typedef struct {
  Vector2 pos;
  char letter;
  Color color;
  int outline;
} Box;

void DrawBox(Box box);
Box Boxes[30];

// Doing buttons
int total_buttons = 1;
Button Buttons[2];

// Abort button
void abortGame(void);
void createAbortButton(Button *b);

// Back button
void backToTitle(void);
void createBackButton(Button *b);

// Checking the word input
void CheckUpdateBox(char intext[5]);
// ------------------------------------------

void InitGameScreen(void) {
  createAbortButton(&Buttons[0]);
  createBackButton(&Buttons[1]);
  
  // Setting the correct word
  long seed = timeSeed();
  loadWordleWord("assets/words.txt", seed);
   
  end = PLAYING;
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

void checkShortcuts(void) {
  if (IsKeyPressed(KEY_DELETE)) {
    currentScreen = TITLE;
  }
}

void DrawGameScreen(void) {
  ClearBackground(BACKROUND_COLOR);
  for (int i = 0; i < 30 ; i++) {
    DrawBox(Boxes[i]);
  }
  for (int i = total_buttons; i >= 0; i--) {
    DrawSimpleButton(Buttons[i]);
    checkClick(Buttons[i]);
  }
};


void UpdateGameScreen(void) {
  DrawGameScreen();
  checkShortcuts();

  // Checks if game has ended
  if (turn >= 6) {
    end = LOSS;
  };
  if (end == WIN || end == LOSS || end == ABORTED) {
    currentScreen = ENDING;
    InitEndingScreen();
  };
  
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

void DrawBox(Box box) {
  DrawRectangleV(box.pos, BOX_SIZE, BACKROUND_COLOR);
  DrawRectangleV(Vector2Add(box.pos, Vector2Scale(BOX_SIZE, .05)), Vector2Scale(BOX_SIZE, 0.90), box.color);
  if (box.outline == 1) {
    DrawRectangleV(Vector2Add(box.pos, Vector2Scale(BOX_SIZE, 0.1)), Vector2Scale(BOX_SIZE, 0.80), BACKROUND_COLOR);
  }
  if (box.letter != 0) {
    char text[2];
    text[0] = box.letter;
    text[1] = '\0';
    DrawTextEx(font, text, Vector2Add(box.pos, Vector2AddValue(Vector2Scale(BOX_SIZE, 0.5), -MeasureText(text, BOX_SIZE.x * 0.4) / 2.0)), BOX_SIZE.x * 0.4, 15, FONT_COLOR);
  }
}

void abortGame(void) {
  end = ABORTED;
}

void createAbortButton(Button *b) {
  b->BackColor = BACKROUND_COLOR;
  b->fontColor = RED;
  b->text = "<|";
  b->caption = "Abort";
  Vector2 b_pos = {GetScreenWidth() - BOX_SIZE.x, 0};
  b->pos = b_pos;
  b->size = BOX_SIZE;
  b->callback = abortGame;
}


void backToTitle(void) {
  end = PAUSED;
  currentScreen = TITLE;
};

void createBackButton(Button *b) {
  b->BackColor = BACKROUND_COLOR;
  b->fontColor = RED;
  b->text = "<<";
  b->caption = "Back";
  Vector2 b_pos = {GetScreenWidth() - BOX_SIZE.x, BOX_SIZE.y};
  b->pos = b_pos;
  b->size = BOX_SIZE;
  b->callback = backToTitle;
};


void CheckUpdateBox(char intext[5]) {
  uint8_t cmask = 0;
  END should_end = WIN;
  
  for (int i = 0; i < 5; i++) {
    if (intext[i] == correct[i]) {
      Boxes[turn * 5 + i].color = CORRECT_COLOR;
      Boxes[turn * 5 + i].outline = 0;
      cmask |= 1 << i;
    }
    else {
      Boxes[turn * 5 + i].color = WRONG_COLOR;
      should_end = PLAYING;
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
