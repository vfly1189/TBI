#pragma once

struct ScrollArea {
	D2D1_RECT_F viewRect;     // ȭ�鿡 ǥ�õǴ� ����
	D2D1_RECT_F contentRect;  // ��ü ������ ����
	D2D1_POINT_2F scrollPos;  // ���� ��ũ�� ��ġ
};

class Direct2DMgr
{
	SINGLE(Direct2DMgr)

private:
	ID2D1Factory* pD2DFactory;
	IWICImagingFactory* pWICFactory;
	IDWriteFactory* g_pDWriteFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1BitmapRenderTarget* pBitmapRenderTarget; // �� ���ۿ� ��� ���� �߰�


	// ���� ��Ʈ���� �����ϱ� ���� ��
	unordered_map<wstring, ID2D1Bitmap*> bitmapMap;
	// �ɰ��� ��Ʈ�� ����� ��
	unordered_map<wstring, vector<ID2D1Bitmap*>> splitBitmapMap;

public:
	ID2D1HwndRenderTarget* GetRenderTarget() { return pRenderTarget; }

public:
	HRESULT init(HWND hwnd);									//�ʱ�ȭ�Լ�

public:
	HRESULT StoreCreateMap(ID2D1Bitmap* bitmap, const wstring& tag);
	HRESULT LoadAndStoreBitmap(const wstring& filePath, const wstring& tag, bool split);
	ID2D1Bitmap* GetStoredBitmap(const wstring& tag);
	vector<ID2D1Bitmap*> GetSplitBitmaps(const wstring& tag);


public:
	HRESULT StoreBitmapsFromFolder(const std::wstring& folderPath, const std::wstring& tag);

public:
	HRESULT SplitBitmap(ID2D1Bitmap* bitmap, const wstring& tag,
		const D2D1_POINT_2F& leftTop, const D2D1_POINT_2F& rightBottom);

public:
	void Cleanup();

public:
	void RenderTextWithOutline(
		const std::wstring& text,
		const D2D1_RECT_F& layoutRect,
		float fontSize,
		const D2D1_COLOR_F& textColor,
		const D2D1_COLOR_F& outlineColor,
		float outlineThickness,
		int horizontal);


private:
	HRESULT LoadBitmap(const wstring& filePath, ID2D1Bitmap** ppBitmap);
	HRESULT SplitBitmap(ID2D1Bitmap* bitmap, const wstring& tag);

};

