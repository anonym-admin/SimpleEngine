#pragma once

class UEditor;

class UEditorManager
{
public:
	UEditorManager();
	~UEditorManager();

	AkBool Initialize();
	void Update();
	void Render();
	UEditor* AddEditor(const wchar_t* wcName, UEditor* pEditor);
	UEditor* SetCurrentEditor(const wchar_t* wcName);
	UEditor* GetCurrentEditor() { return _pCurEditor; }
	void DeleteEditor(const wchar_t* wcName);

private:
	void CleanUp();

private:
	HashTable_t* _pHashTable = nullptr;
	List_t* _pEditorListHead = nullptr;
	List_t* _pEditorListTail = nullptr;
	UEditor* _pCurEditor = nullptr;
};

