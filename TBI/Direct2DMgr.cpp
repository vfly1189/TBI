#include "global.h"
#include "Direct2DMgr.h"
#include "CPathMgr.h"
#include "CFontMgr.h"

Direct2DMgr::Direct2DMgr()
{

}

Direct2DMgr::~Direct2DMgr()
{
    Cleanup();
}

HRESULT Direct2DMgr::init(HWND hwnd)
{
    HRESULT hr;

    // COM ���̺귯�� �ʱ�ȭ
    hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) return hr;

    // Direct2D ���丮 ����
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    if (FAILED(hr)) return hr;


    // ���� Ÿ�� ����
    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    hr = pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
        &pRenderTarget
    );
    if (FAILED(hr)) return hr;

    // WIC ���丮 ����
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,   // CLSID for WIC Imaging Factory
        nullptr,                   // No aggregation
        CLSCTX_INPROC_SERVER,      // Run in the same process
        IID_PPV_ARGS(&pWICFactory) // Output pointer to IWICImagingFactory
    );
    if (FAILED(hr)) {
        MessageBox(nullptr, L"WIC ���丮 ���� ����!", L"����", MB_OK);
        return hr;
    }

    hr = pRenderTarget->CreateCompatibleRenderTarget(&pBitmapRenderTarget);
    if (FAILED(hr)) return hr;


    return S_OK;

}

HRESULT Direct2DMgr::LoadAndStoreBitmap(const std::wstring& filePath, const std::wstring& tag, bool split) {
    // �̹� �ε�� ��Ʈ���� �ִ��� Ȯ��
    if (bitmapMap.find(tag) != bitmapMap.end()) {
        return S_OK; // �̹� �ε��
    }

    ID2D1Bitmap* newBitmap = nullptr;

    const wstring strFinalPath = CPathMgr::GetInstance()->GetContentPath() + filePath;

    HRESULT hr = LoadBitmap(strFinalPath, &newBitmap);

    if (SUCCEEDED(hr)) {
        bitmapMap[tag] = newBitmap; // �ʿ� ����

        // split�� true�� ��� ��Ʈ���� �ɰ�
        if (split) {
            hr = SplitBitmap(newBitmap, tag);
            if (FAILED(hr)) {
                MessageBox(nullptr, L"��Ʈ�� ���� ����!", L"����", MB_OK);
                return hr;
            }
        }
    }

    return hr;
}

ID2D1Bitmap* Direct2DMgr::GetStoredBitmap(const std::wstring& tag) {
    auto it = bitmapMap.find(tag);

    if (it != bitmapMap.end()) {
        return it->second; // ����� ��Ʈ��bitmapMap ��ȯ
    }

    return nullptr; // ����
}

vector<ID2D1Bitmap*> Direct2DMgr::GetSplitBitmaps(const wstring& tag) {
    auto it = splitBitmapMap.find(tag);

    if (it != splitBitmapMap.end()) {
        return it->second; // ����� ��Ʈ�� ��ȯ
    }

    return {}; // ����
}




HRESULT Direct2DMgr::LoadBitmap(const std::wstring& filePath, ID2D1Bitmap** ppBitmap) {

    // ���� ���� ���� Ȯ��
    if (!pWICFactory) {
        MessageBox(nullptr, L"WIC ���丮�� �ʱ�ȭ���� �ʾҽ��ϴ�!", L"����", MB_OK);
        return E_FAIL;
    }
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pSource = nullptr;
    IWICFormatConverter* pConverter = nullptr;

    HRESULT hr = pWICFactory->CreateDecoderFromFilename(
        filePath.c_str(),            // ���� ���
        nullptr,                     // Ư�� ���޾�ü GUID (NULL ���)
        GENERIC_READ,                // �б� ���� �׼���
        WICDecodeMetadataCacheOnLoad,// ��Ÿ������ ĳ�� �ɼ�
        &pDecoder                    // ���ڴ� ���
    );

    if (FAILED(hr)) {
        MessageBox(nullptr, L"�̹��� ���ڴ� ���� ����!", L"����", MB_OK);
        return hr;
    }

    // ù ��° ������ ��������
    hr = pDecoder->GetFrame(0, &pSource);
    if (FAILED(hr)) {
        MessageBox(nullptr, L"�̹��� ������ �������� ����!", L"����", MB_OK);
        pDecoder->Release();
        return hr;
    }

    // ���� ��ȯ�� ����
    hr = pWICFactory->CreateFormatConverter(&pConverter);
    if (FAILED(hr)) {
        MessageBox(nullptr, L"���� ��ȯ�� ���� ����!", L"����", MB_OK);
        pSource->Release();
        pDecoder->Release();
        return hr;
    }

    // ���� ��ȯ �ʱ�ȭ
    hr = pConverter->Initialize(
        pSource,
        GUID_WICPixelFormat32bppPBGRA, // Direct2D���� ����ϴ� ����
        WICBitmapDitherTypeNone,
        nullptr,
        0.f,
        WICBitmapPaletteTypeMedianCut
    );

    if (FAILED(hr)) {
        MessageBox(nullptr, L"�̹��� ���� ��ȯ ����!", L"����", MB_OK);
        pConverter->Release();
        pSource->Release();
        pDecoder->Release();
        return hr;
    }

    // Direct2D ��Ʈ�� ����
    hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, ppBitmap);

    if (FAILED(hr)) {
        MessageBox(nullptr, L"Direct2D ��Ʈ�� ���� ����!", L"����", MB_OK);
    }

    // �޸� ����
    if (pConverter) pConverter->Release();
    if (pSource) pSource->Release();
    if (pDecoder) pDecoder->Release();

    return hr;
}

HRESULT Direct2DMgr::SplitBitmap(ID2D1Bitmap* bitmap, const wstring& tag) {
    if (!bitmap || !pRenderTarget) return E_FAIL;

    D2D1_SIZE_F bitmapSize = bitmap->GetSize();
    UINT rows = static_cast<UINT>(bitmapSize.height / ((float)TILE_SIZE / 2.f));
    UINT cols = static_cast<UINT>(bitmapSize.width / ((float)TILE_SIZE / 2.f));

    vector<ID2D1Bitmap*> splitBitmaps;

    for (UINT row = 0; row < rows; ++row) {
        for (UINT col = 0; col < cols; ++col) {
            // �� 64x64 ���� ����
            D2D1_RECT_F srcRect = D2D1::RectF(
                col * ((float)TILE_SIZE / 2.f),
                row * ((float)TILE_SIZE / 2.f),
                (col + 1) * ((float)TILE_SIZE / 2.f),
                (row + 1) * ((float)TILE_SIZE / 2.f)
            );

            // ȣȯ ���� Ÿ�� ����
            ID2D1BitmapRenderTarget* compatibleRenderTarget = nullptr;
            HRESULT hr = pRenderTarget->CreateCompatibleRenderTarget(
                D2D1::SizeF(((float)TILE_SIZE / 2.f), ((float)TILE_SIZE / 2.f)),
                &compatibleRenderTarget
            );
            if (FAILED(hr)) {
                MessageBox(nullptr, L"ȣȯ ���� Ÿ�� ���� ����!", L"����", MB_OK);
                return hr;
            }

            // ȣȯ ���� Ÿ�ٿ� ���� ��Ʈ�� �׸���
            compatibleRenderTarget->BeginDraw();
            compatibleRenderTarget->DrawBitmap(bitmap, D2D1::RectF(0, 0, TILE_SIZE / 2, TILE_SIZE / 2), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
            hr = compatibleRenderTarget->EndDraw();
            if (FAILED(hr)) {
                MessageBox(nullptr, L"��Ʈ�� �׸��� ����!", L"����", MB_OK);
                compatibleRenderTarget->Release();
                return hr;
            }

            // ȣȯ ���� Ÿ�ٿ��� ��Ʈ�� ��������
            ID2D1Bitmap* subBitmap = nullptr;
            hr = compatibleRenderTarget->GetBitmap(&subBitmap);
            compatibleRenderTarget->Release(); // ���� Ÿ�� ����
            if (FAILED(hr)) {
                MessageBox(nullptr, L"��Ʈ�� �������� ����!", L"����", MB_OK);
                return hr;
            }

            splitBitmaps.push_back(subBitmap);
        }
    }

    splitBitmapMap[tag] = splitBitmaps;
    return S_OK;
}

HRESULT Direct2DMgr::SplitBitmap(ID2D1Bitmap* bitmap, const wstring& tag,
    const D2D1_POINT_2F& leftTop, const D2D1_POINT_2F& rightBottom)
{
    if (!bitmap || !pRenderTarget) return E_FAIL;

    // �Է� �� ��ȿ�� �˻�
    D2D1_SIZE_F bitmapSize = bitmap->GetSize();
    if (leftTop.x >= rightBottom.x || leftTop.y >= rightBottom.y ||
        rightBottom.x > bitmapSize.width || rightBottom.y > bitmapSize.height)
    {
        MessageBox(nullptr, L"�߸��� ���� ��ǥ �Է�!", L"����", MB_OK);
        return E_INVALIDARG;
    }

    // ���� ���� ���
    const float width = rightBottom.x - leftTop.x;
    const float height = rightBottom.y - leftTop.y;

   
    // ȣȯ ���� Ÿ�� ����
    ID2D1BitmapRenderTarget* compatibleRenderTarget = nullptr;
    HRESULT hr = pRenderTarget->CreateCompatibleRenderTarget(
        D2D1::SizeF(width, height),
        &compatibleRenderTarget
    );

    if (FAILED(hr)) {
        MessageBox(nullptr, L"ȣȯ ���� Ÿ�� ���� ����!", L"����", MB_OK);
        return hr;
    }

    // �������� Ư�� ���� ����
    compatibleRenderTarget->BeginDraw();
    compatibleRenderTarget->DrawBitmap(
        bitmap,
        D2D1::RectF(0, 0, width, height),  // ��� ����(��ü ä��)
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        D2D1::RectF(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y)  // ���� ����
    );

    hr = compatibleRenderTarget->EndDraw();
    if (FAILED(hr)) {
        MessageBox(nullptr, L"��Ʈ�� ���� ����!", L"����", MB_OK);
        compatibleRenderTarget->Release();
        return hr;
    }

    // ��� ��Ʈ�� ����
    ID2D1Bitmap* subBitmap = nullptr;
    hr = compatibleRenderTarget->GetBitmap(&subBitmap);
    compatibleRenderTarget->Release();

    if (FAILED(hr)) {
        MessageBox(nullptr, L"��Ʈ�� �������� ����!", L"����", MB_OK);
        return hr;
    }

    
    bitmapMap[tag] = subBitmap;
    return S_OK;
}

HRESULT Direct2DMgr::StoreCreateMap(ID2D1Bitmap* bitmap, const wstring& tag)
{
    // �̹� �ε�� ��Ʈ���� �ִٸ� �� �� ����.
    if (bitmapMap.find(tag) != bitmapMap.end()) {
        //return S_OK; // �̹� �ε��
        bitmapMap[tag]->Release();
    }

    bitmapMap[tag] = bitmap; // �ʿ� ����


    return S_OK;
}

void Direct2DMgr::Cleanup() {
    for (auto& pair : bitmapMap) {
        if (pair.second) pair.second->Release();
    }

    bitmapMap.clear();

    if (pRenderTarget) {
        pRenderTarget->Release();
        pRenderTarget = nullptr;
    }

    if (pWICFactory) {
        pWICFactory->Release();
        pWICFactory = nullptr;
    }

    if (pD2DFactory) {
        pD2DFactory->Release();
        pD2DFactory = nullptr;
    }

    CoUninitialize();
}

HRESULT Direct2DMgr::StoreBitmapsFromFolder(const wstring& folderPath, const wstring& tag)
{
    wstring header = CPathMgr::GetInstance()->GetContentPath();
    vector<ID2D1Bitmap*> splitBitmaps;
    for (int i = 0; i < 64; i++)
    {
        wstring finalPath = header + folderPath + L"mapTile_" + std::to_wstring(i) + L".png";
        ID2D1Bitmap* newBitmap = nullptr;

        LoadBitmap(finalPath, &newBitmap);
        splitBitmaps.push_back(newBitmap);
    }

    splitBitmapMap[tag] = splitBitmaps;
    return S_OK;
}

void Direct2DMgr::RenderTextWithOutline(
    const std::wstring& text,
    const D2D1_RECT_F& layoutRect,
    float fontSize,
    const D2D1_COLOR_F& textColor,
    const D2D1_COLOR_F& outlineColor,
    float outlineThickness,
    int horizontal)
{
    if (!pRenderTarget) return;

    IDWriteTextFormat* pTextFormat = CFontMgr::GetInstance()->GetTextFormat(L"HY�߸���", fontSize);

    // �ؽ�Ʈ ���� ����
    if (horizontal == 0)
        pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // ���� ��� ����
    else if(horizontal == 1)
        pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING); // ���� ���� ����
    else if(horizontal == 2)
        pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING); // ���� ���� ����

    pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // ���� ��� ����

    // �ܰ��� �귯�� ����
    ID2D1SolidColorBrush* pOutlineBrush = nullptr;
    HRESULT hr = pRenderTarget->CreateSolidColorBrush(outlineColor, &pOutlineBrush);
    if (FAILED(hr)) {
        pTextFormat->Release();
        return;
    }

    // �ؽ�Ʈ �귯�� ����
    ID2D1SolidColorBrush* pTextBrush = nullptr;
    hr = pRenderTarget->CreateSolidColorBrush(textColor, &pTextBrush);
    if (FAILED(hr)) {
        pOutlineBrush->Release();
        pTextFormat->Release();
        return;
    }

    // �ܰ��� ������ (�ؽ�Ʈ�� �ణ�� �̵���Ű�� �׸���)
    for (float dx = -outlineThickness; dx <= outlineThickness; dx += outlineThickness) {
        for (float dy = -outlineThickness; dy <= outlineThickness; dy += outlineThickness) {
            if (dx == 0 && dy == 0) continue; // �߽����� �ǳʶ�

            D2D1_RECT_F outlineRect = layoutRect;
            outlineRect.left += dx;
            outlineRect.top += dy;
            outlineRect.right += dx;
            outlineRect.bottom += dy;

            pRenderTarget->DrawText(
                text.c_str(),
                static_cast<UINT32>(text.length()),
                pTextFormat,
                &outlineRect,
                pOutlineBrush
            );
        }
    }

    // ���� �ؽ�Ʈ ������
    pRenderTarget->DrawText(
        text.c_str(),
        static_cast<UINT32>(text.length()),
        pTextFormat,
        &layoutRect,
        pTextBrush
    );

    //���ҽ� ����
    pOutlineBrush->Release();
    pTextBrush->Release();
    pTextFormat->Release();
}

