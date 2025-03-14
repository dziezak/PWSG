#include "DrawingApp.h"
#include "resource.h"
#include <commdlg.h>

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

    hwnd = CreateWindow(L"DrawingAppClass", L"Notatnik z Opcjami Rysowania", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, this);

    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, L"New");
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT, L"Exit");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");

    HMENU hHelpMenu = CreateMenu();
    AppendMenu(hHelpMenu, MF_STRING, IDM_HELP_ABOUT, L"About");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, L"Help");

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
    points.push_back({ x, y });
    InvalidateRect(hwnd, NULL, FALSE);
}

void DrawingApp::OnMouseMove(int x, int y, WPARAM wParam) {
    if (wParam & MK_LBUTTON) {
        points.push_back({ x, y });
        InvalidateRect(hwnd, NULL, FALSE);
    }
}

void DrawingApp::OnCommand(WPARAM wParam) {
    switch (LOWORD(wParam)) {
    case IDM_FILE_NEW:
        ClearCanvas();
        break;
    case IDM_FILE_EXIT:
        PostQuitMessage(0);
        break;
    case IDM_HELP_ABOUT:
        DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, [](HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) -> INT_PTR {
            switch (message) {
            case WM_INITDIALOG:
                return (INT_PTR)TRUE;
            case WM_COMMAND:
                if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                    EndDialog(hDlg, LOWORD(wParam));
                    return (INT_PTR)TRUE;
                }
                break;
            }
            return (INT_PTR)FALSE;
            });
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
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);
}

void DrawingApp::ChangeColor(COLORREF newColor) {
    currentColor = newColor;
    DeleteObject(hPen);
    hPen = CreatePen(PS_SOLID, 2, currentColor);
}
