#include "pch.h"
#include "InGameScene.h"
#include "Application.h"
#include "Actor.h"
#include "GeometryGenerator.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "LandScape.h"
#include "AssetManager.h"
#include "Player.h"
#include "Collider.h"
#include "KDTree.h"
#include "GameInput.h"
#include "Swat.h"

/*
=============
Scene InGame
=============
*/

UInGameScene::UInGameScene()
{
}

UInGameScene::~UInGameScene()
{
	EndScene();
}

AkBool UInGameScene::BeginScene()
{
	// Set IBL Strength.
	GRenderer->SetIBLStrength(0.25f);

	// Set IBL Texture.
	_pIrradiance = GAssetManager->GetTextureContainer(ASSET_TEXTURE_TYPE::ASSET_TEXTURE_TYPE_IBL_IRRADIANCE);
	_pSpecular = GAssetManager->GetTextureContainer(ASSET_TEXTURE_TYPE::ASSET_TEXTURE_TYPE_IBL_SPECULAR);
	_pBRDF = GAssetManager->GetTextureContainer(ASSET_TEXTURE_TYPE::ASSET_TEXTURE_TYPE_IBL_BRDF);
	GRenderer->BindIBLTexture(_pIrradiance->pTexHandle, _pSpecular->pTexHandle, _pBRDF->pTexHandle);

	Vector3 vRot = Vector3(DirectX::XM_PI / 6.0f, 0.0f, 0.0f);
	Swat* pSwat = new Swat;
	pSwat->SetRotation(&vRot);
	pSwat->tLink.pData = pSwat;
	pSwat->Name = L"Swat";
	AddGameObject(GAME_OBJECT_GROUP_TYPE::GAME_OBJ_GROUP_TYPE_PLAYER, pSwat);

	_pSkybox = GRenderer->CreateSkyboxObject();

	return AK_TRUE;
}

AkBool UInGameScene::EndScene()
{
	if (_pSkybox)
	{
		_pSkybox->Release();
		_pSkybox = nullptr;
	}

	return AK_TRUE;
}

void UInGameScene::Update()
{
	Scene::Update();
}

void UInGameScene::FinalUpdate()
{
	Scene::FinalUpdate();
}

void UInGameScene::RenderShadow()
{
	Scene::RenderShadow();



}

void UInGameScene::Render()
{
	Scene::Render();

	// Render skybox.
	AssetTextureContainer_t* pEnv = GAssetManager->GetTextureContainer(ASSET_TEXTURE_TYPE::ASSET_TEXTURE_TYPE_ENV);
	AssetTextureContainer_t* pDiffuseHDR = GAssetManager->GetTextureContainer(ASSET_TEXTURE_TYPE::ASSET_TEXTURE_TYPE_IBL_IRRADIANCE);
	AssetTextureContainer_t* pSpecularHDR = GAssetManager->GetTextureContainer(ASSET_TEXTURE_TYPE::ASSET_TEXTURE_TYPE_IBL_SPECULAR);
	GRenderer->RenderSkybox(_pSkybox, &_mSkyboxTransform, pEnv->pTexHandle, pDiffuseHDR->pTexHandle, pSpecularHDR->pTexHandle);
}

