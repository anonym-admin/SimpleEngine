#include "pch.h"
#include "Global.h"
#include "Window.h"
#include "Application.h"
#include "Timer.h"
#include "GameInput.h"
#include "SceneManager.h"
#include "EditorManager.h"
#include "EventHandler.h"
#include "AssetManager.h"
#include "CollisionManager.h"
#include "UIManager.h"
#include "Sound.h"
#include "Animator.h"

HWND GhWnd;
IRenderer* GRenderer;
ISpriteObject* GCommonSprite;
Window* GWindow;
Application* GApp;
Timer* GTimer;
GameInput* GGameInput;
SceneManager* GSceneManager;
EditorManager* GEditorManager;
EventHandler* GEventHandler;
AssetManager* GAssetManager;
CollisionManager* GCollisionManager;
UIManager* GUIManager;
SoundManager* GSoundManager;
Animator* GAnimator;
void* GCommonFont;

class UGlobal
{
public:
	UGlobal()
	{
		GWindow = new Window(L"Simple Engine", L"Window Program");
		GTimer = new Timer;
		GGameInput = new GameInput;
		GAssetManager = new AssetManager;
		GEventHandler = new EventHandler;
		GSceneManager = new SceneManager;
		GEditorManager = new EditorManager;
		GAnimator = new Animator;
		GCollisionManager = new CollisionManager;
		GUIManager = new UIManager;
		GSoundManager = new SoundManager;
	}
	~UGlobal()
	{
		if (GSoundManager)
		{
			delete GSoundManager;
			GSoundManager = nullptr;
		}
		if (GUIManager)
		{
			delete GUIManager;
			GUIManager = nullptr;
		}
		if (GCollisionManager)
		{
			delete GCollisionManager;
			GCollisionManager = nullptr;
		}
		if (GAnimator)
		{
			delete GAnimator;
			GAnimator = nullptr;
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
		if (GEventHandler)
		{
			delete GEventHandler;
			GEventHandler = nullptr;
		}
		if (GAssetManager)
		{
			delete GAssetManager;
			GAssetManager = nullptr;
		}
		if (GGameInput)
		{
			delete GGameInput;
			GGameInput = nullptr;
		}
		if (GTimer)
		{
			delete GTimer;
			GTimer = nullptr;
		}
		if (GWindow)
		{
			delete GWindow;
			GWindow = nullptr;
		}
	}
} Global;