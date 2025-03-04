#include "pch.h"
#include "EditorManager.h"
#include "Editor.h"

UEditorManager::UEditorManager()
{
}

UEditorManager::~UEditorManager()
{
	CleanUp();
}

AkBool UEditorManager::Initialize()
{
	_pHashTable = HT_CreateHashTable(128, _MAX_PATH, 256);

	return AK_TRUE;
}

void UEditorManager::Update()
{
	if (_pCurEditor)
	{
		_pCurEditor->Update();
		_pCurEditor->UpdateGui();
	}
}

void UEditorManager::Render()
{
	if (_pCurEditor)
		_pCurEditor->Render();
}

UEditor* UEditorManager::AddEditor(const wchar_t* wcName, UEditor* pEditor)
{
	UEditor* pFind = nullptr;
	AkU32 uKeyLen = (AkU32)wcslen(wcName) * sizeof(wchar_t);
	if (HT_Find(_pHashTable, (void**)&pFind, 1, wcName, uKeyLen))
	{
		delete pEditor;
		return pFind;
	}

	// Search Handle is used for HashTable delete.
	void* pSearchHandle = HT_Insert(_pHashTable, pEditor, wcName, uKeyLen);
	pEditor->pSearchHandle = pSearchHandle;
	pEditor->tLink.pData = pEditor;
	pEditor->wcName = wcName;
	LL_PushBack(&_pEditorListHead, &_pEditorListTail, &pEditor->tLink);
	return pEditor;
}

UEditor* UEditorManager::SetCurrentEditor(const wchar_t* wcName)
{
	UEditor* pFind = nullptr;
	AkU32 uKeyLen = (AkU32)wcslen(wcName) * sizeof(wchar_t);
	if (!HT_Find(_pHashTable, (void**)&pFind, 1, wcName, uKeyLen))
	{
		__debugbreak();
		return pFind;
	}
	_pCurEditor = pFind;
	return pFind;
}

void UEditorManager::DeleteEditor(const wchar_t* wcName)
{
	UEditor* pFind = nullptr;
	AkU32 uKeyLen = (AkU32)wcslen(wcName) * sizeof(wchar_t);
	if (HT_Find(_pHashTable, (void**)&pFind, 1, wcName, uKeyLen))
	{
		HT_Delete(_pHashTable, pFind->pSearchHandle);
	}
}

void UEditorManager::CleanUp()
{
	if (_pEditorListHead)
	{
		List_t* pNext = nullptr;
		List_t* pCur = _pEditorListHead;
		while (pCur != nullptr)
		{
			pNext = pCur->pNext;
			UEditor* pEditor = (UEditor*)pCur->pData;
			if (pEditor)
				delete pEditor;
			pCur = pNext;
		}
	}

	if (_pHashTable)
	{
		HT_DeleteAll(_pHashTable);
		HT_DestroyHashTable(_pHashTable);
	}
}