#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <windows.h>
#include <stdio.h>

typedef unsigned long long uint64;

#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }


#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PI 3.14159265358979323846  /* pi */
#define HAVE_M_PI
#undef max
#undef min

#define SETTINGS_FOLDER "Configuration/"

#define ASSETS_FOLDER "Assets/"
#define MODEL_A_FOLDER "Assets/Models/"
#define TEXTURE_A_FOLDER "Assets/Textures/"
#define SCENE_A_FOLDER "Assets/Scenes/"
#define SCRIPT_A_FOLDER "Assets/Scripts/"

#define LIBRARY_FOLDER "Library/"
#define MODEL_L_FOLDER "Library/Models/"
#define MESH_L_FOLDER "Library/Meshes/"
#define MATERIAL_L_FOLDER "Library/Material/"

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

//#define GAME_BUILD
