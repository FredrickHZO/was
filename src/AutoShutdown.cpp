#include <Windows.h>
#include "Window.h"

int WINAPI wWinMain(HINSTANCE hInst,
        HINSTANCE hPrevInst,
        PWSTR pCmdLine,
        int ncmdshow) {

    Window* window = new Window(hInst);

    ShowWindow(window->getHandle(), ncmdshow);
    window->ProcessMessages();

    delete window;
    return 0;
 }
