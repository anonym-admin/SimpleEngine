#pragma once

#include "Scene.h"

class UStartScene : public UScene
{
public:
	UStartScene();
	~UStartScene();

	// Override
	virtual void BeginScene() override;
	virtual void EndScene() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	virtual void CleanUp() override;

private:
	class UModel* _pModel = nullptr;
	void* _pIrradianceIBL = nullptr;
	void* _pSpecularIBL = nullptr;
	void* _pBRDF = nullptr;
};

