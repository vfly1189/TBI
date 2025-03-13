#pragma once

#include "define.h"

class CCore
{
	SINGLE(CCore);

private:
	HWND				m_hWnd;				//���� ������ �ڵ�
	POINT				m_ptResolution;		//���� ������ �ػ�

	//���� ����ϴ� GDI Object ��
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

