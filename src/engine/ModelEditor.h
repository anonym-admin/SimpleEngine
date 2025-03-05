#pragma once

#include "Editor.h"

class ModelEditor : public Editor
{
public:
	ModelEditor();
	~ModelEditor();

	virtual AkBool BeginEditor() override;
	virtual AkBool EndEditor() override;
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;
	virtual void RenderShadow() override;
};

