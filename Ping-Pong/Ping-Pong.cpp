// Ping-Pong.cpp : This file contains the 'WinMain' function. Program execution begins and ends here.

#include "GameWindow.h"
#include "GameConstants.h"

// ENTRY POINT FOR WINDOWS APP
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	float windowWidth = GameConstants::WINDOW_WIDTH;
	float windowHeight = GameConstants::WINDOW_HEIGHT;

	// Create new game window
	GameWindow gameWindow(hInstance, windowWidth, windowHeight);

	// Inoitialize the new game window.	
	if (!gameWindow.InitGameWindow()) {
		MessageBox(nullptr, L"Looks like Klaus messed something up. Again. Contact him [X]: 0xKlaus117 ", L"Game Window initialization failed.",MB_ICONWARNING);
		return -1;
	};

	// Run the Game loop.
	gameWindow.RunGameLoop();

	return 0;
}


