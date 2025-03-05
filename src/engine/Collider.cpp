#include "pch.h"
#include "Collider.h"
#include "Transform.h"
#include "Player.h"

AkBool Collider::DRAW_COLLIDER;
AkU32 Collider::s_uID;

Collider::Collider()
{
	if (!Initialize())
	{
		__debugbreak();
	}
}

Collider::~Collider()
{
	CleanUp();
}

AkBool Collider::Initialize()
{
	_uID = s_uID++;

	// Create Trnasform.
	_pTransform = new Transform;

	return AK_TRUE;
}

void Collider::Update()
{
	if (!_pOwner)
		return;

	_pTransform->SetParent(&_pOwner->GetTransform()->WorldTransform);

	_pTransform->Update();
}

void Collider::Render()
{
	if (!DRAW_COLLIDER)
	{
		return;
	}

	GRenderer->RenderBasicMeshObject(_pMeshObj, &_pTransform->WorldTransform);
}

void Collider::CleanUp()
{
	if (_pTransform)
	{
		delete _pTransform;
		_pTransform = nullptr;
	}
	if (_pMeshObj)
	{
		_pMeshObj->Release();
		_pMeshObj = nullptr;
	}
}
