#include "pch.h"
#include "Actor.h"
#include "Camera.h"
#include "SphereCollider.h"


Actor::~Actor()
{
	CleanUp();
}

Camera* Actor::CreateCamera(const Vector3* pPos)
{
	Vector3 vYawPitchRoll = Vector3(0.0f);
	Camera* pCam = new Camera(pPos, &vYawPitchRoll);
	pCam->Mode = CameraMode::EDITOR;
	return pCam;
}

Collider* Actor::CreateCollider(COLLIDER_TYPE eType)
{
	Collider* pCollider = nullptr;
	switch (eType)
	{
	case COLLIDER_TYPE::COLLIDER_TYPE_SPHERE:
		pCollider = new SphereCollider(1.0f);
		pCollider->SetOwner(this);
		break;
	case COLLIDER_TYPE::COLLIDER_TYPE_BOX:
		break;
	}
	return pCollider;
}

void Actor::CleanUp()
{
	AkU32 uRefCount = s_uInitRefCount - 1;
	if (uRefCount)
	{
		return;
	}

	if (_pCamera)
	{
		delete _pCamera;
		_pCamera = nullptr;
	}
	if (_pCollider)
	{
		delete _pCollider;
		_pCollider = nullptr;
	}
}
