#ifndef SCREENS_H
#define SCREENS_H

// Application State
// ------------------------------------------
typedef enum {
  TITLE = 0, 
  GAMEPLAY, 
  ENDING
} GameScreen;

extern GameScreen currentScreen;

// ------------------------------------------


// General Application configs
// ------------------------------------------
extern int WIDTH;
extern int HEIGHT;

extern Font font;
extern Font font2;

extern Vector2 BOX_SIZE;

// ------------------------------------------

// Title Screen Function Calls
// ------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);

// ------------------------------------------


// Game Screen Function Calls
// ------------------------------------------
void InitGameScreen(void);
void UpdateGameScreen(void);
void DrawGameScreen(void);

// ------------------------------------------


// Ending Screen Function Calls
// ------------------------------------------
void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);

// ------------------------------------------

#endif // SCREENS_H
