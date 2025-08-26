#pragma once

#include <string>

// This struct holds all the mutable data that represents the current state of the game.
// It contains no logic, ONLY DATA.

// Add enums as needed.
// Game states, player states, etc.
//enum MyEnum // state machines...
//{
//    STATE_IDLE,
//    STATE_PLAYING,
//    STATE_PAUSED,
//	  STATE_GAMEOVER
//};

struct GameState
{
	// GAMEPLAY STATES (variables that change during gameplay)

	// Player states
    float player1YPos = 0.0f;
    float player2YPos = 0.0f;

	// Ball states
    float ballPosX = 0.0f;
    float ballPosY = 0.0f;
    float ballVelX = 0.0f;
    float ballVelY = 0.0f;

	// Score states
    int score1 = 0;
    int score2 = 0;
    int winnerNum = 0;

    float gameTime = 0.0f;
    float gameStartDelayTimer = 0.0f;

    bool bGameHasStarted = false;
    bool bIsMultiPlayer = false;
    bool bIsGameOver = false;
    bool bIsDeveggFound = false;
    bool bIs117Found = false;

    // ... more states here.
};
