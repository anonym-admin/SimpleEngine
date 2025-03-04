#include "pch.h"
#include "Model.h"
#include "GeometryGenerator.h"

UModel::UModel(MeshData_t* pMeshData, AkU32 uMeshDataNum)
{
	if (!Initialize(pMeshData, uMeshDataNum))
	{
		__debugbreak();
	}
}

UModel::UModel(const wchar_t* wcFilename)
{

}

UModel::~UModel()
{
	CleanUp();
}

AkBool UModel::Initialize(MeshData_t* pMeshData, AkU32 uMeshDataNum)
{
	Vector3 vAlbdo = Vector3(0.0f);
	AkF32 fMetallic = 0.0f;
	AkF32 fRoughness = 1.0f;
	Vector3 vEmissive = Vector3(0.0f, 1.0f, 0.0f);
	_pMeshObj = RENDERER->CreateBasicMeshObject();
	_pMeshObj->CreateMeshBuffers(pMeshData, uMeshDataNum);
	_pMeshObj->EnableWireFrame();
	_pMeshObj->UpdateMaterialBuffers(&vAlbdo, fMetallic, fRoughness, &vEmissive);

	return AK_TRUE;
}

AkBool UModel::Initialize(const wchar_t* wcFilename)
{
	return AK_TRUE;
}

void UModel::Render()
{
	RENDERER->RenderBasicMeshObject(_pMeshObj, &_mWorldRow);
}

void UModel::CleanUp()
{
	if (_pMeshObj)
	{
		_pMeshObj->Release();
		_pMeshObj = nullptr;
	}
}