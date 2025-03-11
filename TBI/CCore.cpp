#include "global.h"
#include "CCore.h"
#include "Resource.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

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
	CkeyMgr::GetInstance()->init();
	//////////////////////////Manager initialize//////////////////////////////

	return S_OK;
}

void CCore::progress()
{
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt = { 0,0,(long)_vResolution.x, (long)_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}