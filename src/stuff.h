#include "BloxConf.h"

#ifndef _STUFF_H
#define _STUFF_H

//#define NOSOUND
#define VERSION "1.0.0"

/* default screen resolution and color depth */
#define SCREEN_W 800
#define SCREEN_H 600

/* default color key for the whole game */
#define COLOR_KEY_RED 0xff
#define COLOR_KEY_GREEN 0x00
#define COLOR_KEY_BLUE 0xff

/* block dimensions */
#define BLOCK_PIX_WH 30

/* shape array dimensions in blocks */
#define SHAPE_WH 4

/* play field height and width in blocks */
#define FIELD_W 10
#define FIELD_H 18

/* pixel offsets of the play field */
#define FIELD_PIX_X 250
#define FIELD_PIX_Y 0

/* play field width and height in pixels */
#define FIELD_PIX_W 300
#define FIELD_PIX_H 540

/* audio parameters */
#define AUDIO_RATE 22050
#define AUDIO_FORMAT AUDIO_S16
#define AUDIO_CHANNELS 2
#define AUDIO_BUFFERS = 4096

typedef int ShapeField[SHAPE_WH][SHAPE_WH];
extern BloxConf *gConf;

#endif
