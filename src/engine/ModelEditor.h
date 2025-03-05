#pragma once

#include "Editor.h"

class Model;
class Camera;

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

public:
	virtual void Load() override;
	virtual void Save() override;

private:
	Model* _pTarget = nullptr;
	Camera* _pCamera = nullptr;
	Vector3 _vSceneCamPos = Vector3(0.0f);
	AkU32 _uSelectedAnimClip = 0;


	AkU32 _uFrame = 0;
};

