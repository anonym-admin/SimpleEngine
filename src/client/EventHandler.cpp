#include "pch.h"
#include "EventHandler.h"
#include "SceneManager.h"
#include "EditorManager.h"
#include "Editor.h"
#include "Application.h"

void UEventHandler::Excute()
{
	for (AkU32 i = 0; i < _uEventNum; i++)
	{
		switch (_pEvents[i].eType)
		{
		case EVENT_TYPE::CHANGE_SCENE:

			break;
		case EVENT_TYPE::CHANGE_MODEL_EDITOR:
		{
			UEditor* pEditor = GEditorManager->GetCurrentEditor();
			if (!wcscmp(pEditor->wcName, L"Model"))
				break;
			pEditor->EndEditor();
			pEditor = GEditorManager->SetCurrentEditor(L"Model");
			pEditor->BeginEditor();
		}
		break;
		case EVENT_TYPE::CHANGE_MAP_EDITOR:
		{
			UEditor* pEditor = GEditorManager->GetCurrentEditor();
			if (!wcscmp(pEditor->wcName, L"Map"))
				break;
			pEditor->EndEditor();
			pEditor = GEditorManager->SetCurrentEditor(L"Map"); 
			pEditor->BeginEditor();
		}
		break;
		case EVENT_TYPE::CHANGE_SCENE_TO_EDITOR:
		{
			UApplication* pApp = (UApplication*)_pEvents[i].pObj;
			pApp->UseEditor(AK_TRUE);
			UEditor* pEditor = GEditorManager->SetCurrentEditor(L"Model");
			pEditor->BeginEditor();
		}
		break;
		case EVENT_TYPE::CHANGE_EDITOR_TO_SCENE:
		{
			UApplication* pApp = (UApplication*)_pEvents[i].pObj;
			pApp->UseEditor(AK_FALSE);
			UEditor* pEditor = GEditorManager->SetCurrentEditor(L"Model");
			pEditor->EndEditor();
		}
		break;
		default:
		{
			__debugbreak();
		}
		break;
		}
	}

	_uEventNum = 0;
}

void UEventHandler::AddEvent(const EventHandle_t* pEvnetHandle)
{
	if (_uEventNum >= MAX_EVENT_COUNT)
		__debugbreak();

	EventHandle_t* pDest = _pEvents + _uEventNum * sizeof(EventHandle_t);
	memcpy(pDest, pEvnetHandle, sizeof(EventHandle_t));

	_uEventNum++;
}

