#include "pch.h"
#include "Global.h"
#include "Window.h"
#include "GameInput.h"
#include "Timer.h"
#include "SceneManager.h"
#include "EditorManager.h"
#include "EventHandler.h"

IRenderer* GRenderer;
UWindow* GWindow;
UGameInput* GGameInput;
UTimer* GTimer;
USceneManager* GSceneManager;
UEditorManager* GEditorManager;
UEventHandler* GEventHandler;

class UGlobal
{
public:
	UGlobal()
	{
		GWindow = new UWindow;
		if (!GWindow->Initialize())
		{
			__debugbreak();
		}
		GGameInput = new UGameInput;
		if (!GGameInput->Initialize())
		{
			__debugbreak();
		}
		GTimer = new UTimer;
		if (!GTimer->Initialize())
		{
			__debugbreak();
		}
		GSceneManager = new USceneManager;
		if (!GSceneManager->Initialize())
		{
			__debugbreak();
		}
		GEditorManager = new UEditorManager;
		if (!GEditorManager->Initialize())
		{
			__debugbreak();
		}
		GEventHandler = new UEventHandler;
	}
	~UGlobal()
	{
		if (GEventHandler)
		{
			delete GEventHandler;
			GEventHandler = nullptr;
		}
		if (GEditorManager)
		{
			delete GEditorManager;
			GEditorManager = nullptr;
		}
		if (GSceneManager)
		{
			delete GSceneManager;
			GSceneManager = nullptr;
		}
		if (GTimer)
		{
			delete GTimer;
			GTimer = nullptr;
		}
		if (GGameInput)
		{
			delete GGameInput;
			GGameInput = nullptr;
		}
		if (GWindow)
		{
			delete GWindow;
			GWindow = nullptr;
		}
	}
} ClientGlobal;