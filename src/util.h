/*
 *  util.h - Collection of Helper Functions
 *
 * 
 *  This module contais the following functions:
 *
 *  - loadPic: load a picture into a SDL_Surface
 *  - loadPic: same as above but with an color key
 *  - loadPicAlpha: same as loadPic but apply an alpha value
 *  - errorMsg: print an "could't read file" error and call exit
 *  - playSound: play a sound and stop the channel if it's != -1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "stuff.h"
#include "resources.h"

#ifndef _UTIL_H
#define _UTIL_H

namespace util
{
  SDL_Surface *loadPic(const char *filename);
  SDL_Surface *loadPic(const char *filename, Uint32 colorkey);
  SDL_Surface *loadPicAlpha(const char *filename, Uint8 alpha);
  void errorMsg(const char* msg, const char* filename);
  int playSound(int sound, int channel);
  bool encryptFile(const char *filename, char *key);
  bool decryptFile(const char *filename, char *key);
}

#endif
