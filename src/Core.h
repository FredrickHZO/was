#pragma once
#include <string>
#include "Time.h"

#define ID_BUTTON_HOURS 1
#define ID_BUTTON_MINUTES 2
#define ID_BUTTON_CONFIRM 3

#define ID_TEXT_HOURS 4
#define ID_TEXT_MINUTES 5

SYSTEMTIME local;
Time* myTime = new Time();

bool CustomSystemShutDown() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	if (GetLastError() != ERROR_SUCCESS) {
		return false;
	}

	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
		SHTDN_REASON_FLAG_PLANNED)) {
		return false;
	}

	return true;
}

void ShutDownCheck() {
	if (myTime->isTimeConfirmed() == true) {
		GetLocalTime(&local);
		if (myTime->getHours() == local.wHour
			&& myTime->getMinutes() == local.wMinute) {
			CustomSystemShutDown();
		}
	}
}

void PopulateCombobox(HWND hHoursCombBox, HWND hMinutesCombBox) {
	for (int i = 0; i < 24; i++) {
		std::string hour = std::to_string(i);
		if (i < 10) {
			hour = "0" + hour;
		}
		std::wstring tmp = std::wstring(hour.begin(), hour.end());

		SendMessageW(hHoursCombBox, CB_ADDSTRING, 0, (LPARAM)tmp.c_str());
	}

	for (int i = 0; i < 60; i++) {
		std::string minutes = std::to_string(i);
		if (i < 10) {
			minutes = "0" + minutes;
		}
		std::wstring tmp = std::wstring(minutes.begin(), minutes.end());

		SendMessageW(hMinutesCombBox, CB_ADDSTRING, 0, (LPARAM)tmp.c_str());
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_CREATE: {
		HWND hHoursCombBox = CreateWindow(L"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
			30, 50, 100, 200, hWnd, (HMENU)ID_TEXT_HOURS, NULL, NULL);

		HWND hMinutesCombBox = CreateWindow(L"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
			30, 100, 100, 200, hWnd, (HMENU)ID_TEXT_MINUTES, NULL, NULL);

		PopulateCombobox(hHoursCombBox, hMinutesCombBox);

		// confirmation buttons
		HWND hours = CreateWindow(L"Button", L"Confirm hours",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			150, 50,
			120, 30,
			hWnd, (HMENU)ID_BUTTON_HOURS, NULL, NULL);
		HWND minutes = CreateWindow(L"Button", L"Confirm minutes",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			150, 100,
			120, 30,
			hWnd, (HMENU)ID_BUTTON_MINUTES, NULL, NULL);
		HWND confirm = CreateWindow(L"Button", L"Confirm time",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			90, 150,
			120, 30,
			hWnd, (HMENU)ID_BUTTON_CONFIRM, NULL, NULL);

		EnableWindow(confirm, FALSE);

		return 0;
	}

	case WM_COMMAND: {
		switch (LOWORD(wp)) {
		case ID_BUTTON_HOURS: {
			wchar_t buff[3];
			::GetDlgItemText(hWnd, ID_TEXT_HOURS, buff, 3);
			int val = _wtoi(buff);

			myTime->setHours(val);
			myTime->confirmHours();

			if (myTime->areMinConfirmed()) {
				HWND confirm = GetDlgItem(hWnd, ID_BUTTON_CONFIRM);
				EnableWindow(confirm, TRUE);
			}

			return 0;
		}

		case ID_BUTTON_MINUTES: {
			wchar_t buff[3];
			::GetDlgItemText(hWnd, ID_TEXT_MINUTES, buff, 3);
			int val = _wtoi(buff);

			myTime->setMinutes(val);
			myTime->confirmMinutes();

			if (myTime->areHoursConfirmed()) {
				HWND confirm = GetDlgItem(hWnd, ID_BUTTON_CONFIRM);
				EnableWindow(confirm, TRUE);
			}

			return 0;
		}

		case ID_BUTTON_CONFIRM: {
			std::string hours = std::to_string(myTime->getHours());
			std::string minutes = std::to_string(myTime->getMinutes());
	
			if (myTime->getHours() < 10) hours = "0" + hours;
			if (myTime->getMinutes() < 10) minutes = "0" + minutes;

			std::string str = "Windows will shut down at " +
				hours + ":" + minutes;

			std::wstring tmp = std::wstring(str.begin(), str.end());
			::MessageBox(hWnd, (LPCWSTR)tmp.c_str(), L"Confirm", MB_OK);

			myTime->confirmTime();
			return 0;
		}

		default:
			return 0;
		}
	}

	case WM_CLOSE: {
		DestroyWindow(hWnd);
		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}

	return 0;
}
