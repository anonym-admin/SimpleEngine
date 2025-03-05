#include "pch.h"
#include "Application.h"
#include "Timer.h"
#include "GameInput.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "EditorManager.h"
#include "EventHandler.h"
#include "UIManager.h"
#include "Camera.h"
#include "TextUI.h"
#include "PanelUI.h"
#include "BtnUI.h"
#include "InputUI.h"

#include "LoadingScene.h"
#include "InGameScene.h"

#include "ModelEditor.h"
#include "MapEditor.h"

#include "Sound.h"

/*
===============
Application
===============
*/

Application::Application()
{
}

Application::~Application()
{
	CleanUp();
}

AkBool Application::InitApplication(AkBool bEnableDebugLayer, AkBool bEnableGBV)
{
	srand((AkU32)time(nullptr));

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

	IRenderer* pRenderer = nullptr;
	_hRendererDLL = ::LoadLibrary(wcRendererDLLFilename);
	if (!_hRendererDLL)
	{
		uErrorCode = ::GetLastError();
		swprintf_s(wcErrorText, L"Failed LoadLibrary[%s] - Error Code: %u \n", wcRendererDLLFilename, uErrorCode);
		::MessageBox(GhWnd, wcErrorText, L"Error", MB_OK);
		__debugbreak();
	}

	DLL_CreateInstanceFuncPtr pDLL_CreateInstance = reinterpret_cast<DLL_CreateInstanceFuncPtr>(::GetProcAddress(_hRendererDLL, "DLL_CreateInstance"));
	pDLL_CreateInstance(reinterpret_cast<void**>(&pRenderer));

	if (!pRenderer->Initialize(GhWnd, bEnableDebugLayer, bEnableGBV))
	{
		__debugbreak();
	}

	GRenderer = pRenderer;
	GRenderer->BindImGui((void**)&GImGui);

	//////////////////////////////////////////////////////////

	//_pSysTextUI = new UTextUI;
	//if (!_pSysTextUI->Initialize(this, 256, 32, L"Consolas", 10))
	//{
	//	__debugbreak();
	//	return AK_FALSE;
	//}
	//_pSysTextUI->SetPosition(10, 10);
	//_pSysTextUI->SetScale(1.0f, 1.0f);
	//_pSysTextUI->SetFontColor(&_vSysFontColor);

	//_pUIManager->AddUI(_pSysTextUI, UI_OBJECT_TYPE::UI_OBJ_SYS_INFO_TEXT);
	//_pUIManager->OnUI(UI_OBJECT_TYPE::UI_OBJ_SYS_INFO_TEXT);

	//_pDynamicTextUI = new UInputUI;
	//if (!_pDynamicTextUI->Initialize(this, 256, 32, L"Consolas", 10))
	//{
	//	__debugbreak();
	//	return AK_FALSE;
	//}
	//_pDynamicTextUI->SetPosition(10, 500);
	//_pDynamicTextUI->SetScale(1.0f, 1.0f);
	//_pDynamicTextUI->SetFontColor(&_vDynamicTextFontColor);
	//_pDynamicTextUI->SetDrawBackGround(AK_TRUE);

	//_pUIManager->AddUI(_pDynamicTextUI, UI_OBJECT_TYPE::UI_OBJ_CHAT_INPUT_TEXT);
	//_pUIManager->OnUI(UI_OBJECT_TYPE::UI_OBJ_CHAT_INPUT_TEXT);

	//UTextUI* pStaticTextUI = new UTextUI;
	//if (!pStaticTextUI->Initialize(this, 256, 32, L"Consolas", 10))
	//{
	//	__debugbreak();
	//	return AK_FALSE;
	//}
	//pStaticTextUI->SetPosition(10, 32 + 10 + 10);
	//pStaticTextUI->SetScale(1.0f, 1.0f);
	//pStaticTextUI->SetFontColor(&_vSysFontColor);
	//pStaticTextUI->WriteText(L"Test Static Text\n");

	//_pUIManager->AddUI(pStaticTextUI, UI_OBJECT_TYPE::UI_OBJ_TEST_STATIC_TEXT);

	//UPanelUI* pTextureUI = new UPanelUI;
	//if (!pTextureUI->Initialize(this, L"../../assets/ui_01.dds", 0, 0, 2545, 1867))
	//{
	//	__debugbreak();
	//	return AK_FALSE;
	//}
	//pTextureUI->SetPosition(500, 10);
	//pTextureUI->SetScale(0.1f, 0.2f);
	//pTextureUI->SetDrawBackGround(AK_TRUE);
	//pTextureUI->SetResolution((AkU32)(0.1f * 2545), (AkU32)(0.2f * 1867));

	//_pUIManager->AddUI(pTextureUI, UI_OBJECT_TYPE::UI_OBJ_EXIT);

	//UBtnUI* pBtnUI = new UBtnUI;
	//if (!pBtnUI->Initialize(this, L"../../assets/Exit_Btn.dds", 0, 0, 225, 49))
	//{
	//	__debugbreak();
	//	return AK_FALSE;
	//}
	//pBtnUI->SetRelativePosition(10, 10);
	//pBtnUI->SetScale(1.0f, 1.0f);
	//pBtnUI->SetDrawBackGround(AK_TRUE);
	//pBtnUI->SetResolution(225, 49);
	//pBtnUI->SetClickFunc(&UApplication::ExitGame);

	//pTextureUI->AddChildUI(pBtnUI);

	// Common Sprite and Font.
	GCommonSprite = GRenderer->CreateSpriteObject();
	GCommonFont = GRenderer->CreateFontObject(L"Consolas", 12);

	// Create texture for system info sprite.
	_uTextTextureWidth = 516;
	_uTextTextureHeight = 32;
	_pTextTextureImage = (AkU8*)malloc(_uTextTextureWidth * _uTextTextureHeight * 4);
	_pSysTextureHandle = GRenderer->CreateDynamicTexture(_uTextTextureWidth, _uTextTextureHeight);
	memset(_pTextTextureImage, 0, _uTextTextureWidth * _uTextTextureHeight * 4);

	// Reset timer.
	GTimer->Reset();

	// Init Scene.
	InitScene();

	// Init Editor.
	InitEditor();

	return AK_TRUE;
}

AkBool Application::InitScene()
{
	GSceneManager->AddScene(SCENE_TYPE::SCENE_TYPE_LOADING, new ULoadingScene());
	GSceneManager->AddScene(SCENE_TYPE::SCENE_TYPE_INGANE, new UInGameScene());

	GSceneManager->BindCurrentScene(SCENE_TYPE::SCENE_TYPE_LOADING)->BeginScene();

	return AK_TRUE;
}

AkBool Application::InitEditor()
{
	GEditorManager->AddEditor(EDITOR_TYPE::EDITOR_TYPE_MODEL, new ModelEditor());
	GEditorManager->AddEditor(EDITOR_TYPE::EDITOR_TYPE_MAP, new MapEditor());

	return AK_TRUE;
}

void Application::RunApplication()
{
	GTimer->Tick();

	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();

	Update();

	GRenderer->UpdateCascadeOrthoProjMatrix();

	// Shadow Pass.
	for (AkU32 i = 0; i < 5; i++)
	{
		GRenderer->BeginCasterRenderPreparation();

		GSceneManager->RenderShadow();

		GRenderer->EndCasterRenderPreparation();

	}

	// Begin render.
	GRenderer->BeginRender();

	Render();

	ImGui::Render();

	// End render.
	GRenderer->EndRender();
	GRenderer->Present();

	// Evcute the event handler.
	GEventHandler->Excute();

	CalculateFrameRate();
}

AkBool Application::UpdateWindowSize(AkU32 uScreenWidth, AkU32 uScreenHeight)
{
	AkBool bResult = AK_FALSE;

	if (GRenderer)
	{
		bResult = GRenderer->UpdateWindowSize(uScreenWidth, uScreenHeight);
	}

	return bResult;
}

void Application::SetVSync(AkBool bUseVSync)
{
	_bUseVSync = bUseVSync;

	GRenderer->SetVSync(_bUseVSync);
}

void Application::CleanUp()
{
	if (_pTextTextureImage)
	{
		free(_pTextTextureImage);
		_pTextTextureImage = nullptr;
	}
	if (_pSysTextureHandle)
	{
		GRenderer->DestroyTexture(_pSysTextureHandle);
		_pSysTextureHandle = nullptr;
	}
	if (GCommonFont)
	{
		GRenderer->DestroyFontObject(GCommonFont);
		GCommonFont = nullptr;
	}
	if (GCommonSprite)
	{
		GCommonSprite->Release();
		GCommonSprite = nullptr;
	}
	if (GRenderer)
	{
		GRenderer->UnBindImGui();
		GRenderer->Release();
		GRenderer = nullptr;
	}
	if (_hRendererDLL)
	{
#ifndef _DEBUG
		::FreeLibrary(_hRendererDLL);
		_hRendererDLL = nullptr;
#endif
	}
}

void Application::Update()
{
	static AkF32 fTimeElapsed = 0.0f;
	fTimeElapsed += DT;

	// Not Vsync => 60fps 고정을 위한 처리
	//if (fTimeElapsed < 0.016f)
	//{
	//	return;
	//}

	// Update game input.
	GGameInput->Update();;

	// Update env.
	UpdateEnv();

	// Update Scene.
	GSceneManager->Update();

	// Final Update Scene.
	GSceneManager->FinalUpdate();

	// Update Editor.
	GEditorManager->Update();

	// Final Update Editor.
	GEditorManager->FinalUpdate();

	// Update Collision manager.
	GCollisionManager->Update();

	// Update status text
	UpdateText();

	// Update UI Manager.
	GUIManager->Update();

	// Update sound manager.
	GSoundManager->Update();

	// 게임 종료.
	if (GGameInput->KeyFirstDown(KEY_INPUT_ESCAPE))
	{
		ExitGame();
	}

	fTimeElapsed = 0.0f;
}

void Application::UpdateEnv()
{
	// VSync.
	if (GGameInput->KeyFirstDown(KEY_INPUT_F10))
	{
		_bUseVSync = !_bUseVSync;
		SetVSync(_bUseVSync);
	}

	// Change Scene To Model Editor.
	if (GGameInput->KeyFirstDown(KEY_INPUT_F2) && !GEditorManager->GetCurrentEditor())
	{
		EventHandle_t tEvent = {};
		tEvent.eEventType = EVENT_TYPE::EVENT_TYPE_SCENE_TO_MODEL_EDITOR_CHANGE;
		tEvent.tSceneEditorChangeParam.eScene = GSceneManager->GetCurrentSceneType();
		tEvent.tSceneEditorChangeParam.eEditor = EDITOR_TYPE::EDITOR_TYPE_MODEL;
		GEventHandler->AddEvent(&tEvent);
	}

	// Change Scene To Map Editor.
	if (GGameInput->KeyFirstDown(KEY_INPUT_F3) && !GEditorManager->GetCurrentEditor())
	{
		EventHandle_t tEvent = {};
		tEvent.eEventType = EVENT_TYPE::EVENT_TYPE_SCENE_TO_MAP_EDITOR_CHANGE;
		tEvent.tSceneEditorChangeParam.eScene = GSceneManager->GetCurrentSceneType();
		tEvent.tSceneEditorChangeParam.eEditor = EDITOR_TYPE::EDITOR_TYPE_MAP;
		GEventHandler->AddEvent(&tEvent);
	}

	// Change Editor To Scene
	if (GGameInput->KeyFirstDown(KEY_INPUT_F4))
	{
		EventHandle_t tEvent = {};
		tEvent.eEventType = EVENT_TYPE::EVENT_TYPE_EDITOR_TO_SCENE_CHANGE;
		tEvent.tSceneEditorChangeParam.eScene = GSceneManager->GetCurrentSceneType();
		tEvent.tSceneEditorChangeParam.eEditor = GEditorManager->GetCurrentEditorType();
		GEventHandler->AddEvent(&tEvent);
	}

	// Change Editor.
	if (GGameInput->KeyFirstDown(KEY_INPUT_F5) && GEditorManager->GetCurrentEditor())
	{
		AkU32 uTypeIndex = (AkU32)GEditorManager->GetCurrentEditorType() + 1;
		uTypeIndex %= (AkU32)EDITOR_TYPE::EDITOR_TYPE_COUNT;

		EventHandle_t tEvent = {};
		tEvent.eEventType = EVENT_TYPE::EVENT_TYPE_EDITOR_CHANGE;
		tEvent.tEditorChangeParam.eBefore = GEditorManager->GetCurrentEditorType();
		tEvent.tEditorChangeParam.eAfter = (EDITOR_TYPE)uTypeIndex;
		GEventHandler->AddEvent(&tEvent);
	}
}

//void Application::UpdateMouseNdcPos()
//{
//	// AkI32 iMousePosX = _pGameInput->GetMouseX();
//	AkI32 iMousePosX = GGameInput->GetAccumulatedMouseX();
//	AkI32 iMousePosY = GGameInput->GetMouseY();
//
//	_fNdcX = (AkF32)iMousePosX / _uScreenWidth * 2.0f - 1.0f;
//	_fNdcY = (AkF32)iMousePosY / _uScreenHeight * -2.0f + 1.0f;
//
//	_fClampNdcX = Clamp(_fNdcX, -1.0f, 1.0f);
//	_fClampNdcY = Clamp(_fNdcY, -1.0f, 1.0f);
//}

void Application::UpdateText()
{
	AkI32 iTextWidth = 0;
	AkI32 iTextHeight = 0;
	wchar_t wcText[128] = {};
	AkU32 uTxtLen = swprintf_s(wcText, L"fps:%.2lf\n", _fFps);

	if (wcscmp(_wcText, wcText))
	{
		GRenderer->WriteTextToBitmap(_pTextTextureImage, _uTextTextureWidth, _uTextTextureHeight, _uTextTextureWidth * 4, &iTextWidth, &iTextHeight, GCommonFont, _wcText, uTxtLen);
		GRenderer->UpdateTextureWidthImage(_pSysTextureHandle, _pTextTextureImage, _uTextTextureWidth, _uTextTextureHeight);
		wcscpy_s(_wcText, wcText);
	}
}

void Application::Render()
{
	// Render Scene list.
	GSceneManager->Render();

	// Render editor list.
	GEditorManager->Render();

	// Render System info text.
	RenderText();

	// Render UI
	GUIManager->Render();
}

void Application::RenderText()
{
	GRenderer->RenderSpriteWithTex(GCommonSprite, 10, 10, 1.0f, 1.0f, nullptr, 0.0f, _pSysTextureHandle);
}

void Application::CalculateFrameRate()
{
	static AkU32 uFrameCount = 0;
	static AkF32 fTimeElapsed = 0.0f;

	uFrameCount++;

	if (GTimer->GetTotalTime() - fTimeElapsed >= 1.0f)
	{
		AkF32 fFps = static_cast<AkF32>(uFrameCount);

		_fFps = fFps;

		uFrameCount = 0;
		fTimeElapsed += 1.0f;
	}
}

void Application::ExitGame()
{
	::PostQuitMessage(996);
}


