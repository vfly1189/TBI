#pragma once

#include "define.h"

class CCore
{
	SINGLE(CCore);

private:
	HWND				m_hWnd;				//메인 윈도우 핸들
	POINT				m_ptResolution;		//메인 윈도우 해상도

	//자주 사용하는 GDI Object 펜
	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN		m_arrPen[(UINT)PEN_TYPE::END];

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);

public:
	void Clear();

public:
	HWND GetMainHwnd() { return m_hWnd; }
	POINT GetResolution() { return m_ptResolution; }

public:
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }
	void CreateBrushPen();
};

