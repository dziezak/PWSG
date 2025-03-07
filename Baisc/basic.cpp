#include "basic.h"
#include <stdexcept>

std::wstring const BasicApp::s_class_name = L"BasicApp";

BasicApp::BasicApp(HINSTANCE instance) : m_instance(instance), m_window(nullptr) {
	if (!register_class()) {
		throw std::runtime_error("Failed to register window class");
	}
	m_window = create_window();
}

bool BasicApp :: register_class() {
	WNDCLASSEXW desc{};
	if (GetClassInfoExW(m_instance, s_class_name.c_str(), &desc)) {
		return true;
	}
	desc = {
		.cbSize = sizeof(WNDCLASSEXW),
		.lpfnWndProc = DefWindowProcW,
		.hInstance = m_instance,
		.hCursor = LoadCursorW(nullptr, IDC_ARROW),
		.lpszClassName = s_class_name.c_str()
	};
	return RegisterClassExW(&desc);
}

HWND BasicApp::create_window() {
	return CreateWindowExW(
		0, 
		s_class_name.c_str(), 
		L"BasicApp", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, 
		nullptr, 
		m_instance, 
		nullptr);
}

int BasicApp::run(int show_command) {
	if (!m_window) {
		throw std::runtime_error("Window creation failed");
	}
	ShowWindow(m_window, show_command);
	UpdateWindow(m_window);
	MSG msg{};
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return static_cast<int>(msg.wParam);
}


