#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include "BFont.h"

#ifndef NOSOUND
#include <SDL_mixer.h>
#endif

#ifndef _MAIN_H
#define _MAIN_H

/* include some project stuff */
#include "stuff.h"
#include "util.h"
#include "FpsCounter.h"
#include "resources.h"

/* Game Classes */
#include "GameMenu.h"
#include "Game.h"
#include "Shape.h"
#include "BloxConf.h"
#include "HighScore.h"

/* global variables */

BloxConf *gConf;

SDL_Surface *gScreen;     /* the video framebuffer */
SDL_Surface *gIngameBg;
SDL_Surface *gBgImage;
SDL_Rect src, dest;       /* temporary SDL_Rects; can be used by any function */
HighScore *gHighscore;
int gGameState = 0;

GameMenu *gMenu;
Game *gGame;
Shape *gShape;
Shape *gShapeNext;
SDL_Joystick *gJoystick = NULL;

/* game states */
#define GAME_INTRO      1
#define GAME_MENU       2
#define GAME_CREDITS    3
#define GAME_HIGHSCORES 4
#define GAME_PLAY       5
#define GAME_EXIT       6

void drawBackground(void);
void drawScreen(void);

/* initialization functions */
#ifndef NOSOUND
void initAudio(void);
#endif

void initGraphics(void);
void initMisc(void);

/* cleanup functions */
#ifndef NOSOUND
void freeAudio(void);
#endif

void freeGraphics(void);
void freeMisc(void);
void cleanUp(void);

void handleInput(void);
Uint32 fallCallback(Uint32 interval, void *param);
void playGame(void);
void quitGame(void);
void initCreditPos(int *positions, const char **credits, int strings, int fontHeight);
void showCredits(void);
void showHighscores(void);

#endif
