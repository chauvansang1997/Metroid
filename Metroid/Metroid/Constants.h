#pragma once
#include <Windows.h>
#include <d3dx9math.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }

#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)

// Window
const char CLASS_NAME[] = "Metroid";	
const char GAME_TITLE[] = "Metroid";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH = 640;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels

// Game Manger
const double PI = 3.14159265;
const float FRAME_RATE = 60.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 60.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE; // maximum time used in calculations
const float GRAVITY = 6.67428e-11f;             // gravitational constant


// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY = VK_ESCAPE;       // escape key
const UCHAR ALT_KEY = VK_MENU;         // Alt key
const UCHAR ENTER_KEY = VK_RETURN;       // Enter key
#define VK_X 0x58
#define VK_Z 0x5A


// Source
const char SOURCE_IMAGE[] = "Resoures\\SpriteSheets\\MetroidTexture.png";//character of game
const char SOURCE_JSON[] = "json\\MetroidTexture.json"; //json file of metroid sprite

// Element ID
enum eID {
	SAMUS = 0,
};

// Element's status
enum eStatus {
	START = 0,
	JUMPING = 1,
	RUNNING = 2,
	ROLLING = 3,
	ENDING = 4,
	STANDING = 5,
	FALLING = 6,
};

// direction
enum eDirection {
	left = -1,
	right = 1,
};

typedef D3DXVECTOR3 GVector3;
typedef D3DXVECTOR2 GVector2;
#define VECTOR2ZERO GVector2(0.0f, 0.0f)
#define VECTOR2ONE  GVector2(1.0f, 1.0f)