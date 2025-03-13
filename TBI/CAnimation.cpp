#include "global.h"
#include "CAnimation.h"
#include "CAnimator.h"

#include "CObject.h"

#include "CTimeMgr.h"
#include "CCamera.h"

CAnimation::CAnimation()
{

}

CAnimation::~CAnimation()
{

}
void CAnimation::Create(ID2D1Bitmap* _bitmap
	, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pBitmap = _bitmap;

	tAnimFrm frm = {};
	for (UINT frameIDX = 0; frameIDX < _iFrameCount; frameIDX++) {
		frm.fDuration = _fDuration;
		frm.vLT = _vLT + _vStep * (float)frameIDX;
		frm.vSlice = _vSliceSize;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation::update()
{
	if (m_bFinish) return;
	m_fAccTime += fDT;
	//프레임이 계속 돌아가는데, 만약 내가 한 번 재생만 원한다면?
	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime) {

		//일시 정지해서 fDT가 2.344면 프레임 Duration 뺀 값을 넣는게 맞지 않나?
		m_iCurFrm++;
		if (m_vecFrm.size() <= m_iCurFrm) {

			//끝났다는 의미의 -1
			m_iCurFrm = -1;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}
		m_fAccTime -= m_vecFrm[m_iCurFrm].fDuration;
	}

}

void CAnimation::finalupdate()
{
	CObject* pOwnerUI = m_pAnimator->GetObj();
	if (pOwnerUI)
	{
		m_vFinalPos = pOwnerUI->GetFinalPos() + m_vOffSet;
	}
	else
	{
		m_vFinalPos = m_pAnimator->GetObj()->GetPos() + m_vOffSet;
	}
}

void CAnimation::render(ID2D1HwndRenderTarget* _pRender)
{
	if (m_bFinish) return;
	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = m_vFinalPos;

	printf("%.f , %.f \n", m_vOffSet.x, m_vOffSet.y);

	Vec2 vScale = pObj->GetScale();
	
	Vec2 vWaveScale = pObj->GetRenderScale();
	Vec2 vFinalScale = vWaveScale / vScale;

	vPos += m_vecFrm[m_iCurFrm].vOffset;			// Object Position에 Offset만큼 추가 이동 위치. 
	//vPos = CCamera::GetInstance()->GetRenderPos(vPos);

	// 목적지 사각형 계산
	float destLeft = vPos.x - (m_vecFrm[m_iCurFrm].vSlice.x / 2.f) * vFinalScale.x;
	float destTop = vPos.y - (m_vecFrm[m_iCurFrm].vSlice.y / 2.f) * vFinalScale.y;
	float destRight = vPos.x + (m_vecFrm[m_iCurFrm].vSlice.x / 2.f) * vFinalScale.x;
	float destBottom = vPos.y + (m_vecFrm[m_iCurFrm].vSlice.y / 2.f) * vFinalScale.y;

	
	//float destRight = destLeft + m_vecFrm[m_iCurFrm].vSlice.x;
	//float destBottom = destTop + m_vecFrm[m_iCurFrm].vSlice.y;
	D2D1_RECT_F destRect = D2D1::RectF(destLeft, destTop, destRight, destBottom);

	// 소스 사각형 계산 (이미지 내에서 잘라낼 영역)
	float srcLeft = m_vecFrm[m_iCurFrm].vLT.x;
	float srcTop = m_vecFrm[m_iCurFrm].vLT.y;
	float srcRight = srcLeft + m_vecFrm[m_iCurFrm].vSlice.x;
	float srcBottom = srcTop + m_vecFrm[m_iCurFrm].vSlice.y;
	D2D1_RECT_F srcRect = D2D1::RectF(srcLeft, srcTop, srcRight, srcBottom);


	_pRender->DrawBitmap(m_pBitmap, destRect, 1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
}
