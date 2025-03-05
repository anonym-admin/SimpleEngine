#include "pch.h"
#include "Animation.h"
#include "ModelImporter.h"
#include "Application.h"
#include "SceneManager.h"
#include "LoadingScene.h"
#include "Timer.h"

BoneAnimation_t::BoneAnimation_t()
{
}

BoneAnimation_t::~BoneAnimation_t()
{
	CleanUp();
}

void BoneAnimation_t::Interpolate(const AkF32 fTimePos, Matrix* pOutBoneTransform)
{
	if (!uNumKeyFrame)
	{
		return;
	}

	AkU32 uFront = 0;
	AkU32 uBack = uNumKeyFrame - 1;

	if (fTimePos <= pKeyFrameList[uFront].fTimePos)
	{
		Vector3 vScale = pKeyFrameList[uFront].vScale;
		Quaternion qRot = pKeyFrameList[uFront].qRot;
		Vector3 vPos = pKeyFrameList[uFront].vPos;

		*pOutBoneTransform = Matrix::CreateScale(vScale) * Matrix::CreateFromQuaternion(qRot) * Matrix::CreateTranslation(vPos);
	}
	else if (fTimePos >= pKeyFrameList[uBack].fTimePos)
	{
		Vector3 vScale = pKeyFrameList[uBack].vScale;
		Quaternion qRot = pKeyFrameList[uBack].qRot;
		Vector3 vPos = pKeyFrameList[uBack].vPos;

		*pOutBoneTransform = Matrix::CreateScale(vScale) * Matrix::CreateFromQuaternion(qRot) * Matrix::CreateTranslation(vPos);
	}
	else
	{
		for (AkI32 i = 0; i < (AkI32)uNumKeyFrame - 1; i++)
		{
			if (pKeyFrameList[i].fTimePos <= fTimePos && fTimePos <= pKeyFrameList[i + 1].fTimePos)
			{
				const float fLerpRatio = (fTimePos - pKeyFrameList[i].fTimePos) / (pKeyFrameList[i + 1].fTimePos - pKeyFrameList[i].fTimePos);

				Vector3 vScale0 = pKeyFrameList[i].vScale;
				Vector3 vScale1 = pKeyFrameList[i + 1].vScale;

				Vector3 vPos0 = pKeyFrameList[i].vPos;
				Vector3 vPos1 = pKeyFrameList[i + 1].vPos;

				Quaternion qRot0 = pKeyFrameList[i].qRot;
				Quaternion qRot1 = pKeyFrameList[i + 1].qRot;

				Vector3 vScale = DirectX::XMVectorLerp(vScale0, vScale1, fLerpRatio);
				Vector3 vPos = DirectX::XMVectorLerp(vPos0, vPos1, fLerpRatio);
				Quaternion qRot = DirectX::XMQuaternionSlerp(qRot0, qRot1, fLerpRatio);

				*pOutBoneTransform = Matrix::CreateScale(vScale) * Matrix::CreateFromQuaternion(qRot) * Matrix::CreateTranslation(vPos);
			}
		}
	}
}

float BoneAnimation_t::GetStartTime()
{
	if (!uNumKeyFrame)
	{
		return 0.0f;
	}

	AkU32 uFront = 0;
	return pKeyFrameList[uFront].fTimePos;
}

float BoneAnimation_t::GetEndTime()
{
	if (!uNumKeyFrame)
	{
		return 0.0f;
	}

	AkU32 uBack = uNumKeyFrame - 1;
	return pKeyFrameList[uBack].fTimePos;
}

void BoneAnimation_t::CleanUp()
{
	if (pKeyFrameList)
	{
		delete[] pKeyFrameList;
		pKeyFrameList = nullptr;
	}
}

AnimationClip_t::AnimationClip_t()
{
}

AnimationClip_t::~AnimationClip_t()
{
	CleanUp();
}

void AnimationClip_t::Interpolate(const AkF32 fTimePos, Matrix* pBoneTransform)
{
	for (AkU32 i = 0; i < uNumBoneAnimation; i++)
	{
		pBoneAnimationList[i].Interpolate(fTimePos, &pBoneTransform[i]);
	}
}

float AnimationClip_t::GetClipStartTime()
{
	float fMin = FLT_MAX;
	for (AkU32 i = 0; i < uNumBoneAnimation; i++)
	{
		fMin = DirectX::XMMin(fMin, pBoneAnimationList[i].GetStartTime());
	}

	return fMin;
}

float AnimationClip_t::GetClipEndTime()
{
	float fMax = 0.0f;
	for (AkU32 i = 0; i < uNumBoneAnimation; i++)
	{
		fMax = DirectX::XMMax(fMax, pBoneAnimationList[i].GetEndTime());
	}

	return fMax;
}

void AnimationClip_t::CleanUp()
{
	if (pBoneAnimationList)
	{
		delete[] pBoneAnimationList;
		pBoneAnimationList = nullptr;
	}
}

/*
==============
Animator
==============
*/

HashTable_t* Animation::_pAnimationClipTable;
AkU32 Animation::_uInitRefCount;

Animation::Animation()
{
}

Animation::~Animation()
{
	CleanUp();
}

AkBool Animation::Initialize(AkU32 uMaxClipNum)
{
	// Animation clip 은 공유하는 구조.
	if (!_uInitRefCount)
	{
		_pAnimationClipTable = HT_CreateHashTable(uMaxClipNum, _MAX_PATH, uMaxClipNum);
	}

	_uMaxClipNum = uMaxClipNum;
	_uInitRefCount++;

	return AK_TRUE;
}

void Animation::AddRef()
{
	AkU32 uRefCount = ++_uRefCount;
}

void Animation::Release()
{
	if (_pFinalTransforms)
	{
		free(_pFinalTransforms);
		_pFinalTransforms = nullptr;
	}
	if (_pRootTransforms)
	{
		free(_pRootTransforms);
		_pRootTransforms = nullptr;
	}

	AkU32 uRefCount = --_uRefCount;

	if (!uRefCount)
	{
		delete this;
	}
}

void Animation::GetFinalTransform(const wchar_t* wcClipName, const AkF32 fTimePos, Matrix* pFinalTransform, Matrix* pRootTransform, AkBool bInPlace)
{
	Matrix* pToParentTransform = new Matrix[96];

	AnimationClip_t* pAnimationClip = nullptr;
	AkU32 uKeySize = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pAnimationClip, 1, wcClipName, uKeySize))
	{
		__debugbreak();
	}

	pAnimationClip->Interpolate(fTimePos, pToParentTransform);

	Matrix* pToRootTransform = new Matrix[_uBoneNum];

	pToRootTransform[0] = pToParentTransform[0];

	if (bInPlace)
	{
		// 제자리 애니메이션 적용.
		Vector3 vTranslation = pToRootTransform[0].Translation();

		// For Jump.
		AkF32 fY = vTranslation.y * 0.85f;

		// Init root transform.
		pToRootTransform[0].Translation(Vector3(0.0f, fY, 0.0f));
	}

	for (AkU32 i = 1; i < _uBoneNum; i++)
	{
		AkI32 iParentIndex = _pBoneHierarchyList[i];

		pToRootTransform[i] = pToParentTransform[i] * pToRootTransform[iParentIndex];
	}

	for (AkU32 i = 0; i < _uBoneNum; i++)
	{
		// WARNING!!
		// default matrix
		pRootTransform[i] = _pBoneOffsetMatrixList[i] * pToRootTransform[i];

		pFinalTransform[i] = _mDefaultMatrix.Invert() * _pBoneOffsetMatrixList[i] * pToRootTransform[i] * _mDefaultMatrix;
		pFinalTransform[i] = pFinalTransform[i].Transpose();
	}

	delete[] pToRootTransform;
	pToRootTransform = nullptr;

	delete[] pToParentTransform;
	pToParentTransform = nullptr;
}

void Animation::GetFinalTransform(const wchar_t* wcCurClipName, const wchar_t* wcNextClipName, const AkF32 fAccTime, const AkF32 fBlendingStartTime, Matrix* pFinalTransform, Matrix* pRootTransform, AkBool bInPlace)
{
	Matrix* pToParentTransform = new Matrix[96];

	AnimationClip_t* pCurClip = nullptr;
	AkU32 uKeySize = (AkU32)wcslen(wcCurClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pCurClip, 1, wcCurClipName, uKeySize))
	{
		__debugbreak();
	}

	AnimationClip_t* pNextClip = nullptr;
	uKeySize = (AkU32)wcslen(wcNextClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pNextClip, 1, wcNextClipName, uKeySize))
	{
		__debugbreak();
	}

	if (pCurClip->uNumBoneAnimation != pNextClip->uNumBoneAnimation)
	{
		__debugbreak();
	}

	for (AkU32 i = 0; i < pCurClip->uNumBoneAnimation; i++)
	{
		if (!pCurClip->pBoneAnimationList[i].uNumKeyFrame)
		{
			continue;
		}

		AkBool bCalc0 = AK_FALSE;
		AkBool bCalc1 = AK_FALSE;

		AkU32 uFront = 0;
		AkU32 uBack = pCurClip->pBoneAnimationList[i].uNumKeyFrame - 1;

		Vector3 vScale0;
		Vector3 vScale1;
		Vector3 vPos0;
		Vector3 vPos1;
		Quaternion qRot0;
		Quaternion qRot1;

		if (fAccTime <= pCurClip->pBoneAnimationList[i].pKeyFrameList[uFront].fTimePos)
		{
			Vector3 vScale = pCurClip->pBoneAnimationList[i].pKeyFrameList[uFront].vScale;
			Quaternion qRot = pCurClip->pBoneAnimationList[i].pKeyFrameList[uFront].qRot;
			Vector3 vPos = pCurClip->pBoneAnimationList[i].pKeyFrameList[uFront].vPos;

			pToParentTransform[i] = Matrix::CreateScale(vScale) * Matrix::CreateFromQuaternion(qRot) * Matrix::CreateTranslation(vPos);
		}
		else if (fAccTime >= pCurClip->pBoneAnimationList[i].pKeyFrameList[uBack].fTimePos)
		{
			Vector3 vScale = pCurClip->pBoneAnimationList[i].pKeyFrameList[uBack].vScale;
			Quaternion qRot = pCurClip->pBoneAnimationList[i].pKeyFrameList[uBack].qRot;
			Vector3 vPos = pCurClip->pBoneAnimationList[i].pKeyFrameList[uBack].vPos;

			pToParentTransform[i] = Matrix::CreateScale(vScale) * Matrix::CreateFromQuaternion(qRot) * Matrix::CreateTranslation(vPos);
		}
		else
		{
			// Cur
			for (AkI32 j = 0; j < (AkI32)pCurClip->pBoneAnimationList[i].uNumKeyFrame - 1; j++)
			{
				if (pCurClip->pBoneAnimationList[i].pKeyFrameList[j].fTimePos <= fAccTime && fAccTime <= pCurClip->pBoneAnimationList[i].pKeyFrameList[j + 1].fTimePos)
				{
					vScale0 = pCurClip->pBoneAnimationList[i].pKeyFrameList[j].vScale;
					vPos0 = pCurClip->pBoneAnimationList[i].pKeyFrameList[j].vPos;
					qRot0 = pCurClip->pBoneAnimationList[i].pKeyFrameList[j].qRot;
					bCalc0 = AK_TRUE;
				}
			}
		}

		uFront = 0;
		uBack = pNextClip->pBoneAnimationList[i].uNumKeyFrame - 1;

		if (fBlendingStartTime <= pNextClip->pBoneAnimationList[i].pKeyFrameList[uFront].fTimePos)
		{
			Vector3 vScale = pNextClip->pBoneAnimationList[i].pKeyFrameList[uFront].vScale;
			Quaternion qRot = pNextClip->pBoneAnimationList[i].pKeyFrameList[uFront].qRot;
			Vector3 vPos = pNextClip->pBoneAnimationList[i].pKeyFrameList[uFront].vPos;

			pToParentTransform[i] = Matrix::CreateScale(vScale) * Matrix::CreateFromQuaternion(qRot) * Matrix::CreateTranslation(vPos);
		}
		else if (fBlendingStartTime >= pNextClip->pBoneAnimationList[i].pKeyFrameList[uBack].fTimePos)
		{
			Vector3 vScale = pNextClip->pBoneAnimationList[i].pKeyFrameList[uBack].vScale;
			Quaternion qRot = pNextClip->pBoneAnimationList[i].pKeyFrameList[uBack].qRot;
			Vector3 vPos = pNextClip->pBoneAnimationList[i].pKeyFrameList[uBack].vPos;

			pToParentTransform[i] = Matrix::CreateScale(vScale) * Matrix::CreateFromQuaternion(qRot) * Matrix::CreateTranslation(vPos);
		}
		else
		{
			// Next
			for (AkI32 j = 0; j < (AkI32)pNextClip->pBoneAnimationList[i].uNumKeyFrame - 1; j++)
			{
				if (pNextClip->pBoneAnimationList[i].pKeyFrameList[j].fTimePos <= fBlendingStartTime && fBlendingStartTime <= pNextClip->pBoneAnimationList[i].pKeyFrameList[j + 1].fTimePos)
				{
					vScale1 = pNextClip->pBoneAnimationList[i].pKeyFrameList[j].vScale;
					vPos1 = pNextClip->pBoneAnimationList[i].pKeyFrameList[j].vPos;
					qRot1 = pNextClip->pBoneAnimationList[i].pKeyFrameList[j].qRot;
					bCalc1 = AK_TRUE;
				}
			}
		}

		if (bCalc0 && bCalc1)
		{
			printf("%lf\n", fBlendingStartTime);
			printf("%lf\n", pCurClip->pBoneAnimationList[i].pKeyFrameList[uBack].fTimePos);


			const AkF32 fLerpRatio = fBlendingStartTime / (pCurClip->pBoneAnimationList[i].pKeyFrameList[uBack].fTimePos - fAccTime);

			Vector3 vScale = DirectX::XMVectorLerp(vScale0, vScale1, fLerpRatio);
			Vector3 vPos = DirectX::XMVectorLerp(vPos0, vPos1, fLerpRatio);
			Quaternion qRot = DirectX::XMQuaternionSlerp(qRot0, qRot1, fLerpRatio);

			pToParentTransform[i] = Matrix::CreateScale(vScale) * Matrix::CreateFromQuaternion(qRot) * Matrix::CreateTranslation(vPos);
		}
	}



	///////////////////////////////////////////////////////////////////////////////





	Matrix* pToRootTransform = new Matrix[_uBoneNum];

	pToRootTransform[0] = pToParentTransform[0];

	if (bInPlace)
	{
		// 제자리 애니메이션 적용.
		Vector3 vTranslation = pToRootTransform[0].Translation();

		// For Jump.
		AkF32 fY = vTranslation.y * 0.85f;

		// Init root transform.
		pToRootTransform[0].Translation(Vector3(0.0f, fY, 0.0f));
	}

	for (AkU32 i = 1; i < _uBoneNum; i++)
	{
		AkI32 iParentIndex = _pBoneHierarchyList[i];

		pToRootTransform[i] = pToParentTransform[i] * pToRootTransform[iParentIndex];
	}

	for (AkU32 i = 0; i < _uBoneNum; i++)
	{
		// WARNING!!
		// default matrix
		pRootTransform[i] = _pBoneOffsetMatrixList[i] * pToRootTransform[i];

		pFinalTransform[i] = _mDefaultMatrix.Invert() * _pBoneOffsetMatrixList[i] * pToRootTransform[i] * _mDefaultMatrix;
		pFinalTransform[i] = pFinalTransform[i].Transpose();
	}

	delete[] pToRootTransform;
	pToRootTransform = nullptr;

	delete[] pToParentTransform;
	pToParentTransform = nullptr;
}

void Animation::GetFinalTransform(const wchar_t* wcClipName, AkU32 uFrame, Matrix* pFinalTransform)
{
	Matrix* pToParentTransform = new Matrix[96];

	AnimationClip_t* pCurClip = nullptr;
	AkU32 uKeySize = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pCurClip, 1, wcClipName, uKeySize))
	{
		__debugbreak();
	}

	for (AkU32 i = 0; i < pCurClip->uNumBoneAnimation; i++)
	{
		if (!pCurClip->pBoneAnimationList[i].uNumKeyFrame)
		{
			pToParentTransform[i] = Matrix();
			continue;
		}

		uFrame %= pCurClip->pBoneAnimationList[i].uNumKeyFrame;

		Vector3 vScale = pCurClip->pBoneAnimationList[i].pKeyFrameList[uFrame].vScale;
		Vector3 vPos = pCurClip->pBoneAnimationList[i].pKeyFrameList[uFrame].vPos;
		Quaternion qRot = pCurClip->pBoneAnimationList[i].pKeyFrameList[uFrame].qRot;

		pToParentTransform[i] = Matrix::CreateScale(vScale) * Matrix::CreateFromQuaternion(qRot) * Matrix::CreateTranslation(vPos);
	}

	Matrix* pToRootTransform = new Matrix[_uBoneNum];

	pToRootTransform[0] = pToParentTransform[0];

	for (AkU32 i = 1; i < _uBoneNum; i++)
	{
		AkI32 iParentIndex = _pBoneHierarchyList[i];

		pToRootTransform[i] = pToParentTransform[i] * pToRootTransform[iParentIndex];
	}

	for (AkU32 i = 0; i < _uBoneNum; i++)
	{
		pFinalTransform[i] = _mDefaultMatrix.Invert() * _pBoneOffsetMatrixList[i] * pToRootTransform[i] * _mDefaultMatrix;
		pFinalTransform[i] = pFinalTransform[i].Transpose();
	}

	delete[] pToRootTransform;
	pToRootTransform = nullptr;

	delete[] pToParentTransform;
	pToParentTransform = nullptr;
}

AkU32 Animation::GetClipTickPerSecond(const wchar_t* wcClipName)
{
	AnimationClip_t* pAnimationClip = nullptr;
	AkU32 uKeySize = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pAnimationClip, 1, wcClipName, uKeySize))
	{
		__debugbreak();
	}

	return pAnimationClip->uTickPerSecond;
}

AkU32 Animation::GetClipDuration(const wchar_t* wcClipName)
{
	AnimationClip_t* pAnimationClip = nullptr;
	AkU32 uKeySize = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pAnimationClip, 1, wcClipName, uKeySize))
	{
		__debugbreak();
	}

	return pAnimationClip->uDuration;
}

AkF32 Animation::GetClipStartTime(const wchar_t* wcClipName)
{
	AnimationClip_t* pAnimationClip = nullptr;
	AkU32 uKeySize = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pAnimationClip, 1, wcClipName, uKeySize))
	{
		__debugbreak();
	}

	return pAnimationClip->GetClipStartTime();
}

AkF32 Animation::GetClipEndTime(const wchar_t* wcClipName)
{
	AnimationClip_t* pAnimationClip = nullptr;
	AkU32 uKeySize = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pAnimationClip, 1, wcClipName, uKeySize))
	{
		__debugbreak();
	}

	return pAnimationClip->GetClipEndTime();
}

AkF32 Animation::GetClipCurrentTime(const wchar_t* wcClipName)
{
	AnimationClip_t* pAnimationClip = nullptr;
	AkU32 uKeySize = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pAnimationClip, 1, wcClipName, uKeySize))
	{
		__debugbreak();
	}

	return pAnimationClip->GetClipCurrentTime();
}

void Animation::SetDefaultMatrix(const Matrix* mDefaultMat)
{
	_mDefaultMatrix = *mDefaultMat;

	_pFinalTransforms = reinterpret_cast<Matrix*>(malloc(sizeof(Matrix) * 96));
	memset(_pFinalTransforms, 0, sizeof(Matrix) * 96);

	_pRootTransforms = reinterpret_cast<Matrix*>(malloc(sizeof(Matrix) * 96));
	memset(_pRootTransforms, 0, sizeof(Matrix) * 96);
}

void Animation::DestroyAnimationClip(const wchar_t* wcClipName)
{
	AnimationClip_t* pAnimClip = nullptr;
	AkU32 uKeyLength = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);
	if (HT_Find(_pAnimationClipTable, (void**)&pAnimClip, 1, wcClipName, uKeyLength))
	{
		if (pAnimClip)
		{
			if (pAnimClip->pSearchHandle)
			{
				HT_Delete(_pAnimationClipTable, pAnimClip->pSearchHandle);
				pAnimClip->pSearchHandle = nullptr;
			}

			delete[] pAnimClip;
		}
	}
}

AnimationClip_t* Animation::ReadFromAnimationFile(const wchar_t* wcBasePath, const wchar_t* wcFilename)
{
	ULoadingScene* pSceneLoading = (ULoadingScene*)GSceneManager->GetCurrentScene();

	ModelImporter tModelImporter = { };

	tModelImporter.LoadAnimation(wcBasePath, wcFilename, _uBoneNum);

	wchar_t wcFullPath[MAX_PATH] = {};
	wcscpy_s(wcFullPath, wcBasePath);
	wcscat_s(wcFullPath, wcFilename);
	wcscat_s(wcFullPath, L"\n");
	pSceneLoading->RenderLoadingScreenCallBack(wcFullPath);

	AnimationClip_t* pAnimClip = tModelImporter.GetAnimationClip();

	AddAnimationClip(pAnimClip, wcFilename);

	return pAnimClip;
}

AkBool Animation::PlayAnimation(const wchar_t* wcAnimClipname, AkBool bInPlace)
{
	AkBool bIsEnd = AK_FALSE;

	AkU32 uTickPerSecond = GetClipTickPerSecond(wcAnimClipname);
	AkU32 uDuration = GetClipDuration(wcAnimClipname);

	AkF32 fAnimTime = GetClipCurrentTime(wcAnimClipname);
	fAnimTime += (AkF32)uTickPerSecond * DT;

	if (fAnimTime >= GetClipEndTime(wcAnimClipname))
	{
		fAnimTime = 0.0f;
		bIsEnd = AK_TRUE;
	}

	GetFinalTransform(wcAnimClipname, fAnimTime, _pFinalTransforms, _pRootTransforms, bInPlace);
	SetClipCurrentTime(wcAnimClipname, fAnimTime);

	return bIsEnd;
}

AkBool Animation::PlayAnimationBlending(const wchar_t* wcCurClipName, const wchar_t* wcNextClipName)
{
	AkBool bIsBlending = AK_FALSE;
	AkBool bIsCurEnd = AK_FALSE;

	// Cur
	AkU32 uCurTickPerSecond = GetClipTickPerSecond(wcCurClipName);
	AkU32 uCurDuration = GetClipDuration(wcCurClipName);

	AkF32 fCurAnimTime = GetClipCurrentTime(wcCurClipName);
	fCurAnimTime += (AkF32)uCurTickPerSecond * DT;

	AkF32 fCurEndTiem = GetClipEndTime(wcCurClipName);
	AkF32 fNextEndTiem = GetClipEndTime(wcNextClipName);

	if (fCurAnimTime >= fCurEndTiem - 10.0f) // Blending Start Time 이상이 되면
	{
		bIsBlending = AK_TRUE;
	}

	if (fCurAnimTime >= GetClipEndTime(wcCurClipName))
	{
		bIsBlending = AK_FALSE;
		bIsCurEnd = AK_TRUE;
	}

	if (!bIsBlending)
	{
		GetFinalTransform(wcCurClipName, fCurAnimTime, _pFinalTransforms, _pRootTransforms, AK_FALSE);
		SetClipCurrentTime(wcCurClipName, fCurAnimTime);
	}

	SetClipCurrentTime(wcCurClipName, fCurAnimTime);

	// Next
	AkBool bIsNextEnd = AK_FALSE;
	if (bIsBlending)
	{
		AkU32 uNextTickPerSecond = GetClipTickPerSecond(wcNextClipName);
		AkU32 uNextDuration = GetClipDuration(wcNextClipName);

		AkF32 fNextAnimTime = GetClipCurrentTime(wcNextClipName);
		fNextAnimTime += (AkF32)uNextTickPerSecond * DT;

		if (fNextAnimTime >= GetClipEndTime(wcNextClipName))
		{
			__debugbreak();
		}

		GetFinalTransform(wcCurClipName, wcNextClipName, fCurAnimTime, fNextAnimTime, _pFinalTransforms, _pRootTransforms, AK_FALSE);
		SetClipCurrentTime(wcNextClipName, fNextAnimTime);
	}
	else
	{
		if (bIsCurEnd)
		{
			AkU32 uNextTickPerSecond = GetClipTickPerSecond(wcNextClipName);
			AkU32 uNextDuration = GetClipDuration(wcNextClipName);

			AkF32 fNextAnimTime = GetClipCurrentTime(wcNextClipName);
			fNextAnimTime += (AkF32)uNextTickPerSecond * DT;

			if (fNextAnimTime >= GetClipEndTime(wcNextClipName))
			{
				fCurAnimTime = 0.0f;
				fNextAnimTime = 0.0f;
				bIsNextEnd = AK_TRUE;
			}

			GetFinalTransform(wcNextClipName, fNextAnimTime, _pFinalTransforms, _pRootTransforms, AK_FALSE);
			SetClipCurrentTime(wcNextClipName, fNextAnimTime);
		}
	}

	if (bIsNextEnd)
	{
		SetClipCurrentTime(wcCurClipName, fCurAnimTime);
	}

	return bIsNextEnd;
}

AkBool Animation::PlayAnimationWithFrame(const wchar_t* wcClipName, AkU32 uFrame)
{
	GetFinalTransform(wcClipName, uFrame, _pFinalTransforms);

	return AK_TRUE;
}

void Animation::SetClipCurrentTime(const wchar_t* wcClipName, AkF32 fCurTime)
{
	AnimationClip_t* pAnimationClip = nullptr;
	AkU32 uKeySize = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);
	if (!HT_Find(_pAnimationClipTable, (void**)&pAnimationClip, 1, wcClipName, uKeySize))
	{
		__debugbreak();
	}

	pAnimationClip->SetClipCurrentTime(fCurTime);
}

void Animation::AddAnimationClip(AnimationClip_t* pAnimClip, const wchar_t* wcClipName)
{
	AkU32 uClipNameByteSize = (AkU32)wcslen(wcClipName) * sizeof(wchar_t);

	if (HT_Find(_pAnimationClipTable, (void**)&pAnimClip, 1, wcClipName, uClipNameByteSize))
	{
		return;
	}

	void* pSearchHandle = HT_Insert(_pAnimationClipTable, pAnimClip, wcClipName, uClipNameByteSize);
	pAnimClip->pSearchHandle = pSearchHandle;
}

void Animation::CleanUp()
{
	if (_pBoneOffsetMatrixList)
	{
		delete[] _pBoneOffsetMatrixList;
		_pBoneOffsetMatrixList = nullptr;
	}
	if (_pBoneHierarchyList)
	{
		delete[] _pBoneHierarchyList;
		_pBoneHierarchyList = nullptr;
	}
	if (_pAnimationClipTable)
	{
		AkU32 uInitRefCount = --_uInitRefCount;

		if (!uInitRefCount)
		{
			HT_DestroyHashTable(_pAnimationClipTable);
			_pAnimationClipTable = nullptr;
		}
	}
}
