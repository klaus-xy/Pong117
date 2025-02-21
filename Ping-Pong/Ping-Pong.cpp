// Ping-Pong.cpp : This file contains the 'WinMain' function. Program execution begins and ends there.
//

//#include <iostream>
#include "GameWindow.h"

// ENTRY POINT FOR WINDOWS APP
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	GameWindow gameWindow(hInstance, 1200,600 );

	// INITIALIZE NEW GAME WINDOW
	if (!gameWindow.InitGameWindow()) {
		MessageBox(nullptr, L"Looks like Klaus messed something up again. Call him. Or not. Please don't.", L"Game Window initialization failed",MB_ICONWARNING);
		return -1;
	};

	// Run the Game loop.
	gameWindow.RunGameLoop();

	return 0;
}


