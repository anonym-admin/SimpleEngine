#include "pch.h"
#include "Font.h"

ISpriteObject* UFont::s_pSprite;
AkU32 UFont::s_uInitRefCount;

UFont::UFont(AkF32 fWidth, AkF32 fHeight, const wchar_t* wcFontFamilyName, AkF32 fFontSize)
{
	Initialize(fWidth, fHeight, wcFontFamilyName, fFontSize);
}

UFont::~UFont()
{
	CleanUp();
}

AkBool UFont::Initialize(AkF32 fWidth, AkF32 fHeight, const wchar_t* wcFontFamilyName, AkF32 fFontSize)
{
	if (!s_uInitRefCount)
	{
		s_pSprite = RENDERER->CreateSpriteObject();
		s_pSprite->SetDrawBackground(AK_TRUE);
	}

	_uTextTextureWidth = fWidth;
	_uTextTextureHeight = fHeight;
	_pTextTextureImage = (AkU8*)malloc(_uTextTextureWidth * _uTextTextureHeight * 4);
	_pTextTextureHandle = RENDERER->CreateDynamicTexture(_uTextTextureWidth, _uTextTextureHeight);
	memset(_pTextTextureImage, 0, _uTextTextureWidth * _uTextTextureHeight * 4);

	// Create font object.
	_pFontObj = RENDERER->CreateFontObject(wcFontFamilyName, fFontSize);

	s_uInitRefCount++;

	return AK_TRUE;
}

void UFont::Render(const wchar_t* wcText, AkI32 iPosX, AkI32 iPosY, AkF32 fScaleX, AkF32 fScaleY, const Vector3* pFontColor)
{
	AkI32 iTextWidth = 0;
	AkI32 iTextHeight = 0;
	AkU32 uTextLen = (AkU32)wcslen(wcText);

	// 텍스트가 변경된 경우
	if (wcscmp(_wcText, wcText))
	{
		memset(_pTextTextureImage, 0, _uTextTextureWidth * _uTextTextureHeight * 4);
		RENDERER->WriteTextToBitmap(_pTextTextureImage, _uTextTextureWidth, _uTextTextureHeight, _uTextTextureWidth * 4, &iTextWidth, &iTextHeight, _pFontObj, wcText, uTextLen);
		RENDERER->UpdateTextureWidthImage(_pTextTextureHandle, _pTextTextureImage, _uTextTextureWidth, _uTextTextureHeight);
		wcscpy_s(_wcText, wcText);
	}

	RENDERER->RenderSpriteWithTex(s_pSprite, iPosX, iPosY, fScaleX, fScaleY, nullptr, 0.0f, _pTextTextureHandle, pFontColor);
}

void UFont::CleanUp()
{
	AkU32 uRefCount = --s_uInitRefCount;

	if (_pFontObj)
	{
		RENDERER->DestroyFontObject(_pFontObj);
		_pFontObj = nullptr;
	}
	if (_pTextTextureImage)
	{
		free(_pTextTextureImage);
		_pTextTextureImage = nullptr;
	}
	if (_pTextTextureHandle)
	{
		RENDERER->DestroyTexture(_pTextTextureHandle);
		_pTextTextureHandle = nullptr;
	}
	if (!uRefCount)
	{
		s_pSprite->Release();
		s_pSprite = nullptr;
	}
}
