#pragma once

class CFontMgr
{
    SINGLE(CFontMgr)

private:
    IDWriteFactory* m_pDWriteFactory; // DirectWrite 팩토리
    IDWriteTextFormat* m_pTextFormat; // 텍스트 포맷

public:
    HRESULT LoadSystemFont(const std::wstring& fontFamilyName, float fontSize);
    IDWriteTextFormat* GetTextFormat(const std::wstring& fontFamilyName, float fontSize);

public:
    void init();


public:
    CFontMgr(const CFontMgr&) = delete;
    void operator=(const CFontMgr&) = delete;
};
