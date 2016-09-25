#include "resources.h"

/* menu pics array */
SDL_Surface *gPics[NUM_PICS] = {NULL};
/* music array */
Mix_Music* gMusic[NUM_MUSIC] = {NULL};
/* sfx array */
Mix_Chunk* gSfx[NUM_SFX] = {NULL};
/* font array */
BFont_Info* gFonts[NUM_FONTS] = {NULL};

/* pic filenames */
static const char* gPicFiles[NUM_PICS] = {
  DATA_PREFIX "pics/intro.png",
  DATA_PREFIX "pics/menu/menu_newgame.png",
  DATA_PREFIX "pics/menu/menu_highscore.png",
  DATA_PREFIX "pics/menu/menu_credits.png",
  DATA_PREFIX "pics/menu/menu_quit.png",
  DATA_PREFIX "pics/menu/menua_newgame.png",
  DATA_PREFIX "pics/menu/menua_highscore.png",
  DATA_PREFIX "pics/menu/menua_credits.png",
  DATA_PREFIX "pics/menu/menua_quit.png",
  DATA_PREFIX "pics/ingame.png",
  DATA_PREFIX "pics/gameover.png",
  DATA_PREFIX "pics/paused.png"
};

/* music filenames */
static const char* gMusicFiles[NUM_MUSIC] = {
  DATA_PREFIX "music/track01.mod",
  DATA_PREFIX "music/track02.mod"
};

/* sfx filenames */
static const char* gSfxFiles[NUM_SFX] = {
  DATA_PREFIX "sfx/click.wav",
  DATA_PREFIX "sfx/left.wav",
  DATA_PREFIX "sfx/right.wav",
  DATA_PREFIX "sfx/fall.wav",
  DATA_PREFIX "sfx/delete.wav"
};

/* font filenames */
static const char* gFontFiles[NUM_FONTS] = {
  DATA_PREFIX "fonts/font1.png",
  DATA_PREFIX "fonts/font2.png"
};

void resLoadPics(void)
{
  int i;

  fprintf(stderr, "Loading picture files now\n\n");

  /* load pics */
  for (i = 0; i < NUM_PICS; i++)
  {
    if ((gPics[i] = util::loadPic(gPicFiles[i])) == NULL)
      util::errorMsg("Couldn't load picture file: ", gPicFiles[i]);
    else
      fprintf(stderr, "Loaded picture file: \"%s\"\n", gPicFiles[i]);
  }

  fprintf(stderr, "\n\n");
}

void resLoadMusic(void)
{
  int i;

  fprintf(stderr, "Loading music files now\n\n");

  /* load music */
  for (i = 0; i < NUM_MUSIC; i++)
  {
    if ((gMusic[i] = Mix_LoadMUS(gMusicFiles[i])) == NULL)
      fprintf(stderr, "Couldn't find music file: ", gMusicFiles[i]);
    else
      fprintf(stderr, "Loaded music file: \"%s\"\n", gMusicFiles[i]);
  }

  fprintf(stderr, "\n\n");
}

void resLoadSfx(void)
{
  int i;

  fprintf(stderr, "Loading sfx files now\n\n");

  /* load sfx */
  for (i = 0; i < NUM_SFX; i++)
  {
    if ((gSfx[i] = Mix_LoadWAV(gSfxFiles[i])) == NULL)
      util::errorMsg("Couldn't find sfx file: ", gSfxFiles[i]);
    else
      fprintf(stderr, "Loaded sfx file: \"%s\"\n", gSfxFiles[i]);
  }

  fprintf(stderr, "\n\n");
}

void resLoadFonts(void)
{
  int i;

  fprintf(stderr, "Loading fonts now\n\n");

  /* load fonts */
  for (i = 0; i < NUM_FONTS; i++)
  {
    if ((gFonts[i] = LoadFont(gFontFiles[i])) == NULL)
      util::errorMsg("Couldn't find font file: ", gFontFiles[i]);
    else
      fprintf(stderr, "Loaded font: \"%s\"\n", gFontFiles[i]);
  }
}

void resFreePics(void)
{
  int i;

  /* free pics */
  for (i = 0; i < NUM_PICS; i++)
  {
    if (gPics[i] != NULL)
      SDL_FreeSurface(gPics[i]);
  }
}

void resFreeMusic(void)
{
  int i;

  /* free music */
  for (i = 0; i < NUM_MUSIC; i++)
  {
    if (gMusic[i] != NULL)
      Mix_FreeMusic(gMusic[i]);
  }
}

void resFreeSfx(void)
{
  int i;

  /* free sfx */
  for (i = 0; i < NUM_SFX; i++)
  {
    if (gSfx[i] != NULL)
      Mix_FreeChunk(gSfx[i]);
  }
}

void resFreeFonts(void)
{
  int i;
  /* free fonts */
  for (i = 0; i < NUM_FONTS; i++)
  {
    if (gFonts[i] != NULL)
      FreeFont(gFonts[i]);
  }
}
