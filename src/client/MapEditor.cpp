#include "pch.h"
#include "MapEditor.h"
#include "SceneManager.h"
#include "Camera.h"
#include "GameInput.h"

UMapEditor::UMapEditor()
{
	Initialize();
}

UMapEditor::~UMapEditor()
{
	CleanUp();
}

AkBool UMapEditor::Initialize()
{
	_pCam = new UCamera(&_vCamPos, &_vCamDir);
	_pCam->Mode = UCamera::CamMode::EDITOR;

	return AK_TRUE;
}

void UMapEditor::BeginEditor()
{
	_pCam->SetPosition(&_vCamPos);
	_pCam->SetDirection(&_vCamDir);
}

void UMapEditor::EndEditor()
{
}

void UMapEditor::Update()
{
	if (KEY_DOWN(KEY_INPUT_F))
		_bFPV = !_bFPV;

	if (_bFPV)
		_pCam->Update();
}

void UMapEditor::UpdateGui()
{
	ImGui::Begin("Map Editor");
	ImGui::Checkbox("FPV", &_bFPV);
	ImGui::End();
}

void UMapEditor::Render()
{
}

void UMapEditor::CleanUp()
{
	if (_pCam)
	{
		delete _pCam;
		_pCam = nullptr;
	}
}
