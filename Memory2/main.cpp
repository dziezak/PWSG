#include <windows.h>
#include "memory.h"
#include "app_memory.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	app_memory app(hInstance);
	return app.run(nCmdShow);
}

