#pragma once
#include <Windows.h>

class Window {
public:
	Window(HINSTANCE hInst);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void ProcessMessages();
	HWND getHandle();

private:
	HINSTANCE getInstance();

private:
	HINSTANCE _hInstance;
	HWND _hWnd;
};

