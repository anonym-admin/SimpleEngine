#include "pch.h"
#include "Camera.h"
#include "Application.h"
#include "GameInput.h"
#include "Actor.h"
#include "RigidBody.h"

/*
===================
Camera base class
===================
*/

Camera::Camera(const Vector3* pPos, const Vector3* pYawPirchRoll)
{
	Initialize(pPos, pYawPirchRoll);
}

Camera::~Camera()
{

}

AkBool Camera::Initialize(const Vector3* pPos, const Vector3* pYawPirchRoll)
{
	SetCameraPosition(pPos);
	SetCameraDirection(pYawPirchRoll);
	return AK_TRUE;
}

void Camera::Update()
{
	switch (Mode)
	{
	case CameraMode::FREE:
		MoveFree();

		break;
	case CameraMode::EDITOR:
		MoveEditor();

		break;

	case CameraMode::FOLLOW:
		MoveFollow();

		break;
	}
}

void Camera::Render()
{

}

void Camera::SetCameraPosition(const Vector3* pPos)
{
	_vCamPos = *pPos;
	GRenderer->SetCameraPosition(_vCamPos.x, _vCamPos.y, _vCamPos.z);
}

void Camera::SetCameraDirection(const Vector3* pYawPitchRoll)
{
	_vCamCurDir = Vector3::Transform(_vCamInitDir, Matrix::CreateFromYawPitchRoll(pYawPitchRoll->x, pYawPitchRoll->y, pYawPitchRoll->z));
	GRenderer->RotateYawPitchRollCamera(pYawPitchRoll->x, pYawPitchRoll->y, pYawPitchRoll->z);
}

void Camera::MoveFree()
{
}

void Camera::MoveEditor()
{
}

void Camera::MoveFollow()
{
}

