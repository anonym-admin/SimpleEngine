#include "pch.h"
#include "MenuScene.h"

UMenuScene::UMenuScene()
{
}

UMenuScene::~UMenuScene()
{
	CleanUp();
}

void UMenuScene::BeginScene()
{
}

void UMenuScene::EndScene()
{
}

void UMenuScene::Update()
{
}

void UMenuScene::Render()
{
}

void UMenuScene::CleanUp()
{
	EndScene();
}
