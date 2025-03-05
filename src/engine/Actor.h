#pragma once

#include "BaseObject.h"

class Camera;
class Collider;

class Actor : public BaseObject
{
public:
	virtual ~Actor();

	virtual void Update() = 0;
	virtual void FinalUpdate() = 0;
	virtual void Render() = 0;
	virtual void RenderShadow() = 0;

	virtual void OnCollisionEnter(Collider* pOther) = 0;
	virtual void OnCollision(Collider* pOther) = 0;
	virtual void OnCollisionExit(Collider* pOther) = 0;

	Camera* GetCamera() { return _pCamera; }
	Collider* GetCollider() { return _pCollider; }

private:
	void CleanUp();

protected:
	Camera* CreateCamera(const Vector3* pPos);
	Collider* CreateCollider(COLLIDER_TYPE eType);

protected:
	Camera* _pCamera = nullptr;
	Collider* _pCollider = nullptr;
};