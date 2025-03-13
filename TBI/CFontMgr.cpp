#include "global.h"
#include "CFontMgr.h"


CFontMgr::CFontMgr()
    : m_pDWriteFactory(nullptr)
    , m_pTextFormat(nullptr)
{
    // DirectWrite ���丮 ����
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

    // �ý��� ��Ʈ �÷��� ��������
    IDWriteFontCollection* pFontCollection = nullptr;
    HRESULT hr = m_pDWriteFactory->GetSystemFontCollection(&pFontCollection);
    if (FAILED(hr)) return hr;

    // ��Ʈ �йи� �̸� Ȯ��
    UINT32 index = 0;
    BOOL exists = FALSE;
    hr = pFontCollection->FindFamilyName(fontFamilyName.c_str(), &index, &exists);

    if (!exists || FAILED(hr)) {
        pFontCollection->Release();
        return E_FAIL; // ��Ʈ�� �������� ����
    }

    // TextFormat ����
    hr = m_pDWriteFactory->CreateTextFormat(
        fontFamilyName.c_str(),  // �ý��� ��Ʈ �̸�
        pFontCollection,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        L"",                     // ������ (��: "en-us")
        &m_pTextFormat
    );

    pFontCollection->Release();

    return hr;
}

IDWriteTextFormat* CFontMgr::GetTextFormat(const std::wstring& fontFamilyName, float fontSize)
{
    IDWriteTextFormat* pTextFormat = nullptr;

    if (!m_pDWriteFactory) return nullptr;

    // �ý��� ��Ʈ �÷��� ��������
    IDWriteFontCollection* pFontCollection = nullptr;
    HRESULT hr = m_pDWriteFactory->GetSystemFontCollection(&pFontCollection);
    if (FAILED(hr)) return nullptr;

    // ��Ʈ �йи� �̸� Ȯ��
    UINT32 index = 0;
    BOOL exists = FALSE;
    hr = pFontCollection->FindFamilyName(fontFamilyName.c_str(), &index, &exists);

    hr = m_pDWriteFactory->CreateTextFormat(
        fontFamilyName.c_str(), // ��Ʈ �̸�
        pFontCollection,  // ��Ʈ �÷���
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
    // �ý��ۿ� ��ġ�� ��Ʈ �ε� (��: Segoe UI)
    HRESULT hr = LoadSystemFont(L"HY�߸���", 24.f);
    if (FAILED(hr)) {
        MessageBox(nullptr, L"Failed to load system font.", L"Error", MB_OK);
    }

    // �ؽ�Ʈ ���� ���� (��� ����)
    m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // ���� ��� ����
    m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // ���� ��� ����
    */
}
