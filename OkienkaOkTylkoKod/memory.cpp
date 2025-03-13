#include "memory.h"
#include <time.h>
#include <random>
#include <algorithm>
#include <string>

LRESULT CALLBACK Card::CardProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Card* card = (Card*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (!card) {
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	switch (message) {
	case WM_LBUTTONDOWN:
		//MessageBox(hwnd, L"Odkryta karta!", L"Klik", MB_OK);
		if (!card->IsFaceUp()) {
			card->ShowCard();
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		if (card->IsFaceUp()) {
			std::wstring valueText = std::to_wstring(card->GetValue());
			TextOut(hdc, 30, 30, valueText.c_str(), valueText.length());
		}else {
			FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(GRAY_BRUSH));
			//std::wstring valueText = L"HELLO~";
			//TextOut(hdc, 30, 30, valueText.c_str(), valueText.length());
			//Rectangle(hdc, 0, 0, 90, 90);
		}
		EndPaint(hwnd, &ps);
	}
		 return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

Card::Card(int r, int c, int v) : row(r), col(c), isFaceUp(false), value(v), hwnd(NULL) {}

HWND Card::CreateCardWindow(HINSTANCE hInstance, HWND parent)
{

	hwnd = CreateWindowExW(0, L"CardClass",L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
		col * 100, row * 100, 80, 80, parent, NULL, hInstance, NULL);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
	return hwnd;
}

void Card::ShowCard() {
	if (!isFaceUp) {
		isFaceUp = true;
		InvalidateRect(hwnd, NULL, TRUE); // odswiezamy karte
	}
}

void Card::HideCard() {
	if (isFaceUp) {
		isFaceUp = false;
		InvalidateRect(hwnd, NULL, TRUE);
	}
}


MemoryGameApp::MemoryGameApp(HINSTANCE hInstance, int gridSize)
	: hInstance(hInstance), gridSize(gridSize) 
{
	const wchar_t CLASS_NAME[] = L"MemoryGameWindow";
	const wchar_t CARD_CLASS_NAME[] = L"CardClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	if (!RegisterClass(&wc)) {
		MessageBox(NULL, L"Blad rejestracji klasy!", L"Error", MB_OK);
		return;
	}

	WNDCLASS wcCard = {};
	wcCard.lpfnWndProc = Card::CardProc;
	wcCard.hInstance = hInstance;
	wcCard.lpszClassName = CARD_CLASS_NAME;
	if (!RegisterClass(&wcCard)) {
		MessageBox(NULL, L"Blad rejestracji klasy karty!", L"Error", MB_OK);
		return;
	}

	hwnd = CreateWindowExW(
		0, CLASS_NAME, L"Memory Game", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, gridSize * 100 + 20, gridSize * 100 + 40,
		NULL, NULL, hInstance, this);

	if (!hwnd) {
		MessageBox(NULL, L"Nie udalo sie stworzyc okna!", L"Error", MB_OK);
		return;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	std::vector<int> values;
	for (int i = 0; i <= 7; i++) {
		values.push_back(i);
		values.push_back(i);
	}

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(values.begin(), values.end(), g);



	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			cards.emplace_back(i, j, values.back());
			values.pop_back();
			cards.back().CreateCardWindow(hInstance, hwnd);
		}
	}
}

void MemoryGameApp::HandleFlippedCard(Card* card)
{
	flippedCards.push_back(card);
	if (flippedCards.size() == 2) {
		if (flippedCards[0]->IsFaceUp() && flippedCards[1]->IsFaceUp()) {
			if (flippedCards[0]->GetValue() == flippedCards[1]->GetValue()) {
				// Karty pasuj¹ - zostawiamy je odkryte
				MessageBox(hwnd, L"Pasuj¹!", L"Informacja", MB_OK);
			}
			else {
				// Karty nie pasuj¹ - ukrywamy je
				MessageBox(hwnd, L"Nie pasuj¹!", L"Informacja", MB_OK);
				flippedCards[0]->HideCard();
				flippedCards[1]->HideCard();
			}
		}
		flippedCards.clear();
	}
}


LRESULT CALLBACK MemoryGameApp::WindowProc(HWND hwnd, UINT uMSG, WPARAM wParam, LPARAM lParam)
{
	MemoryGameApp* app = nullptr;
	if (uMSG == WM_NCCREATE) {
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		app = (MemoryGameApp*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);
		app->hwnd = hwnd;
	}
	else {
		app = (MemoryGameApp*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}
	if (app) {
		return app->HandleMessage(uMSG, wParam, lParam);
	}
	else {
		return DefWindowProc(hwnd, uMSG, wParam, lParam);
	}
	//return app ? app->HandleMessage(uMSG, wParam, lParam) : DefWindowProc(hwnd, uMSG, wParam, lParam);
}

LRESULT MemoryGameApp::HandleMessage(UINT uMSG, WPARAM wParam, LPARAM lParam)
{
	switch (uMSG) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		//MessageBox(hwnd, L"Renderowanie okna", L"Debug", MB_OK);
		break;
	}
	return DefWindowProc(hwnd, uMSG, wParam, lParam);
}


void MemoryGameApp::Run()
{
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		//MessageBox(NULL, L"Rozpoczynam pêtlê zdarzeñ!", L"Info", MB_OK);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}



