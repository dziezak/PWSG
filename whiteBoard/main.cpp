#include <windows.h>
#include "app.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    DrawingApp app(hInstance);
    return app.Run() ? 0 : 1;
}
