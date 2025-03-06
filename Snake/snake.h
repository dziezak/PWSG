#pragma once
#include <windows.h>
#include <string>
#include <deque>
class app_snake
{
private:
	bool register_class();
	static std::wstring const s_class_name;
	static LRESULT CALLBACK window_proc_static(
		HWND window, UINT message,
		WPARAM wparam, LPARAM lparam);
	LRESULT window_proc(
		HWND window, UINT message,
		WPARAM wparam, LPARAM lparam);

	HWND create_window();
	HINSTANCE m_instance;
	HWND m_main;

	static constexpr int rows = 20;
	static constexpr int columns = 20;
	int board[rows][columns];
	std::pair<int, int> direction;
	int move_counter;
	std::pair<int, int> head;
	int score;

	void draw_game(HDC dc);
	void update_game();
	bool is_game_over(std::pair<int, int> new_head);
	void place_food();

    //std::deque<std::pair<int, int>> snake;
	std::pair<int, int> food;
public:
	app_snake(HINSTANCE instance);
	int run(int show_command);
};