/*
 *  Game.h - Game Logic Class
 *
 * 
 *  This class implements the game logic:
 *
 *  - start a new game
 *  - game over and pause
 *  - select the next shape
 *  - settle a shape and calculate lines
 *  - calculate score
 *  - play sound effects
 *  - draw the whole screen (when playing the game)
 *  - collission detection
 *
 */

#ifndef NOSOUND
#include <SDL_mixer.h>
#endif

#ifndef _GAME_H
#define _GAME_H

#include "Shape.h"
#include "SdlDraw.h"
#include "HighScore.h"
#include "BFont.h"
#include "resources.h"
#include "stuff.h"

#define DEFAULT_TINTERVAL 1000

class Game : public SdlDrawExt
{
  private:

    Shape *shape, *shapeNext;
    
    int score;
    int downscore;   // count how many times the player pressed down
    int lines;
    int level;
    int posx;
    int posy;
    int soundChannel;
    bool paused;
    bool gameOver;
    bool shapeSettled;
    HighScore *highscore;
    Uint32 timerInterval;
    SDL_TimerID timerID;

    int field[FIELD_H][FIELD_W];

    SDL_Surface *gameOverPic;
    SDL_Surface *pausedPic;

    /* shape movement */
    void updateShapePos(void);
    void settleShape(void);
    void addScore(int lineCount);
    void gravity(int posy, int lineCount);
    
    /* collision detection */
    bool collLeft(void);
    bool collRight(void);
    bool shapeOverlapLeft(void);
    bool shapeOverlapRight(void);
    bool collBottom(void);
    bool collBlock(int posx, int posy);

    int detectLines(void);
    void selectNextShape(void);
    
  public:

    Game(SDL_Surface *screen, Shape *shape, Shape *shapeNext);
    ~Game();

    void draw(void);

    void startNewGame(void);
    void initShapePos(void);
    void setTimer(void);
    static Uint32 fallCallback(Uint32 interval, void *object);
    void setScore(int);
    int  getScore(void);
    void setLines(int);
    int  getLines(void);
    void setLevel(int);
    int  getLevel(void);
    void togglePause(void);
    bool isOver() {return this->gameOver;}
    bool isSettled() {return this->shapeSettled;}

    /* shape movement */
    bool moveShapeLeft(void);
    bool moveShapeRight(void);
    bool moveShapeDown(void);
    bool rotateShapeLeft(void);
    bool rotateShapeRight(void);
};

#endif
