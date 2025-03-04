#pragma once

/*
==========
App
==========
*/

class UFont;

class UApplication
{
public:
	UApplication(AkBool bEnableDebugLayer, AkBool bEnableGBV);
	~UApplication();

	AkBool Initialize(AkBool bEnableDebugLayer, AkBool bEnableGBV);
	void RunApplication();

	void UseEditor(AkBool bEnableEditor) { _bEnableEditor = bEnableEditor; }

private:
	void CleanUp();
	
	AkBool InitRenderer(AkBool bEnableDebugLayer, AkBool bEnableGBV);
	AkBool InitScene();
	AkBool InitEditor();
	void CleanUpRenderer();
	void CleanUpEditor();
	void Update();
	void Render();
	void RenderSystemInfo();

#ifdef _DEBUG
	void Debug();
#endif

private:
	HMODULE _hRendererDLL = nullptr;
	HWND _hWnd = nullptr;
	AkBool _bEnableEditor = AK_FALSE;
	UFont* _pFont = nullptr;

public:
	class UCamera* _pCamera = nullptr;
};

