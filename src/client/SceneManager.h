#pragma once

/*
================
Scene Manager
================
*/

class UScene;

class USceneManager
{
public:
	USceneManager();
	~USceneManager();

	AkBool Initialize();
	void Update();
	void Render();
	UScene* AddScene(const wchar_t* wcName, UScene* pScene);
	UScene* SetCurrentScene(const wchar_t* wcName);
	void DeleteScene(const wchar_t* wcName);

private:
	void CleanUp();

private:
	HashTable_t* _pHashTable = nullptr;
	List_t* _pSceneListHead = nullptr;
	List_t* _pSceneListTail = nullptr;
	UScene* _pCurScene = nullptr;
};

