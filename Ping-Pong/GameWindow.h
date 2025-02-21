#include <Windows.h>
#include "RenderEngine.h"
#include "AudioEngine.h"



// Game state
//struct GameData {
//	int paddlewHeight;
//	int player1YPos;
//	int player2YPos;
//	int ballX;
//	int ballY;
//	int ballSpeedX;
//	int ballSpeedY;
//	int score1;
//	int score2;
//};

class GameWindow
{
public:
	GameWindow(HINSTANCE hInstance, float width, float height);
	~GameWindow();

private:
	HINSTANCE hInstance;
	HWND hwnd;
	int wWidth, wHeight;


	// Game time calculations
	LARGE_INTEGER frequency;  // Timer frequency
	LARGE_INTEGER lastFrameTime;  // Last frame time
	float deltaTime;          // Time difference between frames
	int frameCount;
	double fps;
	double timeElapsed; // Timer for FPS calculation



	// Buffer
	HDC memDC;
	HBITMAP memBitmap;
	HBITMAP oldBitmap; 
	HBITMAP cameoBitmap = NULL; 

	// GAMEPLAY SETTINGS
	const float gameStartDelay = 5.0f;
	const float gameResetDelay = 2.0f;
	float currentStartDelay;

public:

	float paddleWidth;
	float paddleHeight;
	float paddleSpeed;
	float paddingX, paddingY;	// Screen padding value
	float player1YPos;
	float player2YPos;
	float ballSize;
	float maxBallSize;
	float ballPosX;
	float ballPosY;
	float ballVelX;
	float ballVelY;
	float ballBaseVel;
	float ballBounciness;
	float deflectMagnitude;
	float startDelay;
	int score1;
	int score2;
	int winScore;
	int winnerNum;
	const int eggWinScore = 117;

	float originalHeight;
	float originalWidth;
	float hScale; // Height factor to scale property based on current screen height
	float wScale; // Width factor to scale property based on current screen height
	float textScale; // Scale factor for texts
	float originalPaddleWidth;
	float originalPaddleHeight;
	float originalPaddleSpeed;
	float originalBallSize;
	float originalBallVelX;
	float originalBallVelY;

	float colliderPadding;


	float aiMoveThreshold;
	COLORREF player1Color; 
	COLORREF player2Color; 
	COLORREF serveTextColor; 
	std::wstring font1;
	std::wstring font2;

	bool bIsRunning;
	bool bGameHasStarted;
	bool bIsMultiPlayer;
	bool bIsGameOver;
	bool canShowHelp;
	bool bIsDeveggFound;
	bool bIs117Found;

	// DEBUG
	bool canShowStats;
	float gameTime;
	float gameStartDelayTimer;
	bool InitGameWindow();
	void InitGameScene();
	void OnResize(float newWidth, float newHeight);
	void RunGameLoop();
	void InitBuffer(HDC hdc);
	void CleanupBuffer();
	void DrawGameScene(HDC);
	void ResetGame();
	void OnGameOver();
	void RestartGame();
	void CheckEggStats();

	// Handle Input
	void Input();


	// MOVE BALL
	void Move();

	void Collision();
	void Update();
	void CleanUp();

	void UpdateDeltaTime();
	void UpdateFPS();

	float GetDeltaTime() const { return deltaTime; }

	RenderEngine* renderEngine;
	AudioEngine* audioEngine;

};


