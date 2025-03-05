#pragma once

/*
===================
Camera base class
===================
*/

class Transform;

class Camera
{
public:
	Camera(const Vector3* pPos, const Vector3* pYawPirchRoll);
	~Camera();

	AkBool Initialize(const Vector3* pPos, const Vector3* pYawPirchRoll);
	void Update();
	void Render();
	void SetPosition(const Vector3* pPos);
	void SetRotation(const Vector3* pYawPitchRoll);
	Vector3 GetPosition();
	Vector3 GetDirection();

private:
	void CleanUp();

	void MoveFree();
	void MoveEditor();
	void MoveFollow();
	
private:
	Vector3 _vCamInitDir = Vector3(0.0f, 0.0f, 1.0f);
	Transform* _pTransform = nullptr;
	AkF32 _fCamSpeed = 1.0f;

public:
	CameraMode Mode = {};
};

