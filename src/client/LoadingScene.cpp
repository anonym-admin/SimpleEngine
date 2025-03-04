#include "pch.h"
#include "LoadingScene.h"

ULoadingScene::ULoadingScene()
{
}

ULoadingScene::~ULoadingScene()
{
	CleanUp();
}

void ULoadingScene::BeginScene()
{
}

void ULoadingScene::EndScene()
{
}

void ULoadingScene::Update()
{
}

void ULoadingScene::Render()
{
}

void ULoadingScene::CleanUp()
{
	EndScene();
}
