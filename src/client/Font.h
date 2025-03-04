#pragma once

class UFont
{
public:
	UFont(AkF32 fWidth, AkF32 fHeight, const wchar_t* wcFontFamilyName, AkF32 fFontSize);
	~UFont();

	AkBool Initialize(AkF32 fWidth, AkF32 fHeight, const wchar_t* wcFontFamilyName, AkF32 fFontSize);
	void Render(const wchar_t* wcText, AkI32 iPosX, AkI32 iPosY, AkF32 fScaleX = 1.0f, AkF32 fScaleY = 1.0f, const Vector3* pFontColor = nullptr);

private:
	void CleanUp();

private:
	static ISpriteObject* s_pSprite;
	static AkU32 s_uInitRefCount;
	AkU32 _uTextTextureWidth = 0;
	AkU32 _uTextTextureHeight = 0;
	AkU8* _pTextTextureImage = nullptr;
	void* _pTextTextureHandle = nullptr;
	void* _pFontObj = nullptr;
	wchar_t _wcText[_MAX_PATH] = {};
};

