#include "RenderEngine.h"


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
