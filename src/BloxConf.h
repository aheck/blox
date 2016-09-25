/*
 *  BloxConf.h - Configuration Class
 *
 * 
 *  This class implements the game config:
 *
 *  - read in configuration form ini file
 *  - create default ini file if none exists
 *  - private member variable for every option
 *  - public methods to access these options (data encapsulation)
 *  - write changed configuration to disk
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <iniparser.h>
#include <string>

#ifndef _BLOXCONF_H
#define _BLOXCONF_H

#define CONF_DIR ".blox"
#define CONF_FILE "blox.ini"

class BloxConf
{
  private:

    std::string confFilePath;                 // absolute path of the config file
    std::string confDirPath;
    std::string homedir;
    dictionary *ini;

    // [General] variables
    bool confFullscreen;
    bool confShowFps;
    std::string confLogfile;
    // [Sfx] variables
    bool confSfxPlay;
    int confSfxVolume;
    // [Music] variables
    bool confMusicPlay;
    int confMusicVolume;

    void getConfigFileName(void);
    bool createConfig(void);

  public:

    BloxConf();
    ~BloxConf();

    void readConfig(void);
    
    // read methods
    bool getFullscreen(void) {return this->confFullscreen;}
    bool getShowFps(void) {return this->confShowFps;}
    std::string getLogfile(void) {return this->confLogfile;}
    bool getSfxPlay(void) {return this->confSfxPlay;}
    int getSfxVolume(void) {return this->confSfxVolume;}
    bool getMusicPlay(void) {return this->confMusicPlay;}
    int getMusicVolume(void) {return this->confMusicVolume;}

    // write methods
    bool setFullscreen(bool);
    bool setShowFps(bool);
    bool setSfxPlay(bool);
    bool setSfxVolume(int);
    bool setMusicPlay(bool);
    bool setMusicVolume(int);
  
};

#endif
