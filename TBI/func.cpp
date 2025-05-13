#include "global.h"
#include "func.h"

#include "CEventMgr.h"
#include "Direct2DMgr.h"

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent createEvn = {};
	createEvn.eEven = EVENT_TYPE::CREATE_OBJECT;
	createEvn.lParam = (DWORD_PTR)_pObj;
	createEvn.wParam = (DWORD_PTR)_eGroup;

	CEventMgr::GetInstance()->AddEvent(createEvn);
}

void DeleteObject(CObject* _pObj)
{
	tEvent deleteEvn = {};
	deleteEvn.eEven = EVENT_TYPE::DELETE_OBJECT;
	deleteEvn.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInstance()->AddEvent(deleteEvn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent changeEvn = {};
	changeEvn.eEven = EVENT_TYPE::SCENE_CHANGE;
	changeEvn.lParam = (DWORD_PTR)_eNext;

	CEventMgr::GetInstance()->AddEvent(changeEvn);
}

void ChangeAIState(AI* _pAI, MON_STATE _eNextState)
{
	tEvent changeEvn = {};
	changeEvn.eEven = EVENT_TYPE::CHANGE_AI_STATE;
	changeEvn.lParam = (DWORD_PTR)_pAI;
	changeEvn.wParam = (DWORD_PTR)_eNextState;

	CEventMgr::GetInstance()->AddEvent(changeEvn);
}

//0~255���� -> 0~1�� ����ȭ
D2D1::ColorF ColorNormalize(int r, int g, int b)
{
	return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f);
}

ID2D1Bitmap* FlipBitmap(ID2D1Bitmap* original, bool x, bool y)
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	ID2D1Bitmap* result;

	D2D1_SIZE_U size = original->GetPixelSize();
	pD2DMgr->GetRenderTarget()->CreateBitmap(size, nullptr, 0, D2D1::BitmapProperties(original->GetPixelFormat()), &result);
	
	// 2. ���� bitmap�� y�� ��Ī���� �� bitmap�� �׸���
	ID2D1BitmapRenderTarget* RT = nullptr;
	
	pD2DMgr->GetRenderTarget()->CreateCompatibleRenderTarget(D2D1::SizeF(static_cast<float>(size.width), static_cast<float>(size.height)), &RT);
	
	RT->BeginDraw();
	
	D2D1::Matrix3x2F transformX = D2D1::Matrix3x2F::Scale(-1, 1, D2D1::Point2F(size.width / 2.0f, size.height / 2.0f));
	D2D1::Matrix3x2F transformY = D2D1::Matrix3x2F::Scale(1, -1, D2D1::Point2F(size.width / 2.0f, size.height / 2.0f));
	D2D1::Matrix3x2F transformXY = D2D1::Matrix3x2F::Scale(-1, -1, D2D1::Point2F(size.width / 2.0f, size.height / 2.0f));

	if(x && y) RT->SetTransform(transformXY);
	else if(x && !y) RT->SetTransform(transformX);
	else if(!x && y) RT->SetTransform(transformY);
	
	RT->DrawBitmap(original);
	RT->EndDraw();
	RT->GetBitmap(&result);

	return result;
}



ID2D1Bitmap* CombineBitmaps2X2(vector<ID2D1Bitmap*>& bitmaps)
{
	ID2D1Bitmap* originalBitmap = bitmaps[0];
	ID2D1Bitmap* xFlip = bitmaps[1];
	ID2D1Bitmap* yFlip = bitmaps[2];
	ID2D1Bitmap* xyFlip = bitmaps[3];


	// ��ü �ռ� ��Ʈ���� ũ�� ���: 
	// ���� ���� offset���� ������ gridCount * tileDrawSize ��ŭ Ȯ��.
	const UINT compositeWidth = (UINT)(originalBitmap->GetSize().width * 2);
	const UINT compositeHeight = (UINT)(originalBitmap->GetSize().height * 2); // ���簢��

	// 3. ������ũ�� ���� Ÿ��(��Ʈ�� ���� Ÿ��) ����
	ID2D1Bitmap* pCompositeBitmap = nullptr;
	ComPtr<ID2D1BitmapRenderTarget> pBitmapRT = nullptr;
	HRESULT hr = Direct2DMgr::GetInstance()->GetRenderTarget()->CreateCompatibleRenderTarget(
		D2D1::SizeF((FLOAT)compositeWidth, (FLOAT)compositeHeight),
		&pBitmapRT
	);

	pBitmapRT->BeginDraw();
	// ���� (��: ���)���� Ŭ����
	pBitmapRT->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));


	// 4. ù ��° �̹��� �׸��� (�»��)
	D2D1_RECT_F destRectLT = D2D1::RectF(
		0.f,    // left
		0.f,    // top
		originalBitmap->GetSize().width,  // right (���� �̹��� �ʺ�)
		originalBitmap->GetSize().height   // bottom (���� �̹��� ����)
	);
	pBitmapRT->DrawBitmap(
		originalBitmap,
		destRectLT,
		1.0f,   // opacity
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);

	// 5. �� ��° �̹��� �׸��� (����)
	D2D1_RECT_F destRectRT = D2D1::RectF(
		originalBitmap->GetSize().width,  // ���� �̹��� ���� ��ġ
		0.f,
		originalBitmap->GetSize().width * 2.f,  // 112 * 2
		originalBitmap->GetSize().height
	);
	pBitmapRT->DrawBitmap(
		xFlip,
		destRectRT,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);


	// 5. �� ��° �̹��� �׸��� (�����ϴ�)
	D2D1_RECT_F destRectLB = D2D1::RectF(
		0,  // ���� �̹��� ���� ��ġ
		originalBitmap->GetSize().height,
		originalBitmap->GetSize().width,  // 112 * 2
		originalBitmap->GetSize().height * 2.f
	);
	pBitmapRT->DrawBitmap(
		yFlip,
		destRectLB,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);


	// 5. �� ��° �̹��� �׸��� (�����ϴ�)
	D2D1_RECT_F destRectRB = D2D1::RectF(
		originalBitmap->GetSize().width,  // ���� �̹��� ���� ��ġ
		originalBitmap->GetSize().height,
		originalBitmap->GetSize().width * 2.f,  // 112 * 2
		originalBitmap->GetSize().height * 2.f
	);
	pBitmapRT->DrawBitmap(
		xyFlip,
		destRectRB,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);

	// 6. ������ ����
	hr = pBitmapRT->EndDraw();

	pBitmapRT->GetBitmap(&pCompositeBitmap);

	return pCompositeBitmap;
}

ID2D1Bitmap* CombineBitmapsX(vector<ID2D1Bitmap*>& bitmaps)
{
	UINT WidthSum = 0;
	UINT HeightSum = (UINT)bitmaps[0]->GetSize().height;
	for (size_t i = 0; i < bitmaps.size(); i++)
		WidthSum += (UINT)bitmaps[i]->GetSize().width;


	// ��ü �ռ� ��Ʈ���� ũ�� ���: 
	// ���� ���� offset���� ������ gridCount * tileDrawSize ��ŭ Ȯ��.
	const UINT compositeWidth = WidthSum;
	const UINT compositeHeight = HeightSum; // ���簢��

	// 3. ������ũ�� ���� Ÿ��(��Ʈ�� ���� Ÿ��) ����
	ID2D1Bitmap* pCompositeBitmap = nullptr;
	ComPtr<ID2D1BitmapRenderTarget> pBitmapRT = nullptr;
	HRESULT hr = Direct2DMgr::GetInstance()->GetRenderTarget()->CreateCompatibleRenderTarget(
		D2D1::SizeF((FLOAT)compositeWidth, (FLOAT)compositeHeight),
		&pBitmapRT
	);

	pBitmapRT->BeginDraw();
	// ���� (��: ���)���� Ŭ����
	pBitmapRT->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));

	float prevX = 0.f;
	float prevY = 0.f;
	for (size_t i = 0; i < bitmaps.size(); i++)
	{
		// 4. ù ��° �̹��� �׸��� (�»��)
		D2D1_RECT_F destRectLT = D2D1::RectF(
			prevX,    // left
			prevY,    // top
			prevX+bitmaps[i]->GetSize().width,  // right (���� �̹��� �ʺ�)
			prevY+bitmaps[i]->GetSize().height   // bottom (���� �̹��� ����)
		);
		pBitmapRT->DrawBitmap(
			bitmaps[i],
			destRectLT,
			1.0f,   // opacity
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
		);
		prevX += bitmaps[i]->GetSize().width;
		prevY = 0.f;
	}

	// 6. ������ ����
	hr = pBitmapRT->EndDraw();

	pBitmapRT->GetBitmap(&pCompositeBitmap);

	return pCompositeBitmap;
}

float ChargeBarRatio(int _curCharge, int _maxCharge)
{
	vector<vector<int>> pixel =
	{
		{},
		{0,32},
		{0,17,32},
		{0,13,21,32},
		{0,11,17,23,32},
		{0,10,15,20,25,32},
		{0,9,13,17,21,25,32},
		{},
		{0,8,11,14,17,20,23,26,32}
	};

	return (float)pixel[_maxCharge][_curCharge] / 32.f;
}