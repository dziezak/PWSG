#include "app.h"
#include <commdlg.h>
#include <string>

LRESULT CALLBACK DrawingApp::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    DrawingApp* app = reinterpret_cast<DrawingApp*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (msg) {
    case WM_CREATE:
        app = reinterpret_cast<DrawingApp*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        app->OnPaint(hdc);
        EndPaint(hwnd, &ps);
    } break;
    case WM_LBUTTONDOWN:
        app->OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MOUSEMOVE:
        app->OnMouseMove(LOWORD(lParam), HIWORD(lParam), wParam);
        break;
    case WM_COMMAND:
        app->OnCommand(wParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

DrawingApp::DrawingApp(HINSTANCE hInstance) : hInstance(hInstance), hwnd(NULL), currentColor(RGB(0, 0, 0)) {
    hPen = CreatePen(PS_SOLID, 2, currentColor);
}

bool DrawingApp::Run() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DrawingAppClass";
    RegisterClass(&wc);

    hwnd = CreateWindow(L"DrawingAppClass", L"Drawing Tool", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, this);

    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_CLEAR, L"Clear Canvas");
    AppendMenu(hFileMenu, MF_STRING, ID_COLOR_CHANGE, L"Change Color");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");

    SetMenu(hwnd, hMenu);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void DrawingApp::OnPaint(HDC hdc) {
    SelectObject(hdc, hPen);
    if (points.empty()) {
		TextOut(hdc, 10, 10, L"Canvas is cleared", 17);
        return;
    }
    for (size_t i = 1; i < points.size(); ++i) {
        MoveToEx(hdc, points[i - 1].x, points[i - 1].y, NULL);
        LineTo(hdc, points[i].x, points[i].y);
    }
}

void DrawingApp::OnLButtonDown(int x, int y) {
    points.clear();
    points.push_back({ x, y });
    InvalidateRect(hwnd, NULL, FALSE);
}

void DrawingApp::OnMouseMove(int x, int y, WPARAM wParam) {
    if (wParam & MK_LBUTTON) {
        points.push_back({ x, y });
        InvalidateRect(hwnd, NULL, FALSE); // tutaj TRUE czy FALSE?
        //UpdateWindow(hwnd);
    }
}

void DrawingApp::OnCommand(WPARAM wParam) {
    switch (LOWORD(wParam)) {
    case ID_FILE_CLEAR: // Clear canvas
        ClearCanvas();
        break;
    case ID_COLOR_CHANGE: // Change color
        ChangeColor(RGB(rand() % 256, rand() % 256, rand() % 256));
        break;
    }
}

void DrawingApp::ClearCanvas() {
    points.clear();
    HDC hdc = GetDC(hwnd);

    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &rect, hBrush);

    DeleteObject(hBrush);
    ReleaseDC(hwnd, hdc);

    std::wstring text = std::to_wstring((points.size()));
    MessageBox(hwnd, text.c_str(), L"Info", MB_OK);
    InvalidateRect(hwnd, NULL, FALSE);
    UpdateWindow(hwnd);
}

void DrawingApp::ChangeColor(COLORREF newColor) {
    currentColor = newColor;
    DeleteObject(hPen);
    hPen = CreatePen(PS_SOLID, 2, currentColor);
}