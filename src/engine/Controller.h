#pragma once

class Player;

class Controller
{
public:
	Controller(Player* pOwner);
	~Controller();

	AkBool Initialize(Player* pOwner);
	void Update();

private:
	Player* _pOwner = nullptr;
};