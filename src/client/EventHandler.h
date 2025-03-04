#pragma once

class UEventHandler
{
private:
	static const AkU32 MAX_EVENT_COUNT = 1024;

public:
	void Excute();
	void AddEvent(const EventHandle_t* pEvnetHandle);

private:
	EventHandle_t _pEvents[MAX_EVENT_COUNT] = {};
	AkU32 _uEventNum = 0;
};

