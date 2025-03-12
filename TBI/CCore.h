#pragma once

#include "define.h"

class CCore
{
	SINGLE(CCore);

private:
	HWND				m_hWnd;				//���� ������ �ڵ�
	POINT				m_ptResolution;		//���� ������ �ػ�

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);

public:
	void Clear();

public:
	HWND GetMainHwnd() { return m_hWnd; }
	POINT GetResolution() { return m_ptResolution; }
};

