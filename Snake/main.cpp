#include <Windows.h>
#include <string>
#include "snake.h"


int WINAPI wWinMain(HINSTANCE instance,
					HINSTANCE /*prevInstance*/,
					LPWSTR /*command_line*/,
					int show_command)

{
	app_snake app{ instance };
	return app.run(show_command);
}