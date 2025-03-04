#include "pch.h"
#include "StartScene.h"

#include "Model.h"
#include "GeometryGenerator.h"

UStartScene::UStartScene()
{
}

UStartScene::~UStartScene()
{
	CleanUp();
}

void UStartScene::BeginScene()
{
	_pIrradianceIBL = RENDERER->CreateCubeMapTexture(L"../../assets/skybox/PureSkyDiffuseHDR.dds");
	_pSpecularIBL = RENDERER->CreateCubeMapTexture(L"../../assets/skybox/PureSkySpecularHDR.dds");
	_pBRDF = RENDERER->CreateCubeMapTexture(L"../../assets/skybox/PureSkyBrdf.dds");
	RENDERER->BindIBLTexture(_pIrradianceIBL, _pSpecularIBL, _pBRDF);

	AkU32 uMeshDataNum = 0;
	MeshData_t* pMeshData = UGeometryGenerator::MakeSphere(&uMeshDataNum, 1.0f, 32, 32);
	_pModel = new UModel(pMeshData, uMeshDataNum);
	UGeometryGenerator::DestroyGeometry(pMeshData, uMeshDataNum);
}

void UStartScene::EndScene()
{
	if (_pModel)
	{
		delete _pModel;
		_pModel = nullptr;
	}

	RENDERER->DestroyTexture(_pBRDF);
	RENDERER->DestroyTexture(_pSpecularIBL);
	RENDERER->DestroyTexture(_pIrradianceIBL);
}

void UStartScene::Update()
{

}

void UStartScene::Render()
{
	_pModel->Render();
}

void UStartScene::CleanUp()
{
	EndScene();
}
