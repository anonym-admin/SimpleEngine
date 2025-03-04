#pragma once

/*
============
Camera
============
*/

class UCamera
{
public:
	UCamera(const Vector3* pPos, const Vector3* pYawPitchRoll);
	~UCamera();

	enum class CamMode
	{
		FREE,
		FOLLOW,
		EDITOR,
	} Mode;

	AkBool Initialize(const Vector3* pPos, const Vector3* pInitDir);
	void Update();
	void Render();
	void SetPosition(const Vector3* pPos);
	void SetDirection(const Vector3* pDir);

	void Editor();

private:
	void UpdateFreeMode();
	void UpdateFollowMode();
	void UpdateEditorMode();

	void Move();
	void MoveEditorMode();
	void Rotation();

private:
	AkF32 _fMoveSpeed = 1.0f;
	AkF32 _fRotSpeed = 1.0f;
	Vector3 _vInitCamDir = Vector3(0.0f, 0.0f, 1.0f);
	Vector3 _vCamPos = Vector3(0.0f);
	Vector3 _vCamDir = Vector3(0.0f, 0.0f, 1.0f);
};

