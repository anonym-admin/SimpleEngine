#include "pch.h"
#include "Window.h"
#include "Application.h"

HWND GhWnd;

LRESULT CALLBACK WndProc(HWND hwnd, AkU32 msg, WPARAM wParam, LPARAM lParam);

UWindow::UWindow()
{
	GWindow = this;
}

UWindow::~UWindow()
{
	CleanUp();
}

AkBool UWindow::Initialize()
{
	const wchar_t wcClassName[] = L"Window Application";
	const wchar_t wcWindowName[] = L"Simple Engine Prototype";

	WNDCLASS tWc = { };

	tWc.lpfnWndProc = WndProc;
	tWc.hInstance = ::GetModuleHandle(nullptr);
	tWc.lpszClassName = wcClassName;
	RegisterClass(&tWc);

	HWND hWnd = ::CreateWindowEx(0, wcClassName, wcWindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, tWc.hInstance, nullptr);
	::ShowWindow(hWnd, SW_SHOW);

	GhWnd = hWnd;

	return AK_TRUE;
}

AkBool UWindow::InitApplication()
{
#ifdef _DEBUG
	AkBool bEnableDebugLayer = AK_FALSE;
	AkBool bEnableGBV = AK_FALSE;
	AkI32 iInput = 0;
	wprintf_s(L"[Select Mode]\n");
	wprintf_s(L"1. Enable debug layer and GPU based validation.\n");
	wprintf_s(L"2. Not debug option.\n");
	scanf_s("%d", &iInput);
	if (iInput == 1)
	{
		bEnableDebugLayer = AK_TRUE;
		bEnableGBV = AK_TRUE;
	}
#else
	AkBool bEnableDebugLayer = AK_FALSE;
	AkBool bEnableGBV = AK_FALSE;
#endif

	_pApp = new UApplication(bEnableDebugLayer, bEnableGBV);;

	return AkBool();
}

AkI32 UWindow::Run()
{
	MSG tMsg = { };
	while (true)
	{
		if (PeekMessage(&tMsg, nullptr, 0, 0, PM_REMOVE))
		{
			if (tMsg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&tMsg);
			DispatchMessage(&tMsg);
		}
		else
		{
			_pApp->RunApplication();
		}
	}

	return static_cast<AkI32>(tMsg.wParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT UWindow::MemWndProc(HWND hWnd, AkU32 uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return AK_TRUE;
	
	switch (uMsg)
	{
	case WM_SIZE:
	{
		AkI32 width = LOWORD(lParam);  // Macro to get the low-order word.
		AkI32 height = HIWORD(lParam); // Macro to get the high-order word.

		//	if (g_pApplication)
		//	{
		//		RECT tRect = {};
		//		::GetClientRect(hWnd, &tRect);
		//		AkU32 uScreenWidth = tRect.right - tRect.left;
		//		AkU32 uScreenHeight = tRect.bottom - tRect.top;

		//		g_pApplication->UpdateWindowSize(uScreenWidth, uScreenHeight);
		//	}
	}
	break;
	case WM_MOUSEMOVE:
	{
		// printf("%d %d\n", LOWORD(lParam), HIWORD(lParam));
	}
	break;
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
	}
	break;
	}
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void UWindow::CleanUp()
{
	if (_pApp)
	{
		delete _pApp;
		_pApp = nullptr;
	}
	if (GhWnd)
	{
		DestroyWindow(GhWnd);
		GhWnd = nullptr;
	}
}

LRESULT WndProc(HWND hWnd, AkU32 uMsg, WPARAM wParam, LPARAM lParam)
{
	return GWindow->MemWndProc(hWnd, uMsg, wParam, lParam);
}

