#pragma once

#include "Scene.h"

class UMenuScene : public UScene
{
public:
	UMenuScene();
	~UMenuScene();

	// Override
	virtual void BeginScene() override;
	virtual void EndScene() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	virtual void CleanUp() override;
};

