#include "pch.h"
#include "Sound.h"

/*
===================
Sound Manager
===================
*/

SoundManager::SoundManager()
{
	Initialize();
}

SoundManager::~SoundManager()
{
	CleanUp();
}

AkBool SoundManager::Initialize()
{
	FMOD_RESULT tRet;

	tRet = FMOD::System_Create(&_pSystem);
	if (tRet != FMOD_OK)
	{
		return AK_FALSE;
	}

	tRet = _pSystem->init(32, FMOD_INIT_NORMAL, nullptr);
	if (tRet != FMOD_OK)
	{
		return AK_FALSE;
	}

	return AK_TRUE;
}

void SoundManager::Update()
{
	_pSystem->update();
}

void SoundManager::Render()
{
}

USound* SoundManager::LoadSound(const char* pFilename)
{
	FMOD_RESULT tRet;

	USound* pSound = new USound;
	if (!pSound->Initialize(_pSystem))
	{
		__debugbreak();
		delete pSound;
		pSound = nullptr;
		return pSound;
	}

	FMOD::Sound** ppSysSound = pSound->GetSoundAddressOf();

	tRet = _pSystem->createSound(pFilename, FMOD_DEFAULT, 0, ppSysSound);
	if (tRet != FMOD_OK)
	{
		(*ppSysSound)->release();
		(*ppSysSound) = nullptr;
		return nullptr;
	}

	_pSound = pSound;

	return pSound;
}

void SoundManager::CleanUp()
{
	if (_pSound)
	{
		delete _pSound;
		_pSound = nullptr;
	}
	if (_pSystem)
	{
		_pSystem->close();
		_pSystem->release();
		_pSystem = nullptr;
	}
}

/*
===========
Sound
===========
*/

USound::USound()
{
}

USound::~USound()
{
	CleanUp();
}

AkBool USound::Initialize(FMOD::System* pSys)
{
	_pSystem = pSys;

	return AK_TRUE;
}

void USound::Update()
{
}

void USound::Render()
{
}

AkBool USound::Play(AkBool bLoop)
{
	if (_pChannel)
	{
		_pChannel->isPlaying((bool*)&_bPlaying);
	}
	if (!_bPlaying)
	{
		FMOD_RESULT eRet = _pSystem->playSound(_pSound, nullptr, AK_FALSE, &_pChannel);
		if (eRet == FMOD_OK)
		{
			if (bLoop)
			{
				_pChannel->setMode(FMOD_LOOP_NORMAL);
			}
			else
			{
				_pChannel->setMode(FMOD_LOOP_OFF);
			}
		}
		return false;
	}
	return true;
}

void USound::PlayOnce()
{
	FMOD::Channel* pChannel = nullptr;
	FMOD_RESULT eRet = _pSystem->playSound(_pSound, nullptr, AK_FALSE, &pChannel);
}

void USound::Stop()
{
}

void USound::Pause()
{
}

void USound::VolumeUp()
{
}

void USound::VolumeDown()
{
}

void USound::CleanUp()
{
	if (_pSound)
	{
		_pSound->release();
		_pSound = nullptr;
	}
}

void USound::VolumeControl()
{
}
