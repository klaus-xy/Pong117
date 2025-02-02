#pragma once

#include <Windows.h>

class RenderEngine
{
public:
	/*RenderEngine();
	~RenderEngine();*/

public:
	int width, height;
	void DrawRect(HDC hdc, int left, int top, int right, int bottom, COLORREF rgbColor); // Draw Rectangle
	void DrawTriangle(HDC hdc, COLORREF rgbColor); // Draw triangle
	void DrawEllipse(HDC hdc, int left, int top, int right, int bottom, COLORREF rgbColor); // Draw triangle
};

