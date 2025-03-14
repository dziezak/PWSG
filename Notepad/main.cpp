#include <windows.h>
#include "DrawingApp.h"
#include "resource.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    DrawingApp app(hInstance);
    if (!app.Run()) {
        return -1;
    }
    return 0;
}
