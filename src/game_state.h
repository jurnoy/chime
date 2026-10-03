#ifndef GAME_STATE_H
#define GAME_STATE_H

// Game State
// ------------------------------------------
typedef enum { WIN, LOSS, PLAYING, ABORTED, PAUSED } END;
extern END end;

// ------------------------------------------


// Game data
// ------------------------------------------
extern char correct[5];
extern int turn;

// ------------------------------------------


#endif
