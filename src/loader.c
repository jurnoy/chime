#include "stdio.h"
#include "stdint.h"
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
