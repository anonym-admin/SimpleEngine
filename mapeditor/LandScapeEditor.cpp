#include "LandScapeEditor.h"
#include <imgui.h>
#include <iostream>
#include <fstream>

extern IRenderer* g_pRenderer;
extern float g_fNdcX;
extern float g_fNdcY;
extern bool g_bKeyDown[256];
extern bool g_bLBtnDown;

/*
==================
LandScapeEditor
==================
*/

LandScapeEditor::LandScapeEditor()
{
}

LandScapeEditor::~LandScapeEditor()
{
	CleanUp();
}

bool LandScapeEditor::Init()
{
	// 01. Grid 생성.
	MeshData grid = GeometryGenerator::MakeSquareGrid(100, 100, 100.0f, Vector2(1.0f));
	MeshData_t* new_grid = new MeshData_t;
	new_grid->uVerticeNum = grid.vertices.size();
	new_grid->uIndicesNum = grid.indices.size();
	new_grid->pVertices = new Vertex_t[new_grid->uVerticeNum];
	new_grid->pIndices = new unsigned int[new_grid->uIndicesNum];
	memcpy(new_grid->pVertices, grid.vertices.data(), sizeof(Vertex_t) * new_grid->uVerticeNum);
	memcpy(new_grid->pIndices, grid.indices.data(), sizeof(unsigned int) * new_grid->uIndicesNum);
	for (int i = 0; i < new_grid->uVerticeNum; i++)
	{
		new_grid->pVertices[i].vPosition = Vector3::Transform(new_grid->pVertices[i].vPosition, Matrix::CreateRotationX(DirectX::XM_PIDIV2));
		new_grid->pVertices[i].vNormalModel = Vector3::Transform(new_grid->pVertices[i].vPosition, Matrix::CreateRotationX(DirectX::XM_PIDIV2));
		new_grid->pVertices[i].vTangentModel = Vector3::Transform(new_grid->pVertices[i].vPosition, Matrix::CreateRotationX(DirectX::XM_PIDIV2));
	}
	// 02. render object 생성.
	Vector3 albedo = Vector3(1.0f, 1.0f, 0.0f);
	float metallic = 0.0f;
	float roughness = 1.0f;
	Vector3 emissive = Vector3(0.0f);
	_landScapehObj = g_pRenderer->CreateLandScapeObject();
	_dynamicVertexHandle = _landScapehObj->CreateDynamicMeshBuffers(new_grid, 1);
	_landScapehObj->UpdateMaterialBuffers(&albedo, metallic, roughness, &emissive);

	_landScapeMeshData = new_grid;
	_brush = new Brush;

	// TODO
	// Delete MeshData

	return true;
}

void LandScapeEditor::Update(const float dt)
{
	if (g_bLBtnDown)
	{
		UpdateMousePicking();
	}

	if (_isDraw)
	{
		if (g_bLBtnDown && _isPicked)
		{
			ComputeHeight(dt);
		}
		else
		{
			g_pRenderer->UpdateDynamicVertices(_dynamicVertexHandle, _landScapeMeshData, 1);
			ComputeNormals();
			ComputeTangents();
		}
	}
	else
	{
		g_pRenderer->UpdateDynamicVertices(_dynamicVertexHandle, _landScapeMeshData, 1);
	}

	// On/Off WireFrame.
	if (_isWire)
	{
		_landScapehObj->EnableWireFrame();
	}
	else
	{
		_landScapehObj->DisableWireFrame();
	}
}

void LandScapeEditor::UpdateGui()
{
	ImGui::Begin("LandScape Editor");
	ImGui::Checkbox("Draw Brush", &_isDraw);
	ImGui::Checkbox("Wire", &_isWire);
	ImGui::Checkbox("Height Direction", &_isPositive);
	if (ImGui::RadioButton("Sphere type", &_brush->type, 0)) _brush->type = 0;
	ImGui::SameLine();
	if (ImGui::RadioButton("Square type", &_brush->type, 1)) _brush->type = 1;
	ImGui::SameLine();
	if (ImGui::RadioButton("Zero type", &_brush->type, 2)) _brush->type = 2;
	if(ImGui::Button("Save"))
	{
		Save();
	}
	if (ImGui::Button("Load"))
	{
		Load();
	}
	ImGui::End();
}

void LandScapeEditor::Render()
{
	g_pRenderer->RenderLandScapeObject(_landScapehObj, &world);
}

void LandScapeEditor::Load()
{
	ifstream fin("test.map", ios_base::binary);

	fin >> _landScapeMeshData->uVerticeNum;
	fin >> _landScapeMeshData->uIndicesNum;
	for (int i = 0; i < _landScapeMeshData->uVerticeNum; i++)
	{
		fin >> _landScapeMeshData->pVertices[i].vPosition.x >> _landScapeMeshData->pVertices[i].vPosition.y >> _landScapeMeshData->pVertices[i].vPosition.z;
		fin >> _landScapeMeshData->pVertices[i].vNormalModel.x >>_landScapeMeshData->pVertices[i].vNormalModel.y >> _landScapeMeshData->pVertices[i].vNormalModel.z;
		fin >> _landScapeMeshData->pVertices[i].vTangentModel.x >> _landScapeMeshData->pVertices[i].vTangentModel.y >> _landScapeMeshData->pVertices[i].vTangentModel.z;
		fin >> _landScapeMeshData->pVertices[i].vTexCoord.x >> _landScapeMeshData->pVertices[i].vTexCoord.y;
	}

	fin.close();
}

void LandScapeEditor::Save()
{
	ofstream fout("test.map", ios_base::binary);

	fout << _landScapeMeshData->uVerticeNum << endl;
	fout << _landScapeMeshData->uIndicesNum << endl;
	for (int i = 0; i < _landScapeMeshData->uVerticeNum; i++)
	{
		fout << _landScapeMeshData->pVertices[i].vPosition.x << " " << _landScapeMeshData->pVertices[i].vPosition.y << " " << _landScapeMeshData->pVertices[i].vPosition.z << endl;
		fout << _landScapeMeshData->pVertices[i].vNormalModel.x << " " << _landScapeMeshData->pVertices[i].vNormalModel.y << " " << _landScapeMeshData->pVertices[i].vNormalModel.z << endl;
		fout << _landScapeMeshData->pVertices[i].vTangentModel.x << " " << _landScapeMeshData->pVertices[i].vTangentModel.y << " " << _landScapeMeshData->pVertices[i].vTangentModel.z << endl;
		fout << _landScapeMeshData->pVertices[i].vTexCoord.x << " " << _landScapeMeshData->pVertices[i].vTexCoord.y << endl;
	}

	fout.close();
}

void LandScapeEditor::CleanUp()
{
	if (_brush)
	{
		delete _brush;
		_brush = nullptr;
	}
	if (_dynamicVertexHandle)
	{
		g_pRenderer->DestroyDynamicVertex(_dynamicVertexHandle);
		_dynamicVertexHandle = nullptr;
	}
	if (_landScapeMeshData)
	{
		if (_landScapeMeshData->pVertices)
		{
			delete[] _landScapeMeshData->pVertices;
			_landScapeMeshData->pVertices = nullptr;
		}
		if (_landScapeMeshData->pIndices)
		{
			delete[] _landScapeMeshData->pIndices;
			_landScapeMeshData->pIndices = nullptr;
		}
		delete _landScapeMeshData;
		_landScapeMeshData = nullptr;
	}
	if (_landScapehObj)
	{
		_landScapehObj->Release();
		_landScapehObj = nullptr;
	}
}

void LandScapeEditor::UpdateMousePicking()
{
	Vector3 v0 = Vector3(-100.0f, 0.0f, -100.0f);
	Vector3 v1 = Vector3(-100.0f, 0.0f, 100.0f);
	Vector3 v2 = Vector3(100.0f, 0.0f, 100.0f);
	Vector3 v3 = Vector3(100.0f, 0.0f, -100.0f);
	bool picked = g_pRenderer->MousePickingToSqaure(&v0, &v1, &v2, &v3, g_fNdcX, g_fNdcY, &_pickPos, &_pickDist, &_ratio);

	_isPicked = picked;


	if (_isPicked)
	{
		cout << "Pick" << endl;
		cout << _pickPos.x << " " << _pickPos.y << " " << _pickPos.z << endl;
	}
}

void LandScapeEditor::ComputeHeight(const float dt)
{
	if (0 == _brush->type)
	{
		// 구
		for (int i = 0; i < _landScapeMeshData->uVerticeNum; i++)
		{
			Vector3 p = Vector3(_landScapeMeshData->pVertices[i].vPosition.x, 0.0f, _landScapeMeshData->pVertices[i].vPosition.z);
			Vector3 c = Vector3(_pickPos.x, 0, _pickPos.z);

			float distance = (c - p).Length();
			float cosValue = cos(DirectX::XM_PIDIV2 * distance / _brush->range);
			float temp = _heightScale * max(0, cosValue);
			if (distance <= _brush->range)
			{
				if (_isPositive)
				{
					_landScapeMeshData->pVertices[i].vPosition.y += temp * dt;
				}
				else
				{
					_landScapeMeshData->pVertices[i].vPosition.y -= temp * dt;
				}
			}
		}
	}
	if (1 == _brush->type)
	{
		// 사각형
		for (int i = 0; i < _landScapeMeshData->uVerticeNum; i++)
		{
			Vector3 p = Vector3(_landScapeMeshData->pVertices[i].vPosition.x, 0.0f, _landScapeMeshData->pVertices[i].vPosition.z);
			Vector3 c = Vector3(_pickPos.x, 0, _pickPos.z);

			float distX = abs(c.x - p.x);
			float distZ = abs(c.z - p.z);

			if (distX <= _brush->range && distZ <= _brush->range)
			{
				if (_isPositive)
				{
					_landScapeMeshData->pVertices[i].vPosition.y += _heightScale * dt;
				}
				else
				{
					_landScapeMeshData->pVertices[i].vPosition.y -= _heightScale * dt;
				}
			}
		}
	}
	if (2 == _brush->type)
	{
		// Height 를 0 으로 만드는 
		for (int i = 0; i < _landScapeMeshData->uVerticeNum; i++)
		{
			Vector3 p = Vector3(_landScapeMeshData->pVertices[i].vPosition.x, 0.0f, _landScapeMeshData->pVertices[i].vPosition.z);
			Vector3 c = Vector3(_pickPos.x, 0, _pickPos.z);

			float distance = (c - p).Length();
			float cosValue = cos(DirectX::XM_PIDIV2 * distance / _brush->range);
			float temp = _heightScale * max(0, cosValue);
			if (distance <= _brush->range)
			{
				_landScapeMeshData->pVertices[i].vPosition.y = 0;
			}
		}
	}

	g_pRenderer->UpdateDynamicVertices(_dynamicVertexHandle, _landScapeMeshData, 1);
}

void LandScapeEditor::ComputeNormals()
{
	
}

void LandScapeEditor::ComputeTangents()
{
}
