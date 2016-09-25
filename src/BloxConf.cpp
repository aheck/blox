#include "BloxConf.h"

BloxConf::BloxConf()
{
  this->ini = NULL;
}

BloxConf::~BloxConf()
{
  if (this->ini != NULL);
    iniparser_freedict(this->ini);
}

void BloxConf::getConfigFileName(void)
{
  FILE *fp;
#ifdef WIN32
  this->confDirPath = "./";
  this->confFilePath = this->confDirPath + "/" + CONF_FILE;
#else
  // get the home directory
  this->homedir = getenv("HOME");
  this->confDirPath = this->homedir + "/" + CONF_DIR;
  this->confFilePath = this->confDirPath + "/" + CONF_FILE;

  // check if config directory exists; when not create it
  if ((fp = fopen(this->confDirPath.c_str(), "r")) == NULL)
  {
    if (mkdir(this->confDirPath.c_str(), 0775) != 0)
      fprintf(stderr, "Can't create dir: %s\n", this->confDirPath.c_str());
    else
      fprintf(stderr, "Creating dir: %s\n", this->confDirPath.c_str());
  }
  else
  {
    fclose(fp);
  }
#endif

  if ((fp = fopen(this->confFilePath.c_str(), "r")) == NULL)
  {
    this->createConfig();
  }
}

bool BloxConf::createConfig(void)
{
  FILE *fp;
  int numlines;
  int i;
  char *filecontent[] = {"[General]\n",
                              "fullscreen = false\n",
                              "showfps = false\n",
                              "logfile = blox.log\n",
                              "[Sfx]\n",
                              "play = true\n",
                              "volume = 10\n",
                              "[Music]\n",
                              "play = false\n",
                              "volume = 10\n"};

  if (this->confFilePath == "")
    return false;

  if ((fp = fopen(this->confFilePath.c_str(), "w")) == NULL)
  {
    fprintf(stderr, "Can't open file for writing: %s\n", this->confFilePath.c_str());
    return false;
  }

  numlines = sizeof(filecontent) / sizeof (char*);

  for (i = 0; i < numlines; i++)
  {
    fwrite(filecontent[i], sizeof(char), strlen(filecontent[i]),fp);
  }

  fclose(fp);
  return true;
}

void BloxConf::readConfig(void)
{
  this->getConfigFileName();

  if ((this->ini = iniparser_load((char*)this->confFilePath.c_str())) == NULL)
  {
    fprintf(stderr, "Can't parse config file: %s\n", this->confFilePath.c_str());
    return;
  }
  

  // read in config values
  this->confFullscreen = iniparser_getboolean(this->ini, "General:fullscreen", -1);
  this->confShowFps = iniparser_getboolean(this->ini, "General:showfps", -1);
  this->confLogfile = iniparser_getstring(this->ini, "General:logfile", "");

  this->confSfxPlay = iniparser_getboolean(this->ini, "Sfx:play", -1);
  this->confSfxVolume = iniparser_getint(this->ini, "Sfx:volume", -1);

  this->confMusicPlay = iniparser_getboolean(this->ini, "Music:play", -1);
  this->confMusicVolume = iniparser_getint(this->ini, "Music:volume", -1);
}

bool BloxConf::setFullscreen(bool fs)
{
  this->confFullscreen = fs;

  return true;
}

bool BloxConf::setShowFps(bool showfps)
{
  this->confShowFps = showfps;

  return true;
}

bool BloxConf::setSfxPlay(bool sfxplay)
{
  this->confSfxPlay = sfxplay;

  return true;
}

bool BloxConf::setSfxVolume(int sfxvolume)
{
  this->confSfxVolume = sfxvolume;

  return true;
}

bool BloxConf::setMusicPlay(bool musicplay)
{
  this->confMusicPlay = musicplay;

  return true;
}

bool BloxConf::setMusicVolume(int vol)
{
  this->confMusicVolume = vol;

  return true;
}
