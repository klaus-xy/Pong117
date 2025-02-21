#pragma once

#include <Windows.h>
#include <string>

class RenderEngine
{
public:
	RenderEngine();
	~RenderEngine();

public:
	int wWidth, wHeight;
	void DrawRect(HDC hdc, int left, int top, int right, int bottom, COLORREF rgbColor); // Draw Rectangle
	void DrawTriangle(HDC hdc, COLORREF rgbColor); // Draw triangle
	void DrawEllipse(HDC hdc, int left, int top, int right, int bottom, COLORREF rgbColor); // Draw triangle
	void DrawText(HDC hdc, int x, int y, const std::wstring& text, COLORREF color, const std::wstring& fontName, int fontSize);
	void DrawImg(HDC hdc, HBITMAP hCameoBitmap, int left, int top );
	

private:
	HFONT hFont;  // Handle to font
};

