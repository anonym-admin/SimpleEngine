#include "pch.h"
#include "Spawn.h"

Spawn::Spawn()
{
}

Spawn::~Spawn()
{
	CleanUp();
}

void Spawn::CleanUp()
{
	AkU32 uRefCount = s_uInitRefCount - 1;
	if (uRefCount)
	{
		return;
	}
}
