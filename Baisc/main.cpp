#include "basic.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    BasicApp app(hInstance);
    return app.run(nCmdShow);
}
