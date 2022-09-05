#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_TOWN,LEVEL_END };
	enum CHANNELID { SOUND_EFFECT, SOUND_BGM, MAXCHANNEL };
}
#include <time.h>
#include <string>
#include <io.h>
#include <process.h>
#include "../inc/fmod.h"
#include "../inc/fmod.hpp"
#pragma comment (lib, "../lib/x64/fmod_vc.lib")
using namespace FMOD;

// Client_Defines.h

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
//extern int g_iFrame;

#define     fSOUND  0.1f
typedef struct UiPos
{
	float fPosX;
	float fPosY;
	bool  bNext;

}Pos;


using namespace Client;