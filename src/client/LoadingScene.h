#pragma once

#include "Scene.h"

class ULoadingScene : public UScene
{
public:
	ULoadingScene();
	~ULoadingScene();

	// Override
	virtual void BeginScene() override;
	virtual void EndScene() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	virtual void CleanUp() override;
};

