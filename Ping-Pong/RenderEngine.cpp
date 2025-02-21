#include "RenderEngine.h"

RenderEngine::RenderEngine() {
	hFont = nullptr; // Initialize font handle
}

RenderEngine::~RenderEngine() {
	if (hFont) {
		DeleteObject(hFont);  // Free font resource
	}
}

void RenderEngine::DrawRect(HDC hdc, int left, int top, int right, int bottom, COLORREF rgbColor)
{
	HBRUSH brush = CreateSolidBrush(rgbColor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush); //sets new brush and returns old brush

	Rectangle(hdc, left,top,right,bottom);

	SelectObject(hdc, oldBrush);
	DeleteObject(brush);

	//Rectangle(hdc, 650, 150, 400, 350); //This uses new brush

}

void RenderEngine::DrawTriangle(HDC hdc, COLORREF rgbColor)
{
	POINT p1 = { 50,50 };
	POINT p2 = { 150,50 };
	POINT p3 = { 50,150 };

	POINT points[3] = {p1, p2, p3};
	HBRUSH brush = CreateSolidBrush(rgbColor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush); //sets new brush and returns old brush

	Polygon(hdc, points, 3);

	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
}

void RenderEngine::DrawEllipse(HDC hdc, int left, int top, int right, int bottom, COLORREF rgbColor)
{
	HBRUSH brush = CreateSolidBrush(rgbColor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush); //sets new brush and returns old brush

	Ellipse(hdc, left, top, right, bottom);

	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
}

void RenderEngine::DrawText(HDC hdc, int x, int y, const std::wstring& text, COLORREF color, const std::wstring& fontName, int fontSize) {

	// Create new font
	HFONT hNewFont = CreateFont(
		fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, fontName.c_str());

	if (hNewFont) {
		SelectObject(hdc, hNewFont);
	}

	SetTextColor(hdc, color);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, x, y, text.c_str(), text.length());

	// Clean up font 
	if (hNewFont) {
		DeleteObject(hNewFont);
	}
}

void RenderEngine::DrawImg(HDC hdc, HBITMAP hCameoBitmap, int left, int top) {

	HDC hdcImg = CreateCompatibleDC(hdc);
	SelectObject(hdcImg, hCameoBitmap);

	BITMAP bm;
	GetObject(hCameoBitmap, sizeof(BITMAP), &bm);

	//// Center the  image
	//int cameoX = screenWidth ;
	//int cameoY = screenHeight ;

	// Draw the image
	BitBlt(
		hdc,
		left,top,
		bm.bmWidth, bm.bmHeight,
		hdcImg,
		0, 0,
		SRCCOPY
	);

	DeleteDC(hdcImg);
}

//Monospac821 BT