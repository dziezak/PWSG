#include <windows.h>
#include <iostream>
#include <string>
#include "memory.h" // Include the header file

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MessageBox(NULL, L"Start programu", L"Debug", MB_OK);
	int gridSize = 4;
	/*
	std::wcout << L"Enter the grid size: ";
	std::wcin >> gridSize;
	*/
    
	if (gridSize < 2 || gridSize > 10)
	{
		std::wcout << L"Invalid grid size. Please enter a number between 2 and 10.";
		return 1;
	}
	MemoryGameApp app(hInstance, gridSize);
	app.Run();
	return 0;
}

/*
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    std::wstring message = L"Hello, World!";
    MessageBoxW(NULL, message.c_str(), L"Hello", MB_OK);
    return 0;

}
*/
