#pragma once

#include "Editor.h"

class UCamera;

class UMapEditor : public UEditor
{
public:
	UMapEditor();
	~UMapEditor();

	AkBool Initialize();

	virtual void BeginEditor() override;
	virtual void EndEditor() override;
	virtual void Update() override;
	virtual void UpdateGui() override;
	virtual void Render() override;

private:
	virtual void CleanUp() override;

private:
	Vector3 _vCamPos = Vector3(0.0f, 0.0f, -10.0f);
	Vector3 _vCamDir = Vector3(0.0f);
	UCamera* _pCam = nullptr;
	AkBool _bFPV = AK_FALSE;
};

