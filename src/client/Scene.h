#pragma once

class UScene
{
public:
	UScene() = default;
	virtual ~UScene() = default;

	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	virtual void CleanUp() = 0;

public:
	// For Hash Table.
	List_t tLink = {};
	void* pSearchHandle = nullptr;
};

