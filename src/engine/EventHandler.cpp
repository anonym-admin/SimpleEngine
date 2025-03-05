#include "pch.h"
#include "EventHandler.h"
#include "SceneManager.h"
#include "EditorManager.h"
#include "Scene.h"
#include "Editor.h"

/*
==================
GameEventManager
==================
*/

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
    CleanUp();
}

void EventHandler::Excute()
{
    EventHandle_t* pEventHandle = nullptr;

    while (pEventHandle = Dispatch())
    {
        switch (pEventHandle->eEventType)
        {
            case EVENT_TYPE::EVENT_TYPE_CREATE_GAME_OBJECT:
            {

            }
            break;
            case EVENT_TYPE::EVENT_TYPE_SCENE_CHANGE:
            {
                GSceneManager->ChangeScene(pEventHandle->tSceneChangeParam.eAfter);
            }
            break;
            case EVENT_TYPE::EVENT_TYPE_SCENE_TO_MODEL_EDITOR_CHANGE:
            {
                GSceneManager->UnBindCurrentScene();
                GEditorManager->BindCurrnetEditor(pEventHandle->tSceneEditorChangeParam.eEditor)->BeginEditor();
            }
            break;
            case EVENT_TYPE::EVENT_TYPE_SCENE_TO_MAP_EDITOR_CHANGE:
            {
                GEditorManager->BindCurrnetEditor(pEventHandle->tSceneEditorChangeParam.eEditor)->BeginEditor();
            }
            break;
            case EVENT_TYPE::EVENT_TYPE_EDITOR_TO_SCENE_CHANGE:
            {
                GEditorManager->UnBindCurrentEditor();
                GSceneManager->BindCurrentScene(pEventHandle->tSceneEditorChangeParam.eScene);
            }
            break;
            case EVENT_TYPE::EVENT_TYPE_EDITOR_CHANGE:
            {
                if (EDITOR_TYPE::EDITOR_TYPE_MAP == pEventHandle->tEditorChangeParam.eBefore)
                {
                    GSceneManager->UnBindCurrentScene();
                }
                if (EDITOR_TYPE::EDITOR_TYPE_MODEL == pEventHandle->tEditorChangeParam.eBefore)
                {
                    GSceneManager->BindCurrentScene(SCENE_TYPE::SCENE_TYPE_INGANE);
                }
                GEditorManager->UnBindCurrentEditor();
                GEditorManager->BindCurrnetEditor(pEventHandle->tEditorChangeParam.eAfter);
            }
            break;
            default:
            {
                __debugbreak();
            }
            break;
        }
    }

    Reset();
}

void EventHandler::Reset()
{
    _uWritePos = 0;
    _uReadPos = 0;
}

void EventHandler::AddEvent(EventHandle_t* pEventHandle)
{
    if (_uWritePos + sizeof(EventHandle_t) > MAX_EVENT_HANDLE_LIST_COUNT)
    {
        __debugbreak();
    }

    EventHandle_t* pDest = _pEventHandleList + _uWritePos;
    memcpy(pDest, pEventHandle, sizeof(EventHandle_t));

    _uWritePos += sizeof(EventHandle_t);
}

void EventHandler::CleanUp()
{
}

EventHandle_t* EventHandler::Dispatch()
{
    EventHandle_t* pResult = nullptr;

    if (_uReadPos + sizeof(EventHandle_t) > _uWritePos)
    {
        return pResult;
    }

    pResult = _pEventHandleList + _uReadPos;

    _uReadPos += sizeof(EventHandle_t);

    return pResult;
}
