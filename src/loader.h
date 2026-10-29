#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>

void loadWordleWord(const char* filename, long seed);
int load_colors(FILE *file);

int xoshiroCielInt(long seed, int ciel);
long timeSeed(void);

#endif
 