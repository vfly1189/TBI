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
	//화면 중앙좌표와 카메라 LootAt 좌표간의 차이 값. 
	CalDiff();
}

void CCamera::CalDiff()
{
	//이전 LookAt과 현재 Look의 차이값을 보정해서 현재의 LookAt을 구한다.
	//방향 벡터.

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

