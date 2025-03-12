#include "global.h"

#include "CCamera.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"



CCamera::CCamera()
	: m_fTime(0.5f)
	, m_fSpeed(0.f)
	, m_fAccTime(0.5f)
{
}

CCamera::~CCamera() {

}

void CCamera::init()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	//m_pVeilTex = CResMgr::GetInstance()->CreateTexture(L"cameraVeil", (UINT)vResolution.x, (UINT)vResolution.y);

	//m_vMinBounds = Vec2(vResolution.x / 2.f, vResolution.y / 2.f);
	//m_vMaxBounds = Vec2(18.f * TILE_SIZE - vResolution.x / 2.f, 18.f * TILE_SIZE - vResolution.y / 2.f);
}

void CCamera::update()
{
	if (KEY_HOLD(KEY::UP)) {
		m_vLookAt.y -= 500.f * fDT;
	}
	if (KEY_HOLD(KEY::DOWN)) {
		m_vLookAt.y += 500.f * fDT;
	}
	if (KEY_HOLD(KEY::RIGHT)) {
		m_vLookAt.x += 500.f * fDT;
	}
	if (KEY_HOLD(KEY::LEFT)) {
		m_vLookAt.x -= 500.f * fDT;
	}
	//화면 중앙좌표와 카메라 LootAt 좌표간의 차이 값. 
	CalDiff();
	//printf("%.2f , %.2f\n", m_vLookAt.x, m_vLookAt.y);
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