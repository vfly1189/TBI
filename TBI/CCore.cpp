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
	
	//�ػ󵵿� �°� ������ ũ�� ����. 
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
	//�� �����Ӵ� �׸� �׸��� ����. 
	pRenderTarget->BeginDraw();
	Clear();

	//Direct2D ���� ������
	CSceneMgr::GetInstance()->render(pRenderTarget);

	//�� ������ ������ ��. 
	HRESULT hr = pRenderTarget->EndDraw();
	if (FAILED(hr)) {
		MessageBox(nullptr, L"������ ����!", L"����", MB_OK);
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
	//Direct2D���
	ID2D1HwndRenderTarget* pRenderTarget = Direct2DMgr::GetInstance()->GetRenderTarget();
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}