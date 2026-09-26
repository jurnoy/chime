#ifndef SCREENS_H
#define SCREENS_H

typedef enum GameScreen { LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

extern GameScreen currentScreen;
extern Font font;
extern Font font2;
extern int WIDTH;
extern int HEIGHT;
extern Vector2 BOX_SIZE;

void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);

void InitGameScreen(void);
void UpdateGameScreen(void);
void DrawGameScreen(void);

#endif // SCREENS_H