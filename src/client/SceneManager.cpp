#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

/*
================
Scene Manager
================
*/

USceneManager::USceneManager()
{
}

USceneManager::~USceneManager()
{
	CleanUp();
}

AkBool USceneManager::Initialize()
{
	_pHashTable = HT_CreateHashTable(128, _MAX_PATH, 256);

	return AK_TRUE;
}

void USceneManager::Update()
{
}

void USceneManager::Render()
{
	if(_pCurScene)
		_pCurScene->Render();
}

UScene* USceneManager::AddScene(const wchar_t* wcName, UScene* pScene)
{
	UScene* pFind = nullptr;
	AkU32 uKeyLen = (AkU32)wcslen(wcName) * sizeof(wchar_t);
	if (HT_Find(_pHashTable, (void**)&pFind, 1, wcName, uKeyLen))
	{
		delete pScene;
		return pFind;
	}

	// Search Handle is used for HashTable delete.
	void* pSearchHandle = HT_Insert(_pHashTable, pScene, wcName, uKeyLen);
	pScene->pSearchHandle = pSearchHandle;
	pScene->tLink.pData = pScene;
	LL_PushBack(&_pSceneListHead, &_pSceneListTail, &pScene->tLink);
	return pScene;
}

UScene* USceneManager::SetCurrentScene(const wchar_t* wcName)
{
	UScene* pFind = nullptr;
	AkU32 uKeyLen = (AkU32)wcslen(wcName) * sizeof(wchar_t);
	if (!HT_Find(_pHashTable, (void**)&pFind, 1, wcName, uKeyLen))
	{
		__debugbreak();
		return pFind;
	}
	_pCurScene = pFind;
	return pFind;
}

void USceneManager::DeleteScene(const wchar_t* wcName)
{
	UScene* pFind = nullptr;
	AkU32 uKeyLen = (AkU32)wcslen(wcName) * sizeof(wchar_t);
	if (HT_Find(_pHashTable, (void**)&pFind, 1, wcName, uKeyLen))
	{
		HT_Delete(_pHashTable, pFind->pSearchHandle);
	}
}

void USceneManager::CleanUp()
{
	if (_pSceneListHead)
	{
		List_t* pNext = nullptr;
		List_t* pCur = _pSceneListHead;
		while (pCur != nullptr)
		{
			pNext = pCur->pNext;
			UScene* pScene = (UScene*)pCur->pData;
			if(pScene)
				delete pScene;
			pCur = pNext;
		}
	}

	if (_pHashTable)
	{
		HT_DeleteAll(_pHashTable);
		HT_DestroyHashTable(_pHashTable);
	}
}