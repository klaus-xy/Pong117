#include "GameWindow.h"


GameWindow::GameWindow(HINSTANCE hInstance, int width, int height)
	: hInstance(hInstance), hwnd(nullptr), width(width), height(height)
{
	// Initialize Game Parameters
	paddleWidth = 20;
	paddleHeight = 100;
	padding = 20;
	player1Y = height/2 - paddleHeight/2;
	player2Y = height/2 - paddleHeight / 2;
	ballSize = 20;
	ballPosX = width/2;
	ballPosY = height/2;
	ballVelX = 5;
	ballVelY = 5;
	score1 = 0;
	score2 = 0;

	bIsRunning = true;
	renderEngine = new RenderEngine();
}


GameWindow::~GameWindow() {
	CleanUp();
}


static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		case 'W':

			//if (game.player1Y > 0) game.player1Y -= 20;
			OutputDebugString(L"Player1 Moving Up...\n");
			break;

		case 'S':
			OutputDebugString(L"Player1 Moving Down...\n");
			//if (game.player1Y < HEIGHT - PADDLE_HEIGHT) game.player1Y += 20;
			break;

		case VK_UP:
			OutputDebugString(L"Player2 Moving Up...\n");
			//if (game.player2Y > 0) game.player2Y -= 20;
			break;

		case VK_DOWN:
			OutputDebugString(L"Player2 Moving Down...\n");
			//if (game.player2Y < HEIGHT - PADDLE_HEIGHT) game.player2Y += 20;
			break;

		}
	

	case WM_SIZE:
		InvalidateRect(hwnd, nullptr, true); //redraws on resize
		return 0;

	case WM_PAINT:
	{  
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		///////// All painting occurs here, between BeginPaint and EndPaint.///////
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+3));

		//Ellipse(hdc, 300, 325, 350, 275);
		
		//Update();

		EndPaint(hwnd, &ps);

		//////////////////////////////////////////////////////////////////////////
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool GameWindow::Initialize()
{
	const wchar_t CLASS_NAME[] = L"Game Window Class";

	// Create window class. //
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Default cursor
	// wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 10); // bg color

	// Register window class. //
	RegisterClass(&wc);

	// Create the window. //
	hwnd = CreateWindowEx(
		0,										  // Optional window styles.
		CLASS_NAME,								 // Window class
		L"Ping Pong Game .117",					// Window text
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,      // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	// ShowWindow(hwnd, SW_SHOW); // no need for this sinve im using WS_VISIBLE
	return true;

} 
// ✅

void GameWindow::RunGameLoop()
{
	MSG msg = { };
	while (bIsRunning)
	{
		// Check if there's a message in the queue (Input, window events, etc)
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		// while (GetMessage(&msg, NULL, 0, 0) > 0) {
			if (msg.message == WM_QUIT) {
				return; // Exit the loop if WM_QUIT is received
			};
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Run rest of game loop here

		HDC hdc = GetDC(hwnd);
		DrawGameScene(hdc);
		ReleaseDC(hwnd, hdc);

	
		// Update();
		// INPUT
		// PHYSICS

		//InvalidateRect(hwnd, NULL, FALSE);
		// Optional: Add a small delay to limit the frame rate
		Sleep(16); // ~60 FPS
	}
}

void GameWindow::DrawGameScene(HDC hdc)
{

	// Player 1 Paddle
	renderEngine->DrawRect(hdc, padding, player1Y, (paddleWidth + padding), (paddleHeight + player1Y), RGB(20, 200, 200));
	
	// Player 2 Paddle
	renderEngine->DrawRect(hdc, (width - paddleWidth - padding), player1Y, (width - paddleWidth), (paddleHeight + player1Y), RGB(255,0, 50));

	// Ball
	renderEngine->DrawEllipse(hdc, ballPosX, ballPosY, (ballPosX + ballSize), (ballPosY + ballSize), RGB(255, 25, 255));

	//// Player 2 Paddle
	//renderEngine->DrawRect(hdc, (width-20), 50, 40, 250, RGB(255, 0, 0));
	// 

	
}

//void GameWindow::Update()
//{
//	if (keyState[VK_LEFT]) {
//		OutputDebugString(L"Moving left continuously...\n");
//		// Add code to move your paddle left
//	}
//	if (keyState[VK_RIGHT]) {
//		OutputDebugString(L"Moving right continuously...\n");
//		// Add code to move your paddle right
//	}
//	if (keyState[VK_UP]) {
//		OutputDebugString(L"Moving up continuously...\n");
//		// Add code for upward movement
//	}
//	if (keyState[VK_DOWN]) {
//		OutputDebugString(L"Moving down continuously...\n");
//		// Add code for downward movement
//	}
//}

void GameWindow::CleanUp()
{
	if (hwnd) {
		DestroyWindow(hwnd);
		hwnd = nullptr;
	}
	if (renderEngine) {
		delete renderEngine;
	}
	bIsRunning = false;
}


