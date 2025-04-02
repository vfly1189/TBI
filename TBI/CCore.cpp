#include "global.h"
#include "CCore.h"
#include "Resource.h"

#include "CPathMgr.h"
#include "CSoundMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "Direct2DMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "CSceneMgr.h"
#include "CFileMgr.h"
#include "CEventMgr.h"
#include "CFontMgr.h"
#include "CCollisionMgr.h"
#include "CPlayerMgr.h"
#include "MapMgr.h"
#include "CItemMgr.h"

CCore::CCore()
	: m_hWnd(0)
	, m_ptResolution{}
{

}

CCore::~CCore()
{

}


int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;
	
	//해상도에 맞게 윈도우 크기 조정. 
	ChangeWindowSize(Vec2((float)_ptResolution.x, (float)_ptResolution.y), false);

	
	//////////////////////////Manager initialize//////////////////////////////
	CPathMgr::GetInstance()->init();
	CTimeMgr::GetInstance()->init();
	CKeyMgr::GetInstance()->init();
	CSoundMgr::GetInstance()->init();
	
	Direct2DMgr::GetInstance()->init(m_hWnd);
	
	CFileMgr::GetInstance()->init(CPathMgr::GetInstance()->GetContentPath());CCamera::GetInstance()->init();
	
	CFontMgr::GetInstance()->init();
	CItemMgr::GetInstance()->init();
	
	CPlayerMgr::GetInstance()->init();
	MapMgr::GetInstance()->init();
	CSceneMgr::GetInstance()->init();
	CItemMgr::GetInstance()->init();;
	//////////////////////////Manager initialize//////////////////////////////

	return S_OK;
}

void CCore::progress()
{
	// ============
	//	Manager Update
	// ============
	CTimeMgr::GetInstance()->update();
	CKeyMgr::GetInstance()->update();
	CCamera::GetInstance()->update();	
	CSoundMgr::GetInstance()->update();


	CSceneMgr::GetInstance()->update();
	//UI 이벤트 체크
	// 충돌 체크. 
	CCollisionMgr::GetInstance()->update();

	CUIMgr::GetInstance()->update();

	ID2D1HwndRenderTarget* pRenderTarget = Direct2DMgr::GetInstance()->GetRenderTarget();
	//한 프레임당 그림 그리기 시작. 
	pRenderTarget->BeginDraw();
	Clear();

	//Direct2D 버전 렌더링
	CSceneMgr::GetInstance()->render(pRenderTarget);

	//한 프레임 렌더링 끝. 
	HRESULT hr = pRenderTarget->EndDraw();
	if (FAILED(hr)) {
		MessageBox(nullptr, L"렌더링 실패!", L"오류", MB_OK);
	}

	CTimeMgr::GetInstance()->render();

	CEventMgr::GetInstance()->update();
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt = { 0,0,(long)_vResolution.x, (long)_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}

void CCore::Clear()
{
	//Direct2D방식
	ID2D1HwndRenderTarget* pRenderTarget = Direct2DMgr::GetInstance()->GetRenderTarget();
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}


void CCore::CreateBrushPen()
{
	// Hollow Brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//R,G,B Pen
	m_arrPen[(UINT)PEN_TYPE::RED] = (HPEN)CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}