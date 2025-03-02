#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#include <ImGuizmo.h>

#include <shlwapi.h>
#include <atlconv.h>
#pragma comment(lib, "Shlwapi.lib")

#include "interface/IRenderer.h"
#include "LandScapeEditor.h"

extern ImGuiContext* GImGui;

using namespace std;

/*
================
Global Variable
================
*/

HWND g_hWnd = nullptr;
unsigned int g_uScreenWidth = 0;
unsigned int g_uScreenHeight = 0;
bool g_bEnableDebugLayer = false;
bool g_bEnableGBV = false;
float g_fFps;
HMODULE g_hRendererDLL = nullptr;
IRenderer* g_pRenderer = nullptr;
void* g_pIrradianceTexHandle;
void* g_pSpecularTexHandle;
void* g_pBrdfTexHandle;
float g_fIBLStrength = 0.35f;
Vector3 g_vInitCamDir = Vector3(0.0f, 0.0f, 1.0f);
Vector3 g_vCurCamDir;
Vector3 g_vCurCamRight;
bool g_bKeyDown[256];
float g_fCamSpeed = 1.0f;
float g_fNdcX;
float g_fNdcY;
int g_iMouseX;
int g_iMouseY;
bool g_bFPV;
bool g_bLBtnDown;
LandScapeEditor* g_landScapeEditor = nullptr;

LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitRenderer();
void InitIBLResource(const wchar_t* wcIrradianceFilename, const wchar_t* wcSpecularFilename, const wchar_t* wcBrdfFilename);
void InitCamera();
void InitScene();
void Update(const float fDT);
void UpdateMouse(int iMouseX, int iMouseY);
void UpdateCamera(const float fDT);
void Render();
void CleanUpRenderer();
void CleanUpIBLResource();
void CleanUpScene();

/*
=================
Entry Point
=================
*/

int main()
{
	const wchar_t wcClassName[] = L"Window Application";
	const wchar_t wcWindowName[] = L"LandScape Editor";

	WNDCLASS tWc = { };
	tWc.lpfnWndProc = WndProc;
	tWc.hInstance = ::GetModuleHandle(nullptr);
	tWc.lpszClassName = wcClassName;
	RegisterClass(&tWc);

	HWND hWnd = ::CreateWindowEx(0, wcClassName, wcWindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, tWc.hInstance, nullptr);
	g_hWnd = hWnd;

	::ShowWindow(hWnd, SW_SHOW);

	RECT tRect = {};
	::GetClientRect(g_hWnd, &tRect);
	g_uScreenWidth = tRect.right - tRect.left;
	g_uScreenHeight = tRect.bottom - tRect.top;

	// Init Renderer.
	InitRenderer();

	// Init IBL.
	InitIBLResource(L"../../assets/skybox/SampleDiffuseHDR.dds", L"../../assets/skybox/SampleSpecularHDR.dds", L"../../assets/skybox/SampleBrdf.dds");

	// Init Camera.
	InitCamera();

	// Init Scene
	InitScene();

	// Run.
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
			// Start the Dear ImGui frame
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGuiIO& io = ImGui::GetIO();

			float fDT = 1 / io.Framerate;
			float fFps = io.Framerate;
			g_fFps = fFps;

			Update(fDT);

			g_pRenderer->BeginRender();

			Render();

			ImGui::Render();

			g_pRenderer->EndRender();
			g_pRenderer->Present();

		}
	}

	CleanUpScene();
	CleanUpIBLResource();
	CleanUpRenderer();

	return 0;
}

void InitRenderer()
{
	const wchar_t* wcRendererDLLFilename = nullptr;

#if defined(_M_AMD64)

#if defined(_DEBUG) || defined(DEBUG)
	wcRendererDLLFilename = L"SimpleEngine_renderer_x64d.dll";
#else
	wcRendererDLLFilename = L"SimpleEngine_renderer_x64.dll";
#endif

#elif defined(_M_IX86)

#if defined(_DEBUG) | defined(DEBUG)
	wcRendererDLLFilename = L"SimpleEngine_renderer_x86d.dll";
#else
	wcRendererDLLFilename = L"SimpleEngine_renderer_x86.dll";
#endif

#endif

	wchar_t wcErrorText[128] = {};
	AkU32 uErrorCode = 0;

	g_hRendererDLL = ::LoadLibrary(wcRendererDLLFilename);
	if (!g_hRendererDLL)
	{
		uErrorCode = ::GetLastError();
		swprintf_s(wcErrorText, L"Failed LoadLibrary[%s] - Error Code: %u \n", wcRendererDLLFilename, uErrorCode);
		::MessageBox(g_hWnd, wcErrorText, L"Error", MB_OK);
		__debugbreak();
	}

	DLL_CreateInstanceFuncPtr pDLL_CreateInstance = reinterpret_cast<DLL_CreateInstanceFuncPtr>(::GetProcAddress(g_hRendererDLL, "DLL_CreateInstance"));
	pDLL_CreateInstance(reinterpret_cast<void**>(&g_pRenderer));

	if (!g_pRenderer->Initialize(g_hWnd, g_bEnableDebugLayer, g_bEnableGBV))
	{
		__debugbreak();
	}

	// Use ImGui.
	g_pRenderer->BindImGui((void**)&GImGui);
}

void InitIBLResource(const wchar_t* wcIrradianceFilename, const wchar_t* wcSpecularFilename, const wchar_t* wcBrdfFilename)
{
	g_pIrradianceTexHandle = g_pRenderer->CreateCubeMapTexture(wcIrradianceFilename);
	g_pSpecularTexHandle = g_pRenderer->CreateCubeMapTexture(wcSpecularFilename);
	g_pBrdfTexHandle = g_pRenderer->CreateTextureFromFile(wcBrdfFilename, AK_FALSE);

	g_pRenderer->BindIBLTexture(g_pIrradianceTexHandle, g_pSpecularTexHandle, g_pBrdfTexHandle);
	g_pRenderer->SetIBLStrength(g_fIBLStrength);
}

void InitCamera()
{
	g_pRenderer->SetCameraPosition(0.0f, 25.0f, -3.0f);

	Vector3 vUp = Vector3(0.0f, 1.0f, 0.0f);
	g_vCurCamDir = g_vInitCamDir;
	g_vCurCamRight = vUp.Cross(g_vCurCamDir);
}

void InitScene()
{
	LandScapeEditor* landScapeEditor = new LandScapeEditor;
	landScapeEditor->Init();

	g_landScapeEditor = landScapeEditor;
}

void Update(const float fDT)
{
	UpdateCamera(fDT);

	g_landScapeEditor->Update(fDT);
}

void UpdateMouse(int iMouseX, int iMouseY)
{
	float fNdcX = (float)iMouseX / g_uScreenWidth * 2.0f - 1.0f;
	float fNdcY = (float)iMouseY / g_uScreenHeight * -2.0f + 1.0f;

	g_fNdcX = std::clamp(fNdcX, -1.0f, 1.0f);
	g_fNdcY = std::clamp(fNdcY, -1.0f, 1.0f);
}

void UpdateCamera(const float fDT)
{
	if (!g_bFPV)
	{
		return;
	}

	Vector3 vDeltaPos = Vector3(0.0f);
	Vector3 vUp = Vector3(0.0f, 1.0f, 0.0f);

	if (g_bKeyDown['W'])
	{
		vDeltaPos = g_vCurCamDir * (fDT * g_fCamSpeed);
	}
	if (g_bKeyDown['S'])
	{
		vDeltaPos = -g_vCurCamDir * (fDT * g_fCamSpeed);
	}
	if (g_bKeyDown['D'])
	{
		vDeltaPos = g_vCurCamRight * (fDT * g_fCamSpeed);
	}
	if (g_bKeyDown['A'])
	{
		vDeltaPos = -g_vCurCamRight * (fDT * g_fCamSpeed);
	}
	if (g_bKeyDown['Q'])
	{
		vDeltaPos = vUp * (fDT * g_fCamSpeed);
	}
	if (g_bKeyDown['E'])
	{
		vDeltaPos = -vUp * (fDT * g_fCamSpeed);
	}

	g_pRenderer->MoveCamera(vDeltaPos.x, vDeltaPos.y, vDeltaPos.z);

	// Camera È¸Àü
	AkF32 fYaw = DirectX::XM_2PI * g_fNdcX;
	AkF32 fPitch = -DirectX::XM_PIDIV2 * g_fNdcY;

	g_pRenderer->RotateYawPitchRollCamera(fYaw, fPitch, 0.0f);

	g_vCurCamDir = Vector3::Transform(g_vInitCamDir, Matrix::CreateFromYawPitchRoll(fYaw, fPitch, 0.0f));
	g_vCurCamRight = vUp.Cross(g_vCurCamDir);
}

void Render()
{
	g_landScapeEditor->Render();
}

void CleanUpRenderer()
{
	g_pRenderer->UnBindImGui();

	if (g_pRenderer)
	{
		g_pRenderer->Release();
		g_pRenderer = nullptr;
	}
}

void CleanUpIBLResource()
{
	g_pRenderer->DestroyTexture(g_pBrdfTexHandle);
	g_pRenderer->DestroyTexture(g_pSpecularTexHandle);
	g_pRenderer->DestroyTexture(g_pIrradianceTexHandle);
}

void CleanUpScene()
{
	if (g_landScapeEditor)
	{
		delete g_landScapeEditor;
		g_landScapeEditor = nullptr;
	}
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WndProc(HWND hWnd, AkU32 uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_SIZE:
	{
		AkI32 width = LOWORD(lParam);  // Macro to get the low-order word.
		AkI32 height = HIWORD(lParam); // Macro to get the high-order word.
	}
	break;
	case WM_KEYDOWN:
	{
		cout << wParam << endl;

		g_bKeyDown[wParam] = true;

		if ('F' == wParam)
		{
			g_bFPV = !g_bFPV;
		}
	}
	break;
	case WM_KEYUP:
	{
		g_bKeyDown[wParam] = false;
	}
	break;
	case WM_MOUSEMOVE:
	{
		g_iMouseX = LOWORD(lParam);
		g_iMouseY = HIWORD(lParam);

		UpdateMouse(g_iMouseX, g_iMouseY);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		g_bLBtnDown = true;
	}
	break;
	case WM_LBUTTONUP:
	{
		g_bLBtnDown = false;
	}
	break;
	case WM_DESTROY:
	{
		::PostQuitMessage(999);
	}
	break;
	}
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}
