#pragma once

struct Glyph {
    D2D1_RECT_F srcRect;
    float xoffset;
    float yoffset;
    float xadvance;
};

class FontMgr {
    SINGLE(FontMgr);

public:
    HRESULT LoadFont(const std::wstring& fontTag, const std::wstring& fntFile, const std::wstring& bitmapFile);
    void RenderText(const std::wstring& fontTag, const std::wstring& text, float x, float y, const D2D1_COLOR_F& color);

public:
    void init();

private:

    struct FontData {
        std::unordered_map<wchar_t, Glyph> glyphs;
        ID2D1Bitmap* bitmap;
    };

    std::unordered_map<std::wstring, FontData> fonts_;

    HRESULT ParseFntFile(const std::wstring& fntFilePath, FontData& fontData);
};
