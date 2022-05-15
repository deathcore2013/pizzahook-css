
//From  Simplex pub base
#define COLORCODE(r,g,b,a)((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)))
#define RED(COLORCODE)((int) ( COLORCODE >> 24) )
#define BLUE(COLORCODE)((int) ( COLORCODE >> 8 ) & 0xFF )
#define GREEN(COLORCODE)((int) ( COLORCODE >> 16 ) & 0xFF )
#define ALPHA(COLORCODE)((int) COLORCODE & 0xFF )
#define RGBA(COLORCODE)RED( COLORCODE ), GREEN( COLORCODE ), BLUE( COLORCODE ), ALPHA( COLORCODE )
#define CHEAT_ORANGE COLORCODE( 255, 100, 0, 255 )
#define CHEAT_YELLOW COLORCODE( 255, 255, 0, 255 )
#define CHEAT_PURPLE COLORCODE( 55, 25, 128, 255 )
#define CHEAT_RED COLORCODE( 255, 0, 0, 255 )
#define CHEAT_GREEN COLORCODE( 0, 255, 0, 255 )
#define CHEAT_BLUE COLORCODE( 0, 0, 255, 255 )
#define CHEAT_BLACK COLORCODE( 0, 0, 0, 255 )
#define CHEAT_WHITE COLORCODE( 255, 255, 255, 255 )
#define CHEAT_PINK COLORCODE( 255, 0, 255, 255 )
#define CHEAT_LIGHTBLUE COLORCODE( 0, 0, 225, 225 )
#define CHEAT_LIGHTGREEN COLORCODE( 0, 225, 0, 225 )
#define CHEAT_LIGHTRED COLORCODE( 225, 0, 0, 225 )

#ifndef UNDEGROUND
#define UNDEGROUND

#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <istream>

#include "detours.h"
#include "utils.h"
#include "sdk.h"
#include "definitions.h"
#include "vmt.h"
#include "netvars.h"
#include "prediction.h"
#include "weapon.h"
#include "bot.h"
#include "Menu.h"
#include "vars.h"

#define BUILD_DATE  __DATE__, __TIME__

extern HANDLE Thread;

#endif