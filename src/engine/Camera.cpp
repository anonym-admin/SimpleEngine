#include "pch.h"
#include "Camera.h"
#include "Application.h"
#include "GameInput.h"
#include "Actor.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Timer.h"

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
	CleanUp();
}

AkBool Camera::Initialize(const Vector3* pPos, const Vector3* pYawPirchRoll)
{
	_pTransform = new Transform;

	SetPosition(pPos);
	SetRotation(pYawPirchRoll);
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

void Camera::SetPosition(const Vector3* pPos)
{
	_pTransform->Position = *pPos;
	GRenderer->SetCameraPosition(pPos->x, pPos->y, pPos->z);
}

void Camera::SetRotation(const Vector3* pYawPitchRoll)
{
	_pTransform->Rotation = *pYawPitchRoll;
	GRenderer->RotateYawPitchRollCamera(pYawPitchRoll->x, pYawPitchRoll->y, pYawPitchRoll->z);
}

Vector3 Camera::GetPosition()
{
	return _pTransform->Position;
}

Vector3 Camera::GetDirection()
{
	Vector3 vDir = Vector3::Transform(_vCamInitDir, Matrix::CreateFromYawPitchRoll(_pTransform->Rotation.x, _pTransform->Rotation.y, _pTransform->Rotation.z));
	vDir.Normalize();
	return vDir;
}

void Camera::CleanUp()
{
	if (_pTransform)
	{
		delete _pTransform;
		_pTransform = nullptr;
	}
}

void Camera::MoveFree()
{
}

void Camera::MoveEditor()
{
	Vector3 vPos = GetPosition();
	Vector3 vDir = GetDirection();
	Vector3 vUp = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 vRight = vUp.Cross(vDir);
	Vector3 vDeltaPos = Vector3(0.0f);
	vRight.Normalize();

	if (KEY_HOLD(KEY_INPUT_W))
	{
		vDeltaPos += (_fCamSpeed * vDir * DT);
	}
	if (KEY_HOLD(KEY_INPUT_S))
	{
		vDeltaPos += (_fCamSpeed * -vDir * DT);
	}
	if (KEY_HOLD(KEY_INPUT_D))
	{
		vDeltaPos += (_fCamSpeed * vRight * DT);
	}
	if (KEY_HOLD(KEY_INPUT_A))
	{
		vDeltaPos += (_fCamSpeed * -vRight * DT);
	}
	if (KEY_HOLD(KEY_INPUT_Q))
	{
		vDeltaPos += (_fCamSpeed * vUp * DT);
	}
	if (KEY_HOLD(KEY_INPUT_E))
	{
		vDeltaPos += (_fCamSpeed * -vUp * DT);
	}

	vPos += vDeltaPos;
	SetPosition(&vPos);
	GRenderer->MoveCamera(vDeltaPos.x, vDeltaPos.y, vDeltaPos.z);
}

void Camera::MoveFollow()
{
}

