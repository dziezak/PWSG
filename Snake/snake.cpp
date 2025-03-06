#include "snake.h"
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <format>
std::wstring const app_snake::s_class_name{ L"Snake Window" };

bool app_snake::register_class()
{
	WNDCLASSEXW desc{};
	if (GetClassInfoExW(m_instance, s_class_name.c_str(), &desc) != 0)
		return true;
	desc = {   
		.cbSize = sizeof(WNDCLASSEXW),
		.lpfnWndProc = window_proc_static,
		.hInstance = m_instance,
		.hCursor = LoadCursorW(nullptr, L"IDC_ARROW"),
		.lpszClassName = s_class_name.c_str()
	};
	move_counter = 0;
	score = 0;
	return RegisterClassExW(&desc) != 0;
}

HWND app_snake::create_window() {
	int cell_size = 20;
	int width = columns * cell_size;
	int height = rows * cell_size;

	DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION |
		WS_BORDER | WS_MINIMIZEBOX;

	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, style, false);

	return CreateWindowExW(
		0,
		s_class_name.c_str(),
		L"Snake",
		style,
		CW_USEDEFAULT, CW_DEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		m_instance, 
		this);
}

app_snake::app_snake(HINSTANCE instance)
	:m_instance{ instance }, m_main{}
{
	register_class();
	m_main = create_window();
	head = { 0, 0 };
	board[head.first][head.second] = 1;
	food = { 10, 10 };
	board[food.first][food.second] = 2;
}

int app_snake::run(int show_command)
{
	ShowWindow(m_main, show_command);
	MSG msg{};
	BOOL result{};
	while ((result = GetMessageW(&msg, nullptr, 0, 0)) != 0)
	{
		if (result == -1)
			throw std::runtime_error{ "GetMessage failed" };
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return EXIT_SUCCESS;
}

LRESULT app_snake::window_proc_static(
	HWND window,
	UINT message,
	WPARAM wparam,
	LPARAM lparam)
{
	app_snake* app = nullptr;
	if (message == WM_NCCREATE)
	{
		auto p = reinterpret_cast<LPCREATESTRUCTW>(lparam);
		app = static_cast<app_snake*>(p -> lpCreateParams);
		SetWindowLongPtrW(window, GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(app));
	}
	else
	{
		app = reinterpret_cast<app_snake*>(
			GetWindowLongPtrW(window, GWLP_USERDATA));
	}
	if (app != nullptr)
	{
		return app -> window_proc(window, message, wparam, lparam);
	}
	return DefWindowProcW(window, message, wparam, lparam);
}

LRESULT app_snake::window_proc(
	HWND window,
	UINT message,
	WPARAM wparam,
	LPARAM lparam)
{
	switch (message)
	{
	case WM_CREATE:
		SetTimer(window, 1, 100, nullptr);
		return 0;
	case WM_CLOSE:
		DestroyWindow(window);
		return 0;
	case WM_DESTROY:
		if (window == m_main)
			PostQuitMessage(EXIT_SUCCESS);
		return 0;
	case WM_PAINT:
	{
		OutputDebugStringW(L"WM_PAINT triggerd\n");
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(window, &ps);
		draw_game(hdc);
		EndPaint(window, &ps);
		return 0;
	}
	case WM_KEYDOWN:
		switch (wparam) {
		case VK_UP: {
			direction = { -1, 0 }; 
			move_counter++;
			auto title = std::format(L"Move counter {}", move_counter);
			SetWindowTextW(window, title.c_str());
			break;
		}
		case VK_DOWN: {
			direction = { 1, 0 }; 
			move_counter++;
			auto title = std::format(L"Move counter {}", move_counter);
			SetWindowTextW(window, title.c_str());
			break;
		}
		case VK_LEFT: {
			direction = { 0, -1 }; 
			move_counter++;
			auto title = std::format(L"Move counter {}", move_counter);
			SetWindowTextW(window, title.c_str());
			break;
		}
		case VK_RIGHT: {
			direction = { 0, 1 }; 
			move_counter++;
			auto title = std::format(L"Move counter {}", move_counter);
			SetWindowTextW(window, title.c_str());
			break;
		}
		}
		update_game();
	default:
		return DefWindowProcW(window, message, wparam, lparam);
	}
}

void app_snake::update_game()
{
	auto new_head = head;
	new_head.first += direction.first;
	new_head.second += direction.second;
	if (is_game_over(new_head))
	{
		KillTimer(m_main, 1);
		auto endMessage = std::format(L"Game Over {}", score);
		MessageBoxW(m_main, endMessage.c_str(), L"Snake", MB_OK);
		return;
	}
	if (head.first == food.first && head.second == food.second)
	{
		score++;
		board[food.first][food.second] = 0;
		place_food();
	}
	else
	{
		board[head.first][head.second] = 0;
	}
	board[new_head.first][new_head.second] = 1;
	head = new_head;
	InvalidateRect(m_main, nullptr, false);
}

bool app_snake::is_game_over(std::pair<int, int> new_head)
{
	if (score == 10)
		return true;
	return false;
}

void app_snake::place_food()
{
	srand(static_cast<unsigned>(time(nullptr)));
	while (true)
	{
		int row = rand() % rows;
		int column = rand() % columns;
		if (board[row][column] == 0)
		{
			food = { row, column };
			board[row][column] = 2;
			return;
		}
	}
}

void app_snake::draw_game(HDC hdc)
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			RECT rect = { j * 20, i * 20, (j + 1) * 20, (i + 1) * 20 };
			if (board[i][j] == 1) {
				FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
			}
			else if (board[i][j] == 2) {
				FillRect(hdc, &rect, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
			}
			else {
				FillRect(hdc, &rect, (HBRUSH)GetStockObject(GRAY_BRUSH));
			}
		}
	}
}

