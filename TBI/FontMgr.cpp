#include "global.h"
#include "FontMgr.h"
#include "CPathMgr.h"
#include "Direct2DMgr.h"
#include <fstream>

FontMgr::FontMgr()
{

}

FontMgr::~FontMgr()
{

}

HRESULT FontMgr::LoadFont(const std::wstring& fontTag, const std::wstring& fntFile, const std::wstring& bitmapFile) {
    if (fonts_.find(fontTag) != fonts_.end())
        return S_OK; // ÀÌ¹Ì ·ÎµåµÊ

    FontData fontData{};

    // ºñÆ®¸Ê ·Îµå
    HRESULT hr = Direct2DMgr::GetInstance()->LoadAndStoreBitmap(bitmapFile, fontTag + L"_bitmap", false);
    if (FAILED(hr)) return hr;

    fontData.bitmap = Direct2DMgr::GetInstance()->GetStoredBitmap(fontTag + L"_bitmap");
    if (!fontData.bitmap) return E_FAIL;

    // .fnt ÆÄÀÏ ÆÄ½Ì
    hr = ParseFntFile(fntFile, fontData);
    if (FAILED(hr)) return hr;

    fonts_[fontTag] = fontData;
    return S_OK;
}

HRESULT FontMgr::ParseFntFile(const std::wstring& fntFilePath, FontData& fontData) {
    std::wifstream file(fntFilePath);
    if (!file.is_open()) return E_FAIL;

    std::wstring line;
    while (std::getline(file, line)) {
        if (line.find(L"char id=") != std::wstring::npos) {
            wchar_t id;
            Glyph glyph{};
            swscanf_s(line.c_str(),
                L"char id=%hu x=%f y=%f width=%f height=%f xoffset=%f yoffset=%f xadvance=%f",
                &id,
                &glyph.srcRect.left,
                &glyph.srcRect.top,
                &glyph.srcRect.right,
                &glyph.srcRect.bottom,
                &glyph.xoffset,
                &glyph.yoffset,
                &glyph.xadvance);

            glyph.srcRect.right += glyph.srcRect.left;
            glyph.srcRect.bottom += glyph.srcRect.top;

            fontData.glyphs[id] = glyph;
        }
    }

    file.close();
    return S_OK;
}

void FontMgr::RenderText(const std::wstring& fontTag, const std::wstring& text, float x, float y, const D2D1_COLOR_F& color) {
    auto it = fonts_.find(fontTag);
    if (it == fonts_.end()) return;

    FontData& font = it->second;

    for (auto ch : text) {
        auto git = font.glyphs.find(ch);
        if (git == font.glyphs.end()) continue;

        Glyph& glyph = git->second;

        D2D1_RECT_F destRect = D2D1::RectF(
            x + glyph.xoffset,
            y + glyph.yoffset,
            x + glyph.xoffset + (glyph.srcRect.right - glyph.srcRect.left),
            y + glyph.yoffset + (glyph.srcRect.bottom - glyph.srcRect.top)
        );

        Direct2DMgr::GetInstance()->RenderBitmap(destRect, fontTag + L"_bitmap", &glyph.srcRect, color);
        x += glyph.xadvance;
    }
}

void FontMgr::init()
{
    LoadFont(L"MyFntFont", L"font\\kr_font12.fnt", L"font\\kr_font12_0.png");
}
