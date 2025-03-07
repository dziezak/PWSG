#pragma once
#include <Windows.h>
#include <string>

class BasicApp {
private:
	static std::wstring const s_class_name;
	HINSTANCE m_instance;
	HWND m_window;

	bool register_class();
	HWND create_window();
public:
	BasicApp(HINSTANCE instance);
	int run(int show_command);
};
