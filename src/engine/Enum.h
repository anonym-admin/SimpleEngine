#pragma once

/*
===========
Scene Type
===========
*/

enum class SCENE_TYPE
{
	SCENE_TYPE_LOADING,
	SCENE_TYPE_INGANE,
	SCENE_TYPE_COUNT,
};

/*
=======================
Game Object Type
=======================
*/

enum class GAME_OBJECT_GROUP_TYPE
{
	GAME_OBJ_GROUP_TYPE_PLAYER,
	GAME_OBJ_GROUP_TYPE_DANCER,
	GAME_OBJ_GROUP_TYPE_WEAPON,
	GAME_OBJ_GROUP_TYPE_MAP, // 움직이지 않는 오브젝트
	GAME_OBJ_GROUP_TYPE_COUNT = 32,
};

/*
======================
UI Object Type
======================
*/

enum class UI_OBJECT_TYPE
{
	UI_OBJ_SYS_INFO_TEXT,
	UI_OBJ_CHAT_INPUT_TEXT,
	UI_OBJ_TEST_STATIC_TEXT,
	UI_OBJ_EXIT,
	UI_OBJ_TYPE_COUNT = 32,
};

/*
============
Editor Type
============
*/

enum class EDITOR_TYPE
{
	EDITOR_TYPE_MODEL,
	EDITOR_TYPE_MAP,
	EDITOR_TYPE_COUNT,
};

/*
================
Collider Type
================
*/

enum class COLLIDER_TYPE
{
	COLLIDER_TYPE_SPHERE,
	COLLIDER_TYPE_BOX,
	COLLIDER_TYPE_PLANE,
	COLLIDER_TYPE_TRI_LIST,
	COLLIDER_TYPE_SPHERE_GROUP,
	COLLIDER_TYPE_CAPSULE,
};

/*
================
Game Event Type
================
*/

enum class EVENT_TYPE
{
	EVENT_TYPE_CREATE_GAME_OBJECT,
	EVENT_TYPE_SCENE_CHANGE,
	EVENT_TYPE_EDITOR_CHANGE,
	EVENT_TYPE_SCENE_TO_MODEL_EDITOR_CHANGE,
	EVENT_TYPE_SCENE_TO_MAP_EDITOR_CHANGE,
	EVENT_TYPE_EDITOR_TO_SCENE_CHANGE,
};

/*
==================
Editor Event Type
==================
*/

enum class EDITOR_EVENT_TYPE
{
	EDITOR_EVENT_TYPE_CREATE_GAME_OBJECT,
	EDITOR_EVENT_TYPE_EDITOR_CHANGE,
};

/*
==================
Camera Type
==================
*/

enum class CameraMode
{
	FREE,
	EDITOR,
	FOLLOW,
};
