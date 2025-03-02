#pragma once

#include "GeometryGenerator.h"
#include "interface/IRenderer.h"
#include "common/AkMeshData.h"

/*
==================
LandScapeEditor
==================
*/

class LandScapeEditor
{
	struct Brush // Constant Buffer 로 Renderer 에 넘겨줘야 함
	{
		int type = 0;
		Vector3 pos = Vector3(0.0f);
		float range = 10.0f;
		Vector3 color = Vector3(0.0f, 0.5f, 0.0f);
	};

public:
	LandScapeEditor();
	~LandScapeEditor();

	bool Init();
	void Update(const float dt);
	void UpdateGui();
	void Render();

	void Load();
	void Save();

private:
	void CleanUp();
	void UpdateMousePicking();
	void ComputeHeight(const float dt);
	void ComputeNormals();
	void ComputeTangents();

private:
	int _width = 0;
	int _height = 0;
	bool _isPicked = false;
	ILandScape* _landScapehObj = nullptr;
	Matrix world = Matrix();
	DirectX::SimpleMath::Plane _plane = { 0.0f, 1.0f, 0.0f, 0.0f };
	Vector3 _pickPos = Vector3(0.0f);
	float _pickDist = 0.0f;
	float _ratio = 1.0f;
	void* _dynamicVertexHandle = nullptr;
	MeshData_t* _landScapeMeshData = nullptr;
	Brush* _brush = nullptr;
	float _heightScale = 50.0f;
	bool _isPositive = true;
	bool _isDraw = false;
	bool _isWire = false;
};

