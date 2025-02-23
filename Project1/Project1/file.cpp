#include <windows.h>

LPCWSTR NazwaKlasy = L"Klasa Okienka";
MSG Komunikat;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//to terminate: taskkill /IM Project1.exe /F

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // WYPEŁNIANIE STRUKTURY
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE; // Dodaj CS_NOCLOSE
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // REJESTROWANIE KLASY OKNA
    if (!RegisterClassEx(&wc))
    {
        MessageBoxA(NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
            MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    // TWORZENIE OKNA
    HWND hwnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,  // Rozszerzony styl okna
        NazwaKlasy,              // Nazwa klasy okna
        L"Oto okienko",          // Tytuł okna
        WS_OVERLAPPEDWINDOW,     // Styl okna
        CW_USEDEFAULT, CW_USEDEFAULT,  // Pozycja okna
        1200, 600,               // Rozmiar okna
        NULL, NULL, hInstance, NULL    // Parametry dodatkowe
    );

    if (hwnd == NULL)
    {
        MessageBoxA(NULL, "Okno odmówiło przyjścia na świat!", "Ale kicha...", MB_ICONEXCLAMATION);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Pętla komunikatów
    while (GetMessage(&Komunikat, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Komunikat);
        DispatchMessage(&Komunikat);
    }

    return (int)Komunikat.wParam;
}

// OBSŁUGA ZDARZEŃ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
