#pragma once

#include "Scene.h"

/*
=============
Scene InGame
=============
*/

class UInGameScene : public Scene
{
public:
	UInGameScene();
	~UInGameScene();

	virtual AkBool BeginScene() override;
	virtual AkBool EndScene() override;
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;
	virtual void RenderShadow() override;

private:
	AssetTextureContainer_t* _pIrradiance = nullptr;
	AssetTextureContainer_t* _pSpecular = nullptr;
	AssetTextureContainer_t* _pBRDF = nullptr;
	ISkyboxObject* _pSkybox = nullptr;
	Matrix _mSkyboxTransform = Matrix();
};


