#include "global.h"

#include "CCamera.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "Direct2DMgr.h"




CCamera::CCamera()
	: m_fTime(0.2f)
	, m_fSpeed(0.f)
	, m_fAccTime(0.5f)
	, m_fScale(2.f)
{
}

CCamera::~CCamera() {
	
}

void CCamera::init()
{

}

void CCamera::update()
{
	//ȭ�� �߾���ǥ�� ī�޶� LootAt ��ǥ���� ���� ��. 
	CalDiff();
}

void CCamera::CalDiff()
{
	//���� LookAt�� ���� Look�� ���̰��� �����ؼ� ������ LookAt�� ���Ѵ�.
	//���� ����.

	m_fAccTime += fDT;
	if (m_fAccTime >= m_fTime) {
		m_vCurLookAt = m_vLookAt;
	}
	else {
		Vec2 vLookDir = m_vLookAt - m_prevLookAt;

		if (!vLookDir.IsZero()) {
			m_vCurLookAt = m_prevLookAt + vLookDir.Normalize() * m_fSpeed * fDT;
		}
	}
	
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_vDiff = m_vCurLookAt - vCenter;

	m_prevLookAt = m_vCurLookAt;
}

