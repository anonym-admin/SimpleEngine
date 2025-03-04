#include "pch.h"
#include "Camera.h"
#include "GameInput.h"
#include "Timer.h"

/*
============
Camera
============
*/

UCamera::UCamera(const Vector3* pPos, const Vector3* pInitDir)
{
	Initialize(pPos, pInitDir);
}

UCamera::~UCamera()
{

}

AkBool UCamera::Initialize(const Vector3* pPos, const Vector3* pYawPitchRoll)
{
	SetPosition(pPos);
	SetDirection(pYawPitchRoll);
	return AK_TRUE;
}

void UCamera::Update()
{
	switch (Mode)
	{
	case CamMode::FREE:
		break;
	case CamMode::FOLLOW:
		break;
	case CamMode::EDITOR:
		UpdateEditorMode();
		break;
	}
}

void UCamera::Render()
{
}

void UCamera::SetPosition(const Vector3* pPos)
{
	_vCamPos = *pPos;
	RENDERER->SetCameraPosition(_vCamPos.x, _vCamPos.y, _vCamPos.z);
}

void UCamera::SetDirection(const Vector3* pYawPitchRoll)
{
	_vCamDir = Vector3::Transform(_vInitCamDir, Matrix::CreateFromYawPitchRoll(pYawPitchRoll->x, pYawPitchRoll->y, pYawPitchRoll->z));
	RENDERER->RotateYawPitchRollCamera(pYawPitchRoll->x, pYawPitchRoll->y, pYawPitchRoll->z);
}

void UCamera::Editor()
{
	ImGui::Begin("Camera Edit");
	ImGui::End();
}

void UCamera::UpdateFreeMode()
{
	Move();
	Rotation();
}

void UCamera::UpdateFollowMode()
{
	Move();
	Rotation();
}

void UCamera::UpdateEditorMode()
{
	Move();
	Rotation();
}

void UCamera::Move()
{
	switch (Mode)
	{
	case CamMode::FREE:
		break;
	case CamMode::FOLLOW:
		break;
	case CamMode::EDITOR:
		MoveEditorMode();
		break;
	}
}

void UCamera::MoveEditorMode()
{
	Vector3 vDeltaPos = Vector3(0.0f);

	if (KEY_HOLD(KEY_INPUT_W))
	{
		vDeltaPos = Vector3(0.0f, 0.0f, DT * _fMoveSpeed);
	}
	if (KEY_HOLD(KEY_INPUT_S))
	{
		vDeltaPos = Vector3(0.0f, 0.0f, -DT * _fMoveSpeed);
	}
	if (KEY_HOLD(KEY_INPUT_A))
	{
		vDeltaPos = Vector3(-DT * _fMoveSpeed, 0.0f, 0.0f);
	}
	if (KEY_HOLD(KEY_INPUT_D))
	{
		vDeltaPos = Vector3(DT * _fMoveSpeed, 0.0f, 0.0f);
	}
	if (KEY_HOLD(KEY_INPUT_Q))
	{
		vDeltaPos = Vector3(0.0f, DT * _fMoveSpeed, 0.0f);
	}
	if (KEY_HOLD(KEY_INPUT_E))
	{
		vDeltaPos = Vector3(0.0f, -DT * _fMoveSpeed, 0.0f);
	}

	_vCamPos += vDeltaPos;
	RENDERER->MoveCamera(vDeltaPos.x, vDeltaPos.y, vDeltaPos.z);
}

void UCamera::Rotation()
{
	RECT tRect = {};
	GetClientRect(GhWnd, &tRect);
	AkU32 uScreenWidth = (tRect.right - tRect.left);
	AkU32 uScreenHeight = (tRect.bottom - tRect.top);

	Vector3 vYawPicthRoll = Vector3(0.0f);
	vYawPicthRoll.x = GGameInput->GetClampNdcX() * DirectX::XM_PI;
	vYawPicthRoll.y = GGameInput->GetClampNdcY() * -DirectX::XM_PIDIV2;

	SetDirection(&vYawPicthRoll);
}
