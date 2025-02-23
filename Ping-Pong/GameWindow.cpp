#include "GameWindow.h"
#include <algorithm>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")


GameWindow::GameWindow(HINSTANCE hInstance, float width, float height)
	: hInstance(hInstance), hwnd(nullptr), wWidth(width), wHeight(height)
{
	// Initialize Game Parameters
	originalHeight = height;
	originalWidth = width;
	originalPaddleWidth = 20.0f;
	originalPaddleHeight = 100.0f;
	originalPaddleSpeed = 600.0f;
	originalBallSize = 20;
	maxBallSize = 26.0f;
	originalBallVelX = 700.0f;
	originalBallVelY = 0.0f;

	hScale = 1;
	wScale = 1;
	textScale = (wScale + hScale)/2;
	paddleWidth = originalPaddleWidth*wScale;
	paddleHeight = originalPaddleHeight*hScale;
	paddleSpeed = originalPaddleSpeed;
	paddingX = 20.0f;
	paddingY = 20.0f;
	player1YPos = wHeight/2.0f - paddleHeight/2.0f;
	player2YPos = wHeight/2.0f - paddleHeight / 2.0f;
	ballSize = originalBallSize;
	ballPosX = wWidth/2.0f;
	ballPosY = wHeight/2.0f;
	ballVelX = originalBallVelX;
	ballVelY = originalBallVelY;
	colliderPadding = 2.0f;
	ballBounciness = 1.015f;
	deflectMagnitude = 10.0f;
	aiMoveThreshold = 50.0f; // easy 50 + slow speed; md 25; hd 10;
	score1 = 0;
	score2 = 0;
	winScore = 7;
	winnerNum = 0;

	player1Color = RGB(20, 200, 200);
	player2Color = RGB(255, 50, 50);
	serveTextColor = player1Color;
	font1 = L"Gotham";
	font2 = L"Monospac821 BT";

	QueryPerformanceFrequency(&frequency);		  // Get timer frequency
	QueryPerformanceCounter(&lastFrameTime);     // Initialize last frame time
	deltaTime = 0.0f;

	frameCount = 0;
	fps = 0.0;
	timeElapsed = 0.0;
	canShowStats = false;
	canShowHelp = true;

	currentStartDelay = gameStartDelay;
	gameStartDelayTimer = gameStartDelay;

	gameTime = 0.0f;


	bIsRunning = true;
	bGameHasStarted = false;
	bIsMultiPlayer = false;
	bIsGameOver = false;
	bIsDeveggFound = false;
	bIs117Found = false;

	cameoBitmap = (HBITMAP)LoadImage(NULL,L"assets/cameo.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	renderEngine = new RenderEngine();
	audioEngine = new AudioEngine();

	audioEngine->PreloadMusic(L"assets/audio/8-bit-music.mp3", L"bgm");      // Preload background music
	audioEngine->PreloadMusic(L"assets/audio/bg-music-2.mp3", L"bgm-2");      // Preload background music 2
}


GameWindow::~GameWindow() {
	CleanUp();
}


static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	GameWindow* pGameWindow = reinterpret_cast<GameWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)); // Pointer to game window that was created.
	
	switch (uMsg)
	{
	case WM_KEYDOWN:
		// CHEAT CODE
		pGameWindow->CheckEggStats();
		////////

		switch (wParam) {
		case VK_OEM_3:			// Back tick `
		
			pGameWindow->canShowStats = !pGameWindow->canShowStats;
			break;
		
		case 0x48:			// "H"
		
			pGameWindow->canShowHelp = !pGameWindow->canShowHelp;
			break;

		case 0x59:			// "Y"
		
			if (pGameWindow->bIsGameOver) {
				pGameWindow->RestartGame();
			}
			break;

		case 0x4E:			// "N"

			if (pGameWindow->bIsGameOver) {
				pGameWindow->bIsRunning = false; // Quit game
			}
			break;
		case 0x52:			// "R"
				pGameWindow->RestartGame(); // Restart game at any point in the game
		
			break;
		case 0x4d:			// "M"
				pGameWindow->bIsMultiPlayer = !pGameWindow->bIsMultiPlayer; // Toggle game mode [CPU or local multiplayer]
		
			break;
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;	

	case WM_SIZE:
		if (pGameWindow)
		{
			int newwWidth = LOWORD(lParam);
			int newwHeight = HIWORD(lParam);
			pGameWindow->OnResize(newwWidth, newwHeight);

		}
		InvalidateRect(hwnd, nullptr, true); //redraws on resize
		return 0;

	case WM_PAINT:
	{  
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

	
		///////// All painting occurs here, between BeginPaint and EndPaint.///////

		if (pGameWindow) 
		{
			pGameWindow->InitBuffer(hdc);
			pGameWindow->DrawGameScene(hdc);			
		}

		EndPaint(hwnd, &ps);

		//////////////////////////////////////////////////////////////////////////
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool GameWindow::InitGameWindow()
{
	const wchar_t CLASS_NAME[] = L"Game Window Class";

	// Create window class. //
	WNDCLASS wc = { };

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Default cursor
	
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 10); // bg color

	// Register window class. //
	RegisterClass(&wc);

	// Create the window. //
	hwnd = CreateWindowEx(
		0,										  // Optional window styles.
		CLASS_NAME,								 // Window class
		L"🧛🏽‍♀️ Pong .117",						// Window text
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,      // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, wWidth, wHeight,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	// Store 'this' window in the window's user data
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	// Get ACTUAL client area size (the title bar and borders share the window size, so getting client rect gets the dimensions of the display area)
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	wWidth = clientRect.right - clientRect.left;
	wHeight = clientRect.bottom - clientRect.top;

	originalHeight = wHeight;
	originalWidth = wWidth;

	// Initialize game state with actual window dimensions
	InitGameScene();

	// Initialize buffer
	HDC hdc = GetDC(hwnd);
	InitBuffer(hdc);
	ReleaseDC(hwnd, hdc);
 
	// Play game music
	audioEngine->PlayMusic(L"bgm");
	audioEngine->PlayClip(L"assets/audio/restart");

	return true;

} 

void GameWindow::RunGameLoop()
{
	MSG msg = { };
	
	while (bIsRunning)
	{
		UpdateDeltaTime();  // Calculate deltaTime for this frame
		gameTime += deltaTime;
	

		// Add delay before game start
		if (gameStartDelayTimer > 0)
		{
			gameStartDelayTimer -= deltaTime;
		}
		else {
			gameStartDelayTimer = 0.0f;
		}

		
	 
		// Check if there's a message in the queue (Input, window events, etc)
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				return;			// Exit the loop if WM_QUIT is received
			};
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Run rest of game loop here
		Update();	
			
		InvalidateRect(hwnd, NULL, FALSE);

		// Optional
		//Sleep(16); // LOCK @ ~60 FPS
	}
}

void GameWindow::Input()
{

	if (gameStartDelayTimer <= (currentStartDelay/1.5) && !bIsGameOver)
	{
		// 'W'
		if (GetAsyncKeyState(0x57) && player1YPos > 0)
		{
			player1YPos -= paddleSpeed * deltaTime;
		}

		// 'S'
		if (GetAsyncKeyState(0x53) && (player1YPos + paddleHeight) < wHeight) {
			player1YPos += paddleSpeed * deltaTime;
		}

		// PLAYER 2 CONTROLS [LOCAL MULTIPLAYER]
		if (bIsMultiPlayer)
		{


			// UP ARROW
			if (GetAsyncKeyState(VK_UP) && player2YPos > 0)
			{
				player2YPos -= paddleSpeed * deltaTime;
				// OutputDebugString(L"Player2 Moving Up...\n");
			}

			// DOWN ARROW
			if (GetAsyncKeyState(VK_DOWN) && (player2YPos + paddleHeight) < wHeight) {
				player2YPos += paddleSpeed * deltaTime;
			}
		}

		// Else use AI player
		else 
		{
			float paddle2Center = player2YPos + paddleHeight / 2.0f;
			float ballCenter = ballPosY + ballSize / 2.0f;
			
			// If ball is above paddle
			if ((paddle2Center > ballCenter + aiMoveThreshold * hScale) && player2YPos > 0 ) 
			{
				player2YPos -= paddleSpeed * 0.75 * deltaTime; // Move up
			}
			// If ball is below paddle
			else if ((paddle2Center < ballCenter - aiMoveThreshold * hScale) && (player2YPos + paddleHeight < wHeight ))
			{
				player2YPos += paddleSpeed * 0.75 * deltaTime;	// Move down
			}
		}

	}

	


}

void GameWindow::Move()
{
	if (gameStartDelayTimer <= 0 && !bIsGameOver) 
	{
		ballPosX += ballVelX * deltaTime; //dir and veelocty
		ballPosY += ballVelY * deltaTime;
	}
	
}

void GameWindow::Collision()
{
	// BALL COLLISION DETECTION
// Test Vertical Bounds
	if (ballPosY <= 0)
	{
		ballVelY = abs(ballVelY);	// Use abs for better collision
		//audioEngine->PlayClip(L"paddle-hit");
	}
	else if (ballPosY + ballSize >= wHeight)
	{
		ballVelY = -abs(ballVelY);	// Use abs for better collision
		//audioEngine->PlayClip(L"paddle-hit");
	}

	// Test Paddle bounds
	// Paddle 1
	if ((ballPosX <= (paddingX + paddleWidth + colliderPadding)) && ((ballPosY + ballSize) >= (player1YPos - colliderPadding)) && (ballPosY) <= (player1YPos + paddleHeight + colliderPadding))
	{
		ballVelX = abs(ballVelX); // Use absolute values for better collision

		// Add proper ball deflection from the paddle depending on hit point on the paddle
		float paddleCenter = player1YPos + paddleHeight / 2.0f;
		float hitPosition = (ballPosY + ballSize / 2.0f) - paddleCenter;
		ballVelY = hitPosition * deflectMagnitude;
		ballVelX *= ballBounciness;

		audioEngine->PlayClip(L"assets/audio/paddle-hit");
	}
	else if (ballPosX < 0)
	{
		score2++;
		audioEngine->PlayClip(L"assets/audio/score");
		serveTextColor = player2Color;
		ResetGame();
		ballVelX *= -1;
		
		if (score2 >= winScore) 
		{
			OnGameOver();
		}
		
	}

	// Paddle 2
	if (((ballPosX + ballSize) >= (wWidth - paddingX - paddleWidth + colliderPadding)) && ((ballPosY + ballSize) >= (player2YPos - colliderPadding)) && (ballPosY) <= (player2YPos + paddleHeight + colliderPadding))
	{
		ballVelX = -abs(ballVelX);	// Use abs for better collision

		// Add proper ball deflection from the paddle depending on hit point on the paddle
		float paddleCenter = player2YPos + paddleHeight / 2;
		float hitPosition = (ballPosY + ballSize / 2) - paddleCenter;
		ballVelY = hitPosition * deflectMagnitude;
		ballVelX *= ballBounciness;

		audioEngine->PlayClip(L"assets/audio/paddle-hit");
	}
	else if (ballPosX > wWidth)
	{
		score1++;
		audioEngine->PlayClip(L"assets/audio/score");
		serveTextColor = player1Color;
		ResetGame();

		if (score1 >= winScore)
		{
			OnGameOver();
		}
		
	}
}

// INPUT, PHYSICS, Game logic ...
void GameWindow::Update()
{
	Input();
	Move();
	Collision();
} 

void GameWindow::OnResize(float newWidth, float newHeight)
{
	wWidth = newWidth;
	wHeight = newHeight;

	// Re-render game scene elements wrt new window dimensions
	 // Calculate new scale
	hScale = newHeight/originalHeight;	 
	wScale = newWidth/originalWidth;
	
	textScale = (wScale + hScale) / 2;

	ballSize = originalBallSize * textScale;
	if (ballSize > maxBallSize) 
	{
		ballSize = maxBallSize;
	};
	
	InitGameScene();


	// Recreate buffer with new window size
	HDC hdc = GetDC(hwnd);
	InitBuffer(hdc);  
	ReleaseDC(hwnd, hdc);
}

void GameWindow::UpdateDeltaTime() {
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	deltaTime = static_cast<float>(currentTime.QuadPart - lastFrameTime.QuadPart) / frequency.QuadPart;
	lastFrameTime = currentTime;

	UpdateFPS();
}

void GameWindow::UpdateFPS()
{
	frameCount++; // Count frames
	timeElapsed += deltaTime;  // Start timer

	if (timeElapsed >= 0.5) // If 1 second has passed
	{
		fps = frameCount / timeElapsed; // Calculate FPS
		frameCount = 0; // Reset frame count
		timeElapsed = 0.0; // Reset timer
	}
}

void GameWindow::InitBuffer(HDC hdc)
{
	if (memDC) 
	{ 
		DeleteDC(memDC);	 // Delete old DC
	}       
	if (memBitmap)
	{
		DeleteObject(memBitmap); // Delete old bitmap
	}

	memDC = CreateCompatibleDC(hdc);
	memBitmap = CreateCompatibleBitmap(hdc, wWidth, wHeight);
	oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
}

void GameWindow::InitGameScene()
{
	// Update paddle params
	paddleHeight = originalPaddleHeight * hScale;
	paddleWidth = originalPaddleWidth;		//* textScale;
	paddleSpeed = originalPaddleSpeed * hScale;

	player1YPos = wHeight / 2 - paddleHeight / 2;
	player2YPos = wHeight / 2 - paddleHeight / 2;

	//ballSize = originalBallSize*textScale*0.8;

	// Reset ball position to center
	ballPosX = wWidth / 2;
	ballPosY = wHeight / 2;

	ballVelX = originalBallVelX * wScale;
	ballVelY = originalBallVelY * hScale;
}

void GameWindow::DrawGameScene(HDC hdc)
{
	// Clear the memory buffer - same as fillrect for bg-color
	PatBlt(memDC, 0, 0, wWidth, wHeight, (bIsDeveggFound ? BLACKNESS : BLACKNESS));

	///////////////////////// RENDER SCENE ////////////////////////////////
	// Center line
	renderEngine->DrawRect(memDC, ((wWidth / 2) + 10 * textScale), 60, (wWidth / 2 - 10 * textScale), (wHeight - 40), RGB(25, 20, 20));

	// Center Circle
	renderEngine->DrawEllipse(memDC, (wWidth / 2) - 40 * textScale, (wHeight / 2) - 40 * textScale, (wWidth / 2) + 40 * textScale, (wHeight / 2) + 40 * textScale, RGB(25, 20, 20));

	// Draw Table Text
	renderEngine->DrawText(memDC, (wWidth / 2) - 225 * textScale, (wHeight / 2) - 25 * textScale, L"PONG ", RGB(25, 20, 20), font1, 60 * textScale);
	renderEngine->DrawText(memDC, (wWidth / 2) + 60 * textScale, (wHeight / 2) - 25 * textScale, L" 1 1 7", RGB(25, 20, 20), font1, 60 * textScale);

	// Player 1 Paddle
	renderEngine->DrawRect(memDC, paddingX, player1YPos, (paddleWidth + paddingX), (paddleHeight + player1YPos), player1Color);

	// Player 2 Paddle
	renderEngine->DrawRect(memDC, (wWidth - paddleWidth - paddingX), player2YPos, (wWidth - paddingX), (paddleHeight + player2YPos), player2Color);

	// Ball
	renderEngine->DrawEllipse(memDC, (ballPosX-(ballSize/2)), (ballPosY - (ballSize / 2)), (ballPosX + (ballSize / 2)), (ballPosY + (ballSize / 2)), RGB(255, 255, 255));


	// Draw score
	std::wstring scoreText = std::to_wstring(score1) + L" || " + std::to_wstring(score2);
	std::wstring deltaTimeText = L"△Time: " + std::to_wstring(deltaTime);
	std::wstring fpsText = std::to_wstring((int)fps) + L" FPS";
	std::wstring hScaleText = L"hScale: " + std::to_wstring(hScale) ;
	std::wstring wScaleText = L"wScale: " + std::to_wstring(wScale) ;
	std::wstring player1PosText = L"Player1-YPos: " +std::to_wstring(player1YPos);
	std::wstring player2PosText = L"Player2-YPos: " +std::to_wstring(player2YPos);
	std::wstring gameTimeText = L"Game Time: " +std::to_wstring((int)gameTime);
	std::wstring startDelayTimerText = L"hTimer:  " +std::to_wstring(gameStartDelayTimer);
	std::wstring singleWinnerText = (score1 > score2 ? L"PLAYER " + std::to_wstring(winnerNum):L"CPU ") + L" WINS";
	std::wstring multiWinnerText = L"PLAYER " + std::to_wstring(winnerNum) + L" WINS";
	std::wstring konamiIndexText = L"KONAMI " + std::to_wstring(konamiIndex);

	renderEngine->DrawText(memDC, wWidth / 2 - 180*textScale, 10, L"PLAYER 1  : : : ", player1Color, font1, 20*textScale);
	renderEngine->DrawText(memDC, wWidth / 2 - 28 * textScale, 10, scoreText, RGB(255, 255, 225), font1, 22 * textScale);
	renderEngine->DrawText(memDC, wWidth / 2 + 60 * textScale, 10, (bIsMultiPlayer ? L" : : :  PLAYER 2": L" : : :  CPU"), player2Color, font1, 20*textScale);

	if (gameStartDelayTimer < 2.5f && gameStartDelayTimer > 1.25 )
	{
		renderEngine->DrawText(memDC, wWidth / 2 - 70 * textScale, wHeight/2 - 100 * textScale, L"READY!", RGB(255, 250, 250), font1, 40 * textScale);
	}

	if (gameStartDelayTimer < 1.0f && gameStartDelayTimer > 0.25f)
	{
		renderEngine->DrawText(memDC, wWidth / 2 - 70 * textScale, wHeight/2 - 100 * textScale, L"SERVE!", serveTextColor, font1, 40 * textScale);

	}

	// GAME OVER UI
	if (bIsGameOver)
	{
		renderEngine->DrawText(memDC, wWidth / 2 - 140 * textScale, 60 * textScale, L"GAME OVER!", RGB(255, 250, 250), font1, 50 * textScale);

		 //// WIN SCREEN  /////
		if (bIsMultiPlayer) 
		{
			renderEngine->DrawText(memDC, wWidth / 2 - 75 * textScale, 115 * textScale, multiWinnerText, serveTextColor, font1, 25 * textScale);
		}
		else 
		{
			renderEngine->DrawText(memDC, wWidth / 2 - 75 * textScale, 115 * textScale, singleWinnerText, serveTextColor, font1, 25 * textScale);
		}
	
		renderEngine->DrawText(memDC, wWidth / 2 - 75 * textScale, wHeight/2+80 * textScale, L"RESTART? [Y/N]", RGB(255, 250, 250), font1, 25 * textScale);
	
	
	}


	renderEngine->DrawText(memDC, wWidth-100, 20, L"👻🧛🏽‍♀️ .117", RGB(100, 100, 100), font1, 20);
	//renderEngine->DrawText(memDC, wWidth-500, 200, konamiIndexText, RGB(255, 100, 100), font1, 30);
	
	
	// TOGGLE HELP
	if (!canShowHelp)
	{
		renderEngine->DrawText(memDC, wWidth -100, 40, L"[H] HELP", RGB(100, 100, 100), font1, 20);
	}
	if (canShowHelp)
	{
		renderEngine->DrawText(memDC, wWidth - 200, 55, L"[H] HELP ", RGB(200, 200, 200), font1, 20);
		renderEngine->DrawText(memDC, wWidth - 200, 80, L"[PLAYER 1]", player1Color, font1, 20);
		renderEngine->DrawText(memDC, wWidth - 200, 100, L"[W]/[S] MOVE", RGB(100, 100, 100), font1, 20);
		renderEngine->DrawText(memDC, wWidth - 200, 140, L"[PLAYER 2]", player2Color, font1, 20);
		renderEngine->DrawText(memDC, wWidth - 200, 160, L"[⬆️]/[⬇️] MOVE ", RGB(100, 100, 100), font1, 20);

		renderEngine->DrawText(memDC, wWidth - 200, 195, L"[GAME]", RGB(200, 200, 200), font1, 20);
		renderEngine->DrawText(memDC, wWidth - 200, 220, L"[R] RESTART", RGB(100, 100, 100), font1, 20);
		renderEngine->DrawText(memDC, wWidth - 200, 240, L"[M] GAME MODE", RGB(100, 100, 100), font1, 20);


	
											  
	}

	// TOGGLE DEBUG STATS
	if (canShowStats)
	{
		renderEngine->DrawText(memDC,  10, 20, deltaTimeText, RGB(50, 255, 50), font2, 17);
		renderEngine->DrawText(memDC,  10, 40, fpsText, RGB(250, 255, 50), font2, 17);
		renderEngine->DrawText(memDC,  10, 60, wScaleText, RGB(250, 255, 50), font2, 17);
		renderEngine->DrawText(memDC,  10, 80, hScaleText, RGB(250, 255, 50), font2, 17);
		renderEngine->DrawText(memDC,  10, 100, player1PosText, RGB(250, 255, 50), font2, 17);
		renderEngine->DrawText(memDC,  10, 120, player2PosText, RGB(250, 255, 50), font2, 17);
		renderEngine->DrawText(memDC,  10, 140, gameTimeText, RGB(250, 255, 50), font2, 17);
		renderEngine->DrawText(memDC,  10, 160, startDelayTimerText, RGB(250, 255, 50), font2, 17);
	}

	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//
	if (bIsDeveggFound) 
	{
		renderEngine->DrawImg(memDC, cameoBitmap, 40, wHeight - 450);
		renderEngine->DrawText(memDC, 40, wHeight - 60, L"XXXXX Thank you for playing my game XXXXX", RGB(50, 255, 50), font2, 17);
		renderEngine->DrawText(memDC, 40, wHeight - 40, L"   XXXXXXXX Love, Klaus 🧛🏽‍♀️ XXXXXXXXXX   ", RGB(50, 255, 50), font2, 17);
		
	}
	
	//
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//
	if (bIs117Found && bIsGameOver && (bIsMultiPlayer || (!bIsMultiPlayer && (score1 > score2)))) 
	{
		renderEngine->DrawText(memDC, wWidth/2+40, wHeight - 60, L"XXXX Dear H, I'll meet you halfway to The Arcade XXXX", RGB(50, 255, 50), font2, 17);
		renderEngine->DrawText(memDC, wWidth / 2+40, wHeight - 40, L"           XXXXX Love, K. 🧛🏽‍♀️💟 XXXXX", RGB(50, 255, 50), font2, 17);
	}
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// 
	/////////////////////////////////////////////////////////////////////////////////////////////


	// Copy buffer to screen
	BitBlt(hdc, 0, 0, wWidth, wHeight, memDC, 0, 0, SRCCOPY);

}

void GameWindow::ResetGame()
{
	ballPosX = wWidth / 2;
	ballPosY = wHeight / 2;
	ballVelX = originalBallVelX * wScale;
	ballVelY = originalBallVelY *hScale;
	player1YPos = wHeight / 2 - paddleHeight / 2;
	player2YPos = wHeight / 2 - paddleHeight / 2;
	
	currentStartDelay = gameResetDelay;
	gameStartDelayTimer = currentStartDelay;
}

void GameWindow::OnGameOver()
{
	bIsGameOver = true;
	audioEngine->PlayClip(L"assets/audio/game-over");
	ResetGame();
	gameStartDelayTimer = 0.0f;

	if (score1 > score2) {
		winnerNum = 1;
	}
	else {
		winnerNum = 2;
	}
}

void GameWindow::RestartGame()
{
	ResetGame();
	audioEngine->PlayClip(L"assets/audio/restart");

	currentStartDelay = gameStartDelay;
	gameStartDelayTimer = currentStartDelay; //commemt
	score1 = 0;
	score2 = 0;
	serveTextColor = player1Color;
	bIsGameOver = false;
	bIsDeveggFound = false;
}

void GameWindow::CheckEggStats()
{
	// KONAMI
	if (GetAsyncKeyState(konamiCode[konamiIndex]) & 0x8000)
	{
		konamiIndex++;

		// If the whole sequence is entered correctly
		if (konamiIndex == konamiCode.size())
		{
			//powerModeEnabled = !powerModeEnabled; // Toggle power mode
			konamiIndex = 0; // Reset sequence
			bIsDeveggFound = true;
			OutputDebugString(L"Cheat Activated!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

			//audioEngine->StopMusic(L"bgm");
			// pGameWindow->audioEngine->PlayMusic(L"bg-music-2.mp3");

		}
	}
	else
	{
		konamiIndex = 0; // Reset if a wrong key is pressed
	}

	/// ORDER 117 /////////////////
	if (GetAsyncKeyState(code117[code117Index]) & 0x8000)
	{
		code117Index++;

		// If the whole sequence is entered correctly
		if (code117Index == code117.size())
		{
			//powerModeEnabled = !powerModeEnabled; // Toggle power mode
			code117Index = 0; // Reset sequence
			bIs117Found = true;
			winScore = eggWinScore;
			OutputDebugString(L"ORDER 117 ACTIVATED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

			audioEngine->StopMusic(L"bgm");
			audioEngine->PlayMusic(L"bgm-2");

		}
	}
	else
	{
		code117Index = 0; // Reset if a wrong key is pressed
	}
}

void GameWindow::CleanUp()
{
	CleanupBuffer();

	if (hwnd) {
		DestroyWindow(hwnd);
		hwnd = nullptr;
	}
	if (renderEngine) {
		delete renderEngine;

	}if (audioEngine) {
		delete audioEngine;
	}
	bIsRunning = false;
}
void GameWindow::CleanupBuffer()
{
	SelectObject(memDC, oldBitmap); // Restore previous state
	DeleteObject(memBitmap);
	DeleteDC(memDC);
}






