#include "pch.h"
#include "Player.h"
#include "Controller.h"

Player::~Player()
{
	CleanUp();
}

Controller* Player::CreateController()
{
	Controller* pController = new Controller(this);
	return pController;
}

void Player::CleanUp()
{
	AkU32 uRefCount = s_uInitRefCount - 1;
	if (uRefCount)
	{
		return;
	}

	if (_pController)
	{
		delete _pController;
		_pController = nullptr;
	}
}
