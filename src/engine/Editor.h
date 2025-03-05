#pragma once

class Editor
{
public:
	Editor() = default;
	virtual ~Editor() = default;

	virtual AkBool BeginEditor() = 0;
	virtual AkBool EndEditor() = 0;
	virtual void Update() = 0;
	virtual void FinalUpdate() = 0;
	virtual void Render() = 0;
	virtual void RenderShadow() = 0;

	void SetName(const wchar_t* wcName) { Name = wcName; }

public:
	const wchar_t* Name;
};

