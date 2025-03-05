#pragma once

/*
=====================================
Asset Handle => using Asset Manager
=====================================
*/
struct AssetMeshDataContainer_t
{
	MeshData_t* pMeshData = nullptr;
	AkU32 uMeshDataNum = 0;
	Matrix mDefaultMat = Matrix();
	const Matrix* pBoneOffsetMatList = nullptr;
	const AkI32* pBoneHierarchyList = nullptr;
	AkU32 uBoneNum = 0;
};

struct AssetTextureContainer_t
{
	void* pTexHandle = nullptr;
};

/*
==================
Animator Handle
==================
*/

struct AnimatorHandle_t
{
	Matrix mDefaultMat = Matrix();
	const Matrix* pBoneOffsetMatList = nullptr;
	const AkI32* pBoneHierarchyList = nullptr;
	AkU32 uBoneNum = 0;
};

/*
==============================
Event Handler Type
==============================
*/

struct EventCreateGameObjectParam_t
{

};

struct EventSceneChangeParam_t
{
	SCENE_TYPE eBefore;
	SCENE_TYPE eAfter;
};

struct EventEditorChangeParam_t
{
	EDITOR_TYPE eBefore;
	EDITOR_TYPE eAfter;
};

struct EventChangeSceneEditorParam_t
{
	SCENE_TYPE eScene;
	EDITOR_TYPE eEditor;
};

struct EventHandle_t
{
	EVENT_TYPE eEventType = {};
	void* pObj = nullptr;

	union
	{
		EventCreateGameObjectParam_t tCreateGameObjParam;
		EventSceneChangeParam_t tSceneChangeParam;
		EventEditorChangeParam_t tEditorChangeParam;
		EventChangeSceneEditorParam_t tSceneEditorChangeParam;
	};
};
