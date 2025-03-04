#pragma once

class UEditor
{
public:
	UEditor() = default;
	virtual ~UEditor() = default;

	virtual void BeginEditor() = 0;
	virtual void EndEditor() = 0;
	virtual void Update() = 0;
	virtual void UpdateGui() = 0;
	virtual void Render() = 0;

protected:
	virtual void CleanUp() = 0;

public:
	// For Hash Table.
	List_t tLink = {};
	void* pSearchHandle = nullptr;
	const wchar_t* wcName = nullptr;
};

