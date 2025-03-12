#include "global.h"
#include "CCore.h"
#include "Resource.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "Direct2DMgr.h"
#include "CCamera.h"
#include "CSceneMgr.h"

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
	CCamera::GetInstance()->init();
	Direct2DMgr::GetInstance()->init(m_hWnd);
	CSceneMgr::GetInstance()->init();
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


	CSceneMgr::GetInstance()->update();

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