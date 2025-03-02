#include "LandScapeEditor.h"

extern IRenderer* g_pRenderer;
extern float g_fNdcX;
extern float g_fNdcY;

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
	// 01. Grid 持失.
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
	// 02. render object 持失.
	Vector3 albedo = Vector3(1.0f, 1.0f, 0.0f);
	float metallic = 0.0f;
	float roughness = 1.0f;
	Vector3 emissive = Vector3(0.0f);
	_meshObj = g_pRenderer->CreateBasicMeshObject();
	_meshObj->CreateMeshBuffers(new_grid, 1);
	_meshObj->UpdateMaterialBuffers(&albedo, metallic, roughness, &emissive);
	_meshObj->EnableWireFrame();

	// TODO
	// Delete MeshData

	return true;
}

void LandScapeEditor::Update(const float dt)
{
	UpdateMousePicking();
}

void LandScapeEditor::Render()
{
	g_pRenderer->RenderBasicMeshObject(_meshObj, &world);
}

void LandScapeEditor::Load()
{
}

void LandScapeEditor::Save()
{
}

void LandScapeEditor::CleanUp()
{
	if (_meshObj)
	{
		_meshObj->Release();
		_meshObj = nullptr;
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

void LandScapeEditor::ComputeHeight()
{

}
