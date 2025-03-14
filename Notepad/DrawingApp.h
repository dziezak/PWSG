#pragma once

#pragma once
#include <windows.h>
#include <vector>

class DrawingApp {
public:
    DrawingApp(HINSTANCE hInstance);
    bool Run();

protected:
    void OnPaint(HDC hdc);
    void OnLButtonDown(int x, int y);
    void OnMouseMove(int x, int y, WPARAM wParam);
    void OnCommand(WPARAM wParam);
    void ClearCanvas();
    void ChangeColor(COLORREF newColor);

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    HINSTANCE hInstance;
    HWND hwnd;
    HPEN hPen;
    COLORREF currentColor;
    std::vector<POINT> points;
};
