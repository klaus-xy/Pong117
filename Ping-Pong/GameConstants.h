#pragma once

// This file contains the [ GAME CONFIG ] used throughout the game. 
// These values can be adjusted to tweak gameplay, appearance, and behavior.
// It is recommended to keep all game settings centralized in this file for easy management. Hence,a namespace is used.
// So, Future Klaus, and to whom it may concern... Avoid scattering magic numbers throughout the codebase.

namespace GameConstants // Rename to GAME CONFIG
{
    // GAME WINDOW
    // Original Game Window dimensions for scaling. // Choose an aspect ratio when you start working on screen resizing.
    constexpr float WINDOW_WIDTH = 1200.0f; // 1920
    constexpr float WINDOW_HEIGHT = 600.0f; // 1080 
    constexpr float WINDOW_PADDING_X = 20.0f;
    constexpr float WINDOW_PADDING_Y = 20.0f;

	// PADDLE PROPERTIES
    constexpr float PADDLE_WIDTH = 20.0f;
    constexpr float PADDLE_HEIGHT = 100.0f;
    constexpr float PADDLE_SPEED = 600.0f;
    
	// BALL
    constexpr float BALL_SIZE = 20.0f;
    constexpr float MAX_BALL_SIZE = 26.0f;
    constexpr float START_BALL_VEL_X = 700.0f;
    constexpr float START_BALL_VEL_Y = 0.0f;
    constexpr float BALL_BOUNCINESS = 1.015f;
    constexpr float BALL_DEFLECT_MAGNITUDE = 10.0f;
    constexpr float BALL_COLLIDER_PADDING = 2.0f;

    // AI
    constexpr float AI_MOVE_THRESHOLD_EASY = 50.0f;

    // Game Rules
    constexpr int WIN_SCORE = 7;
    constexpr int EGG_WIN_SCORE = 117;
    constexpr float GAME_START_DELAY = 5.0f;
    constexpr float GAME_RESTART_DELAY = 3.0f;
    constexpr float GAME_RESET_DELAY = 2.0f;

	// Colors [ Note to self: colorRef is Windows specific. Use uint_32 and HEX to define colors in future updates ]
    constexpr COLORREF PLAYER_1_COLOR = RGB(20, 200, 200); // 0x14C8C8; // 
    constexpr COLORREF PLAYER_2_COLOR = RGB(255, 50, 50); // 0xFF3232; // 

    // Themes - consider theming system for diferent game states and seasons (christmas, halloween, easter eggs etc.)

}

// NB: constexpr => compile-time constant.For optimization.