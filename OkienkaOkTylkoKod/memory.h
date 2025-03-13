#pragma once
#include <windows.h>
#include <vector>

class Card {
private:
	int row, col;
	HWND hwnd;
	bool isFaceUp;
	int value; // wartosc od 0 do 7
public:
	static LRESULT CALLBACK CardProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	Card(int row, int col, int v);
	HWND CreateCardWindow(HINSTANCE hInstance, HWND parent);
	void ShowCard();
	void HideCard();
	bool IsFaceUp() {
		return isFaceUp;
	}
	void SetFaceUp(bool faceUp) { isFaceUp = faceUp; }
	int GetValue() {
		return value;
	}
};

class MemoryGameApp {
private:
	HINSTANCE hInstance;
	HWND hwnd;
	int gridSize;
	std::vector<Card> cards;
	std::vector<Card*> flippedCards;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMSG, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(UINT uMSG, WPARAM wParam, LPARAM lParam);
	
public:
	MemoryGameApp(HINSTANCE hInstance, int gridSize);
	void HandleFlippedCard(Card* card);
	void Run();
};
