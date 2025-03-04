#pragma once

#include "Editor.h"

class UModelEditor : public UEditor
{
public:
	UModelEditor();
	~UModelEditor();

	virtual void BeginEditor() override;
	virtual void EndEditor() override;
	virtual void Update() override;
	virtual void UpdateGui() override;
	virtual void Render() override;

private:
	virtual void CleanUp() override;
};

