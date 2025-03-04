#include "pch.h"
#include "Application.h"
#include "Timer.h"
#include "GameInput.h"
#include "SceneManager.h"
#include "EditorManager.h"
#include "EventHandler.h"
#include "StartScene.h"
#include "MenuScene.h"
#include "LoadingScene.h"
#include "ModelEditor.h"
#include "MapEditor.h"
#include "Camera.h"
#include "Font.h"

UApplication::UApplication(AkBool bEnableDebugLayer, AkBool bEnableGBV)
{
	Initialize(bEnableDebugLayer, bEnableGBV);
}

UApplication::~UApplication()
{
	CleanUp();
}

AkBool UApplication::Initialize(AkBool bEnableDebugLayer, AkBool bEnableGBV)
{
	AkBool bResult = AK_FALSE;

	// Init Renderer.
	if (!InitRenderer(bEnableDebugLayer, bEnableGBV))
	{
		__debugbreak();
		return AK_FALSE;
	}
	
	// Init Scene.
	if (!InitScene())
	{
		__debugbreak();
		return AK_FALSE;
	}

	// Init Editor.
	if (!InitEditor())
	{
		__debugbreak();
		return AK_FALSE;
	}

	// Reset Timer.
	GTimer->Reset();

	// Init Camera Position.
	Vector3 vCamPos = Vector3(0.0f, 0.0f, -5.0f);
	Vector3 vCamDir = Vector3(0.0f); // Init Direction : (0.0f, 0.0f, 1.0f)
	_pCamera = new UCamera(&vCamPos, &vCamDir);
	_pCamera->Mode = UCamera::CamMode::EDITOR;

	_pFont = new UFont(256.0f, 16.0f, L"Consolas", 16.0f);

	return AK_TRUE;
}

void UApplication::RunApplication()
{
	Update();

	if(!_bEnableEditor)
		_pCamera->Update();

	Render();

	GEventHandler->Excute();
}

void UApplication::CleanUp()
{
	if (_pFont)
	{
		delete _pFont;
		_pFont = nullptr;
	}
	if (_pCamera)
	{
		delete _pCamera;
		_pCamera = nullptr;
	}

	CleanUpEditor();
	CleanUpRenderer();
}

AkBool UApplication::InitRenderer(AkBool bEnableDebugLayer, AkBool bEnableGBV)
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

	_hRendererDLL = ::LoadLibrary(wcRendererDLLFilename);
	if (!_hRendererDLL)
	{
		uErrorCode = ::GetLastError();
		swprintf_s(wcErrorText, L"Failed LoadLibrary[%s] - Error Code: %u \n", wcRendererDLLFilename, uErrorCode);
		::MessageBox(GhWnd, wcErrorText, L"Error", MB_OK);
		__debugbreak();
	}

	IRenderer* pRenderer = nullptr;
	DLL_CreateInstanceFuncPtr pDLL_CreateInstance = reinterpret_cast<DLL_CreateInstanceFuncPtr>(::GetProcAddress(_hRendererDLL, "DLL_CreateInstance"));
	pDLL_CreateInstance(reinterpret_cast<void**>(&pRenderer));

	if (!pRenderer->Initialize(GhWnd, bEnableDebugLayer, bEnableGBV))
	{
		__debugbreak();
		return AK_FALSE;
	}

	RENDERER = pRenderer;

	return AK_TRUE;
}

AkBool UApplication::InitScene()
{
	GSceneManager->AddScene(L"Start", new UStartScene());
	GSceneManager->AddScene(L"Menu", new UMenuScene());
	GSceneManager->AddScene(L"Loading", new ULoadingScene());

	UScene* pCurScene = GSceneManager->SetCurrentScene(L"Start");
	pCurScene->BeginScene();

	return AK_TRUE;
}

void UApplication::Update()
{
#ifdef _DEBUG
	Debug();
#endif

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();

	GGameInput->Update();
	GTimer->Tick();

	GSceneManager->Update();

	if(_bEnableEditor)
		GEditorManager->Update();
}

void UApplication::Render()
{
	RENDERER->BeginRender();

	GSceneManager->Render();

	if (_bEnableEditor)
		GEditorManager->Render();

	_pFont->Render(L"Fps: ", 0, 0);

	ImGui::Render();

	RENDERER->EndRender();
	RENDERER->Present();
}

void UApplication::RenderSystemInfo()
{
	
}

void UApplication::CleanUpRenderer()
{
	if (RENDERER)
	{
		RENDERER->Release();
		RENDERER = nullptr;
	}
	if (_hRendererDLL)
	{
		// Because of CRT Debug check
#ifndef _DEBUG
		::FreeLibrary(_hRendererDLL);
		_hRendererDLL = nullptr;
#endif
	}
}

AkBool UApplication::InitEditor()
{
	// Use ImGui.
	RENDERER->BindImGui((void**)&GImGui);

	GEditorManager->AddEditor(L"Model", new UModelEditor());
	GEditorManager->AddEditor(L"Map", new UMapEditor());

	return AK_TRUE;
}

void UApplication::CleanUpEditor()
{
	RENDERER->UnBindImGui();
}

#ifdef _DEBUG
void UApplication::Debug()
{
	if (KEY_DOWN(KEY_INPUT_F5))
	{
		EventHandle_t tEvent = {};
		tEvent.eType = EVENT_TYPE::CHANGE_SCENE_TO_EDITOR;
		tEvent.pObj = this;
		GEventHandler->AddEvent(&tEvent);
	}
	if (KEY_DOWN(KEY_INPUT_F6))
	{
		EventHandle_t tEvent = {};
		tEvent.eType = EVENT_TYPE::CHANGE_EDITOR_TO_SCENE;
		tEvent.pObj = this;
		GEventHandler->AddEvent(&tEvent);
	}
	if (_bEnableEditor && KEY_DOWN(KEY_INPUT_F7))
	{
		EventHandle_t tEvent = {};
		tEvent.eType = EVENT_TYPE::CHANGE_MODEL_EDITOR;
		GEventHandler->AddEvent(&tEvent);
	}
	if (_bEnableEditor && KEY_DOWN(KEY_INPUT_F8))
	{
		EventHandle_t tEvent = {};
		tEvent.eType = EVENT_TYPE::CHANGE_MAP_EDITOR;
		GEventHandler->AddEvent(&tEvent);
	}
}
#endif