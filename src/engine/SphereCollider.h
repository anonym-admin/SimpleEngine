#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(AkF32 fRadius = 1.0f, AkU32 uStack = 15, AkU32 uSlice = 30);
	~SphereCollider();

	AkBool Initialize(AkF32 fRadius, AkU32 uStack, AkU32 uSlice);

	virtual void OnCollisionEnter(Collider* pOther) override;
	virtual void OnCollision(Collider* pOther) override;
	virtual void OnCollisionExit(Collider* pOther) override;

private:
	AkF32 _fRadius = 1.0f;
};

