cmake_minimum_required(VERSION 3.0)

find_package(PkgConfig)
pkg_check_modules(SDL sdl)
find_package(SDL_mixer)
find_package(SDL_image)

include_directories(
    ${SDL_INCLUDE_DIRS}
    ${SDL_MIXER_INCLUDE_DIRS}
    ${SDL_IMAGE_INCLUDE_DIRS}
    extras
)

add_executable(blox BloxConf.cpp FpsCounter.cpp Game.cpp GameMenu.cpp HighScore.cpp main.cpp resources.cpp SdlDraw.cpp Shape.cpp util.cpp extras/BFont.c extras/dictionary.c extras/iniparser.c extras/strlib.c)
target_link_libraries(blox ${SDL_LIBRARIES} ${SDL_MIXER_LIBRARIES} ${SDL_IMAGE_LIBRARIES})
