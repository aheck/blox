#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_mixer.h>

#ifndef _RESOURCES_H
#define _RESOURCES_H

#include "BFont.h"
#include "util.h"

#define DATA_PREFIX "data/"

/* get macros */
#define getResPic(id) gPics[id]
#define getResMusic(id) gMusic[id]
#define getResSfx(id) gSfx[id]
#define getResFont(id) gFonts[id]

/* pic enumeration */
enum
{
  MPIC_BACKGROUND,
  MPIC_NEWGAME,
  MPIC_HIGHSCORE,
  MPIC_CREDITS,
  MPIC_QUIT,
  MPIC_ANEWGAME,
  MPIC_AHIGHSCORE,
  MPIC_ACREDITS,
  MPIC_AQUIT,
  GPIC_INGAME,
  GPIC_GAMEOVER,
  GPIC_PAUSED,
  NUM_PICS
};

/* music enumeration */
enum
{
  MUSIC_TRACK01,
  MUSIC_TRACK02,
  NUM_MUSIC
};

/* sfx enumeration */
enum
{
  SFX_CLICK,
  SFX_ROTLEFT,
  SFX_ROTRIGHT,
  SFX_FALL,
  SFX_DELETE,
  NUM_SFX
};

/* font enumeration */
enum
{
  FONT_FONT1,
  FONT_FONT2,
  NUM_FONTS
};

/* menu pics array */
extern SDL_Surface* gPics[NUM_PICS];
/* music array */
extern Mix_Music* gMusic[NUM_MUSIC];
/* sfx array */
extern Mix_Chunk* gSfx[NUM_SFX];
/* font array */
extern BFont_Info* gFonts[NUM_FONTS];

void resLoadPics(void);
void resLoadMusic(void);
void resLoadSfx(void);
void resLoadFonts(void);

void resFreePics(void);
void resFreeMusic(void);
void resFreeSfx(void);
void resFreeFonts(void);

#endif
