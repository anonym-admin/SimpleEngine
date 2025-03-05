#pragma once

/*
===================
Camera base class
===================
*/

class Camera
{
public:
	Camera(const Vector3* pPos, const Vector3* pYawPirchRoll);
	~Camera();

	AkBool Initialize(const Vector3* pPos, const Vector3* pYawPirchRoll);
	void Update();
	void Render();
	void SetCameraPosition(const Vector3* pPos);
	void SetCameraDirection(const Vector3* pYawPitchRoll);
	Vector3 GetCameraPosition() { return _vCamPos; }
	Vector3 GetCameraDirection() { return _vCamCurDir; }

private:
	void MoveFree();
	void MoveEditor();
	void MoveFollow();
	
private:
	Vector3 _vCamInitDir = Vector3(0.0f, 0.0f, 1.0f);
	Vector3 _vCamCurDir = Vector3(0.0f);
	Vector3 _vCamPos = Vector3(0.0f);
	AkF32 _fCamSpeed = 1.0f;

public:
	CameraMode Mode = {};
};

