#pragma once
#include <Windows.h>
#include <vector>
#include <utility>
#include <string>

class app_memory {
private:
	static std::wstring const s_class_name;
	static LRESULT CALLBACK window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT window_proc(HWND window, UINT message, WPARAM wparam,  LPARAM lparam);

	bool register_class();
	HWND create_window();
	void draw_game(HDC hdc);
	void reset_unmatched_pairs();
	void check_match();
	void shuffle_board();

	HINSTANCE m_instance;
	HWND m_main;
	std::vector<std::pair<int, int>> revealed_cards;
	static const int height = 10;
	static const int width = 10;
	bool matched[height][width];
	int board[height][width];

public:
	app_memory(HINSTANCE instance);
	int run(int show_command);


};
