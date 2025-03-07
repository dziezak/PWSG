#include "app_memory.h"
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <algorithm> // std::shuffle
#include <random>    // std::default_random_engine

std::wstring const app_memory::s_class_name = L"Memory";

app_memory::app_memory(HINSTANCE instance)
	:m_instance{instance}, m_main{}
{
	register_class();
	m_main = create_window();
	std::srand(std::time(nullptr));
	shuffle_board();
}


bool app_memory::register_class()
{
	WNDCLASSEXW desc{};
	if(GetClassInfoExW(m_instance, s_class_name.c_str(), &desc))
		return true;
	desc = {
		.cbSize = sizeof(WNDCLASSEXW),
		.lpfnWndProc = app_memory::window_proc_static,
		.hInstance = m_instance,
		.hCursor = LoadCursorW(nullptr, IDC_ARROW),
		.lpszClassName = s_class_name.c_str()
	};
	return RegisterClassExW(&desc) != 0;
}

HWND app_memory::create_window()
{
	return CreateWindowExW(
		0,
		s_class_name.c_str(),
		L"Memory",
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION
		| WS_BORDER | WS_MINIMIZEBOX,
		CW_USEDEFAULT,0, 
		500, 500,
		nullptr,
		nullptr,
		m_instance,
		this);
}

int app_memory::run(int show_command)
{
	if (!m_main)
		throw std::runtime_error{ "Failed to create window" };
	ShowWindow(m_main, show_command);
	MSG msg{};
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return EXIT_SUCCESS;
}

LRESULT app_memory::window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	app_memory* app = nullptr;
	if (message == WM_NCCREATE) {
		auto p = reinterpret_cast<CREATESTRUCTW*>(lparam);
		app = static_cast<app_memory*>(p->lpCreateParams);
		SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
	}
	else {
		app = reinterpret_cast<app_memory*>(GetWindowLongPtrW(window, GWLP_USERDATA));
	}
	if (app) {
		return app->window_proc(window, message, wparam, lparam);
	}
	return DefWindowProcW(window, message, wparam, lparam);
}


LRESULT app_memory::window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		auto hdc = BeginPaint(window, &ps);
		draw_game(hdc);
		EndPaint(window, &ps);
		return 0;
	}
	case WM_LBUTTONDOWN: {
		auto x = LOWORD(lparam) / 50;
		auto y = HIWORD(lparam) / 50;
		if (x < width && y < height && !matched[y][x]) {
			revealed_cards.push_back({ y, x });
			check_match();
		}
		InvalidateRect(window, nullptr, true);
		return 0;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	case WM_TIMER:
		reset_unmatched_pairs();
		InvalidateRect(window, nullptr, TRUE); // Od wie enie okna
		return 0;

	default:
		return DefWindowProcW(window, message, wparam, lparam);
	}
}

void app_memory::check_match() {
	if (revealed_cards.size() == 2) {
		auto first = revealed_cards[0];
		auto second = revealed_cards[1];
		if (board[first.first][first.second] == board[second.first][second.second]) {
			matched[first.first][first.second] = true;
			matched[second.first][second.second] = true;
		}
		revealed_cards.clear();
	}
}


void app_memory::shuffle_board() {
	// Przygotowanie kart (1-8 w parach)
	std::vector<int> cards;
	int num_pairs = height * width / 2;
	for (int i = 1; i <= num_pairs; ++i) {
		cards.push_back(i);
		cards.push_back(i);
	}

	// Tasowanie i przypisywanie do planszy
	std::random_device rd; //  r d o losowe
	std::mt19937 g(rd());  // Generator losowy Mersenne Twister
	std::shuffle(cards.begin(), cards.end(), g);

	int index = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			board[i][j] = cards[index++];
			matched[i][j] = false; // Brak dopasowania na pocz tku
		}
	}
}

void app_memory::draw_game(HDC hdc) {
	RECT rect{};
	GetClientRect(m_main, &rect);
	auto width = rect.right / this->width;
	auto height = rect.bottom / this->height;
	for (int i = 0; i < this->height; ++i) {
		for (int j = 0; j < this->width; ++j) {
			auto x = j * width;
			auto y = i * height;
			//auto card = board[i][j];
			rect = { x, y, x + width, y + height };
			bool is_revealed = matched[i][j] || std::find(revealed_cards.begin(), revealed_cards.end(), std::make_pair(i, j)) != revealed_cards.end();
			if (is_revealed) {
				HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255)); // bialy
				SelectObject(hdc, brush);
				Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
				DeleteObject(brush);

				wchar_t text[2];
				swprintf_s(text, L"%d", board[i][j]);
				SetBkMode(hdc, TRANSPARENT);
				DrawTextW(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else {
				HBRUSH brush = CreateSolidBrush(RGB(150, 150, 150)); // czarny
				SelectObject(hdc, brush);
				Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
				DeleteObject(brush);
			}
		}
	}
}
void app_memory::reset_unmatched_pairs() {
	if (revealed_cards.size() == 2) {
		auto first = revealed_cards[0];
		auto second = revealed_cards[1];

		if (board[first.first][first.second] != board[second.first][second.second]) {
			revealed_cards.clear();
		}
	}
}



