#include "pch.h"
#include "Controller.h"
#include "GameInput.h"

Controller::Controller(Player* pOwner)
{
	if (!Initialize(pOwner))
	{
		__debugbreak();
	}
}

Controller::~Controller()
{
}

AkBool Controller::Initialize(Player* pOwner)
{
	_pOwner = pOwner;
	return AK_TRUE;
}

void Controller::Update()
{
	if (KEY_HOLD(KEY_INPUT_W))
	{

	}
	if (KEY_HOLD(KEY_INPUT_S))
	{

	}
	if (KEY_HOLD(KEY_INPUT_A))
	{

	}
	if (KEY_HOLD(KEY_INPUT_D))
	{

	}
	if (KEY_HOLD(KEY_INPUT_SPACE))
	{

	}
	if (LBTN_DOWN)
	{

	}
	if (LBTN_HOLD)
	{

	}
}