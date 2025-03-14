#pragma once
#ifndef APP_H
#define APP_H
#define ID_FILE_CLEAR 1
#define ID_COLOR_CHANGE 2


#include <windows.h>
#include <vector>

class DrawingApp {
public:
    DrawingApp(HINSTANCE hInstance);
    bool Run();

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void OnPaint(HDC hdc);
    void OnLButtonDown(int x, int y);
    void OnMouseMove(int x, int y, WPARAM wParam);
    void OnCommand(WPARAM wParam);
    void ClearCanvas();
    void ChangeColor(COLORREF newColor);

    HINSTANCE hInstance;
    HWND hwnd;
    HPEN hPen;
    COLORREF currentColor;
    std::vector<POINT> points;
};

#endif
