// Chapter 4 Direct3D Initialization.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>
#include "Application.h"


#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	Application app(hInstance);
	
	if( !app.Init() )
		return 0;
	
	return app.Run();
}