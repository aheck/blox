/*
 *  HighScore.h - HighScore Class
 *
 * 
 *  This class implements the handling of highscores:
 *
 *  - check if an score is a highscore
 *  - add an highscore to the list
 *  - save highscores on disk
 *  - encrypt the highscore file
 *  - read in highscores from disk
 *  - create empty highscore file if it doesn't exist
 *
 */

#include <stdio.h>
#include <iniparser.h>
#include <string>

#include "SdlDraw.h"
#include "stuff.h"
#include "resources.h"

#ifndef _HIGHSCORE_H
#define _HIGHSCORE_H

#define HIGHSCORE_COUNT 10

#define HIGHSCORE_FILE "highscore.ini"

#define ENC_KEY "84926519"


class HighScore : public SdlDraw
{
  private:

    dictionary *ini;                     // hash to hold the parsed ini data
    std::string highscoreFile;

    struct HsEntry
    {
      int highscore;     // the scores
      std::string player;      // the name of the player
    } hsEntries[HIGHSCORE_COUNT];

    bool createHighScoreFile();
    void encryptFile(void);
    void decryptFile(void);
    int isHighScore(int score);
    void insertHighScore(int score, char *name);

  public:

    HighScore(SDL_Surface *screen);
    ~HighScore();

    void readHighScores(void);
    void addHighScore(int points);
    void draw(void);

};

#endif
