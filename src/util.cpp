#include "util.h"

/* load a picture file into an SDL_Surface and convert it to display format */
SDL_Surface *util::loadPic(const char *filename)
{
  SDL_Surface *tmp, *buffer;

  tmp = IMG_Load(filename);
  if (tmp == NULL)
  {
    fprintf(stderr, "Unable to load picture file %s", filename);
    exit(1);
  }
  else
  {
    fprintf(stderr, "Loaded picture file \"%s\"\n", filename);
  }

  SDL_SetAlpha(tmp, SDL_SRCALPHA | SDL_RLEACCEL, 255);
  buffer = SDL_DisplayFormatAlpha(tmp);
  SDL_FreeSurface(tmp);

  return buffer;
}

/* load a picture file into an SDL_Surface and convert it to display format */
SDL_Surface *util::loadPic(const char *filename, Uint32 colorkey)
{
  SDL_Surface *tmp, *buffer;

  tmp = IMG_Load(filename);
  if (tmp == NULL)
  {
    fprintf(stderr, "Unable to load picture file %s", filename);
    exit(1);
  }
  else
  {
    fprintf(stderr, "Loaded picture file \"%s\"\n", filename);
  }

  SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, colorkey);
  buffer = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);

  return buffer;
}

SDL_Surface *util::loadPicAlpha(const char *filename, Uint8 alpha)
{
  SDL_Surface *tmp, *buffer;

  tmp = IMG_Load(filename);
  if (tmp == NULL)
  {
    fprintf(stderr, "Unable to load picture file %s", filename);
    exit(1);
  }
  else
  {
    fprintf(stderr, "Loaded picture file \"%s\"\n", filename);
  }

  SDL_SetAlpha(tmp, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
  buffer = SDL_DisplayFormatAlpha(tmp);
  SDL_FreeSurface(tmp);

  return buffer;
}

void util::errorMsg(const char* msg, const char* filename)
{
  fprintf(stderr, "\n\nFATAL ERROR:\n\n%s%s\n\n", msg, filename);
  exit(1);
}

int util::playSound(int sound, int channel)
{
#ifndef NOSOUND
  if (gConf->getSfxPlay())
  {
    if (Mix_Playing(channel))
    {
      Mix_HaltChannel(channel);
    }

    return Mix_PlayChannel(-1, getResSfx(sound), 0);
  }
  else
  {
    return -1;
  }
#else
  return -1;
#endif
}

bool util::encryptFile(const char *filename, char *key)
{
  FILE *fp;
  char chunk;
  int keypos = 0;
  int keylen = strlen(key);

  // test if file exists and we can read it
  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "Can't open file for reading: %s\n", filename);
    return false;
  }

  fclose(fp);

  // open file for the actual encryption
  if ((fp = fopen(filename, "r+")) == NULL)
  {
    fprintf(stderr, "Can't open file for writing: %s\n", filename);
    return false;
  }

  // do the actual encryption
  while (1)
  {
    chunk = fgetc(fp);
    if (feof(fp))
      break;
    chunk = (chunk) ^ (key[keypos]);
    fseek(fp, -1, SEEK_CUR);
    fputc(chunk, fp);

    if (keypos == keylen)
      keypos = 0;
    else
      keypos++;
  }

  fclose(fp);

  return true;
}

bool util::decryptFile(const char *filename, char *key)
{
  return util::encryptFile(filename, key);
}
