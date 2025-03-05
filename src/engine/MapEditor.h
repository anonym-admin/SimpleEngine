#pragma once

#include "Editor.h"

class MapEditor : public Editor
{
public:
	MapEditor();
	~MapEditor();

	virtual AkBool BeginEditor() override;
	virtual AkBool EndEditor() override;
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;
	virtual void RenderShadow() override;
};

