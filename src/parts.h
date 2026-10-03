#ifndef PARTS_H
#define PARTS_H

typedef struct {
  Vector2 pos;
  Vector2 size;
  Color BackColor;
  Color fontColor;
  const char* text;
  const char* caption;
  void (*callback) (void);
} Button;

void DrawSimpleButton(Button b);
void checkClick(Button b);

#endif
