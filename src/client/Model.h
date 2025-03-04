#pragma once

class UModel
{
public:
	UModel(MeshData_t* pMeshData, AkU32 uMeshDataNum);
	UModel(const wchar_t* wcFilename);
	~UModel();

	AkBool Initialize(MeshData_t* pMeshData, AkU32 uMeshDataNum);
	AkBool Initialize(const wchar_t* wcFilename);
	void Render();

private:
	void CleanUp();

private:
	IMeshObject* _pMeshObj = nullptr;
	Matrix _mWorldRow = Matrix();
};

