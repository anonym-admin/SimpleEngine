#pragma once

class Actor;
class Transform;

class Collider
{
public:
	static AkBool DRAW_COLLIDER;

	Collider();
	~Collider();

	AkBool Initialize();
	void Update();
	void Render();

	void SetOwner(Actor* pOwner) { _pOwner = pOwner; }
	COLLIDER_TYPE GetType() { return _eType; }
	AkU32 GetID() { return _uID; }

	virtual void OnCollisionEnter(Collider* pOther) = 0;
	virtual void OnCollision(Collider* pOther) = 0;
	virtual void OnCollisionExit(Collider* pOther) = 0;

private:
	void CleanUp();

protected:
	static AkU32 s_uID;
	AkU32 _uID = 0;
	Actor* _pOwner = nullptr;
	COLLIDER_TYPE _eType = {};

	IMeshObject* _pMeshObj = nullptr;
	Transform* _pTransform = nullptr;

	Vector3 _vColor = Vector3(0.0f, 1.0f, 0.0f);
};

