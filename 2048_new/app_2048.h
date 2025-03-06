#pragma once
#include <windows.h>
#include <string>
#include "board.h"
#include <chrono>

class app_2048
{
private:
	bool register_class();
	static std::wstring const s_class_name;
	static constexpr UINT_PTR s_timer = 1;
	static LRESULT CALLBACK window_proc_static(
		HWND window, UINT message,
		WPARAM wparam, LPARAM lparam);
	LRESULT window_proc(
		HWND window, UINT message,
		WPARAM wparam, LPARAM lparam);
	HINSTANCE m_instance;
	HWND m_main, m_popup;
	board m_board;
	HBRUSH m_field_brush;
	POINT m_screen_size;
	void on_window_move(HWND window, LPWINDOWPOS params);
	HWND create_window(DWORD style, HWND parent = nullptr, DWORD ex_style = 0);
	void update_transparency();
	std::chrono::time_point<std::chrono::system_clock> m_startTime;
	void on_timer();
	void on_command(WORD cmdID);
public:
	app_2048(HINSTANCE instance);
	int run(int show_command);
};