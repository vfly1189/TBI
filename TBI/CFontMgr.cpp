#include "global.h"
#include "CFontMgr.h"


CFontMgr::CFontMgr()
    : m_pDWriteFactory(nullptr)
    , m_pTextFormat(nullptr)
{
    // DirectWrite 팩토리 생성
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
}

CFontMgr::~CFontMgr()
{
    if (m_pTextFormat) m_pTextFormat->Release();
    if (m_pDWriteFactory) m_pDWriteFactory->Release();
}

HRESULT CFontMgr::LoadSystemFont(const std::wstring& fontFamilyName, float fontSize)
{
    if (!m_pDWriteFactory) return E_FAIL;

    // 시스템 폰트 컬렉션 가져오기
    IDWriteFontCollection* pFontCollection = nullptr;
    HRESULT hr = m_pDWriteFactory->GetSystemFontCollection(&pFontCollection);
    if (FAILED(hr)) return hr;

    // 폰트 패밀리 이름 확인
    UINT32 index = 0;
    BOOL exists = FALSE;
    hr = pFontCollection->FindFamilyName(fontFamilyName.c_str(), &index, &exists);

    if (!exists || FAILED(hr)) {
        pFontCollection->Release();
        return E_FAIL; // 폰트가 존재하지 않음
    }

    // TextFormat 생성
    hr = m_pDWriteFactory->CreateTextFormat(
        fontFamilyName.c_str(),  // 시스템 폰트 이름
        pFontCollection,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        L"",                     // 로케일 (예: "en-us")
        &m_pTextFormat
    );

    pFontCollection->Release();

    return hr;
}

IDWriteTextFormat* CFontMgr::GetTextFormat(const std::wstring& fontFamilyName, float fontSize)
{
    IDWriteTextFormat* pTextFormat = nullptr;

    if (!m_pDWriteFactory) return nullptr;

    // 시스템 폰트 컬렉션 가져오기
    IDWriteFontCollection* pFontCollection = nullptr;
    HRESULT hr = m_pDWriteFactory->GetSystemFontCollection(&pFontCollection);
    if (FAILED(hr)) return nullptr;

    // 폰트 패밀리 이름 확인
    UINT32 index = 0;
    BOOL exists = FALSE;
    hr = pFontCollection->FindFamilyName(fontFamilyName.c_str(), &index, &exists);

    hr = m_pDWriteFactory->CreateTextFormat(
        fontFamilyName.c_str(), // 폰트 이름
        pFontCollection,  // 폰트 컬렉션
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        L"",
        &pTextFormat
    );

    return SUCCEEDED(hr) ? pTextFormat : nullptr;
}

void CFontMgr::init()
{
    /*
    // 시스템에 설치된 폰트 로딩 (예: Segoe UI)
    HRESULT hr = LoadSystemFont(L"HY견명조", 24.f);
    if (FAILED(hr)) {
        MessageBox(nullptr, L"Failed to load system font.", L"Error", MB_OK);
    }

    // 텍스트 정렬 설정 (가운데 정렬)
    m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // 가로 가운데 정렬
    m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // 세로 가운데 정렬
    */
}
