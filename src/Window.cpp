#include "Window.h"
#include "Core.h"

Window::Window(HINSTANCE hInst) {
	_hInstance = hInst;

	const wchar_t* CLASS_NAME = L"Shutdown";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = _hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Auto Shutdown",
		WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPED,

		100, 100,
		300, 300,

		NULL, NULL, _hInstance, NULL
	);
}

Window::~Window() {
	const wchar_t* CLASS_NAME = L"Shutdown";
	UnregisterClass(CLASS_NAME, _hInstance);
}

void Window::ProcessMessages() {
	MSG msg = {};

	// I didn't figure out how to run the shutdown logic wether the
	// window is minimized or not without duplicating code
	while (true) {
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			ShutDownCheck();
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			break;
		}

		ShutDownCheck();
		Sleep(1);
	}
}

HWND Window::getHandle() { return _hWnd; }