#pragma once

class UApplication;

class UWindow
{
public:
	UWindow();
	~UWindow();

	AkBool Initialize();
	AkBool InitApplication();
	AkI32 Run();

	LRESULT MemWndProc(HWND hWnd, AkU32 uMsg, WPARAM wParam, LPARAM lParam);

private:
	void CleanUp();

private:
	UApplication* _pApp = nullptr;
};

