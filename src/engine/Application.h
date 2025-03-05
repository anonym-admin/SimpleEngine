#pragma once

/*
===============
Application
===============
*/

class BaseObject;
class CollisionManager;
class Animator;
class UEditorEvenetManager;
class UModelManager;
class UIManager;
class UTextUI;
class UInputUI;

class SoundManager;
class USound;

class Application
{
public:
	Application();
	~Application();

	AkBool InitApplication( AkBool bEnableDebugLayer, AkBool bEnableGBV);
	void RunApplication();

	AkBool UpdateWindowSize(AkU32 uScreenWidth, AkU32 uScreenHeight);

	AkF32 GetFps() { return _fFps; }
	AkBool EnableEditor() { return _bEnableEditor; }
	AkF32 GetClampNdcX() { return _fClampNdcX; }
	AkF32 GetClampNdcY() { return _fClampNdcY; }
	AkU32 GetScreenWidth() { return _uScreenWidth; }
	AkU32 GetScreenHeight() { return _uScreenHeight; }
	void SetVSync(AkBool bUseVSync);

private:
	void CleanUp();

	AkBool InitScene();
	AkBool InitEditor();

	void Update();
	void UpdateEnv();
	void UpdateMouseNdcPos();
	void UpdateText();
	void Render();
	void RenderText();
	void CalculateFrameRate();
	void ExitGame();

private:
	HMODULE _hRendererDLL = nullptr;
	AkF32 _fFps = 0.0f;
	AkU32 _uScreenWidth = 0;
	AkU32 _uScreenHeight = 0;
	AkF32 _fClampNdcX = 0.0f;
	AkF32 _fClampNdcY = 0.0f;
	AkU32 _uEditorType = 0;
	AkBool _bUpdatedFirst = AK_TRUE;
	AkBool _bEnableEditor = AK_FALSE;
	AkBool _bUseVSync = AK_FALSE;

	AkBool _bESC = AK_FALSE;

	// System info sprite texture.
	AkU32 _uTextTextureWidth = 0;
	AkU32 _uTextTextureHeight = 0;
	AkU8* _pTextTextureImage = nullptr;
	void* _pSysTextureHandle = nullptr;
	wchar_t _wcText[_MAX_PATH] = {};

public:
	AkF32 _fNdcX = 0.0f;
	AkF32 _fNdcY = 0.0f;
};


