#ifndef CONFIG_H
#define CONFIG_H

#include "processor.h"

// #define WIDTH (1920*2/3)
// #define HEIGHT (1080*2/3)

#define SCALEFACTOR 8
#define WIDTH (TILEMAP_X*TILESIZE*SCALEFACTOR)
#define HEIGHT (TILEMAP_Y*TILESIZE*SCALEFACTOR)

#define FPS (05)
#define WINDOWNAME "sillyemulator"

#endif
