#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "raylib.h"
#include "confs.h"
#include "loader.h"
#include "game_state.h"

extern char correct[5];

void loadWordleWord(const char* fpname, long seed) {
  FILE *fp = fopen(fpname, "rb");
  char word[6] = "HELLO";
  for (int i = 0; i < 5; i++) {
    correct[i] = word[i];
  }

  if (fp == NULL) {
      perror("Unable to open file");
      return;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
      perror("fseek failed");
      fclose(fp);
      return;
  }

  long fp_size = ftell(fp);

  if (fp_size == -1L) {
      perror("ftell failed");
      fclose(fp);
      return;
  }

  int target = xoshiroCielInt(seed, fp_size / 6.0);

  fseek(fp, (target - 1) * 6, SEEK_SET);
  fgets(word, 6, fp);

  for (int i = 0; i < 5; i++) {
    if (word[i] >= 'a' && word[i] <= 'z') {
      correct[i] = word[i] - 'a' + 'A';
    }
    else correct[i] = word[i];
  }
}

int load_colors(FILE *file) {
    char buf[50];
    char *type;
    long int val;

    while (fgets(buf, sizeof buf, file) != NULL) {
        type = strtok(buf, " = ");
        val = strtol(strtok(NULL, " = "), NULL, 16);
        if (strcmp(type, "BACKGROUND") == 0) BACKROUND_COLOR = GetColor(val);
        else if (strcmp(type, "CORRECT") == 0) CORRECT_COLOR = GetColor(val);
        else if (strcmp(type, "MISPLACED") == 0) MISPLACED_COLOR = GetColor(val);
        else if (strcmp(type, "WRONG") == 0) WRONG_COLOR = GetColor(val);
        else if (strcmp(type, "FONT") == 0) FONT_COLOR = GetColor(val);
        else if (strcmp(type, "OUTLINE") == 0) OUTLINE_COLOR = GetColor(val);
        else if (strcmp(type, "BUTTON") == 0) BUTTON_COLOR = GetColor(val);
    }
    return 0;
}
