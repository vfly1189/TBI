#include "global.h"
#include "CTimeMgr.h"
#include "CCore.h"

CTimeMgr::CTimeMgr()
	: m_llCurCount{}
	, m_llPrevCount{}
	, m_llFrequency{}
	, m_dDT(0.)
	, m_iCallCount(0)
	, m_dAcc(0.)
	, m_iFPS(0)
	, m_ftimeScale(1.f)
{

}

CTimeMgr::~CTimeMgr() {

}

void CTimeMgr::init()
{
	//���� ī��Ʈ
	QueryPerformanceCounter(&m_llPrevCount);
	//�ʴ� ī��Ʈ�� �����Ǿ� �ִ°͵� �ƴ�. 

	//�ʴ� ī��Ʈ Ƚ��(1000��)
	QueryPerformanceFrequency(&m_llFrequency);
	SetTimeScale(1.f);

	SetWindowText(CCore::GetInstance()->GetMainHwnd(), L"Brotato");

}

void CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	//���� �������� ī���ð�, ���� ������ ī���� ���� ���̸� ���Ѵ�. 
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;

	//���� ī��Ʈ ���� ���簪���� ����(�������� ����� ���ؼ�)
	m_llPrevCount = m_llCurCount;
}

void CTimeMgr::render()
{
	m_iCallCount++;
	m_dAcc += m_dDT;	//DT ����. 
	if (m_dAcc >= 1.) {
		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DT : %f", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInstance()->GetMainHwnd(), szBuffer);
	}
}
