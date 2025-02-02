#include <Windows.h>
#include "RenderEngine.h"



// Game state
//struct GameData {
//	int paddleHeight;
//	int player1Y;
//	int player2Y;
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
	GameWindow(HINSTANCE hInstance, int width, int height);
	~GameWindow();

private:
	HINSTANCE hInstance;
	HWND hwnd;
	int width, height;



public:
	int paddleWidth;
	int paddleHeight;
	int padding;
	int player1Y;
	int player2Y;
	int ballSize;
	int ballPosX;
	int ballPosY;
	int ballVelX;
	int ballVelY;
	int score1;
	int score2;

	bool bIsRunning = true;
	RenderEngine* renderEngine;
	bool Initialize();
	void RunGameLoop();
	void DrawGameScene(HDC hdc);
	void Update();
	void CleanUp();

};


