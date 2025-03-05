#pragma once

/*
==================
GameEventManager
==================
*/

class Application;

class EventHandler
{
public:
	static const AkU32 MAX_EVENT_HANDLE_LIST_COUNT = 1024;

	EventHandler();
	~EventHandler();

	void Excute();

	void AddEvent(EventHandle_t* pEventHandle);

private:
	void CleanUp();

	EventHandle_t* Dispatch();
	void Reset();

private:
	Application* _pApp = nullptr;
	EventHandle_t _pEventHandleList[MAX_EVENT_HANDLE_LIST_COUNT] = {};
	AkU32 _uReadPos = 0;
	AkU32 _uWritePos = 0;
};
