#include "global.h"
#include "CAnimation.h"
#include "CAnimator.h"

#include "CObject.h"

#include "CTimeMgr.h"
#include "CCamera.h"

CAnimation::CAnimation()
	: m_fAlpha(1.f)
{

}

CAnimation::~CAnimation()
{

}
void CAnimation::Create(ID2D1Bitmap* _bitmap
	, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount, Vec2 _offSet)
{
	m_pBitmap = _bitmap;

	tAnimFrm frm = {};
	for (UINT frameIDX = 0; frameIDX < _iFrameCount; frameIDX++) {

		
		frm.fDuration = _fDuration;
		frm.vLT = _vLT + _vStep * (float)frameIDX;
		frm.vSlice = _vSliceSize;
		frm.vOffset = _offSet;

		m_vecFrm.push_back(frm);
	}
}

CAnimation* CAnimation::Clone()
{
	CAnimation* pClone = new CAnimation();
	pClone->m_pAnimName = this->m_pAnimName;
	pClone->m_vOffSet = this->m_vOffSet;
	pClone->m_pAnimator = this->m_pAnimator;
	pClone->m_pBitmap = this->m_pBitmap;
	pClone->m_vecFrm = this->m_vecFrm;
	pClone->m_iCurFrm = 0; // �ʱ� ������ ����
	pClone->m_fAccTime = 0;
	pClone->m_bRepeat = false; // �� ��� ���� �߰� �ʿ�
	return pClone;
}

void CAnimation::update()
{
	if (m_bFinish || m_bPaused) return;  // pause ������ �� ������Ʈ ����


	// ������ ȿ�� ó��
	if (m_bFlicker) {
		m_fFlickerTimer += fDT * m_fFlickerSpeed;
		m_fAlpha = (sin(m_fFlickerTimer) + 1.0f) * 0.5f; // 0~1 ���� ����
	}

	m_fAccTime += fDT;
	//�������� ��� ���ư��µ�, ���� ���� �� �� ����� ���Ѵٸ�?
	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime) {

		//�Ͻ� �����ؼ� fDT�� 2.344�� ������ Duration �� ���� �ִ°� ���� �ʳ�?
		m_iCurFrm++;
		if (m_vecFrm.size() <= m_iCurFrm) {

			//�����ٴ� �ǹ��� -1
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

	Vec2 vScale = pObj->GetScale();
	
	Vec2 vWaveScale = pObj->GetRenderScale();
	Vec2 vFinalScale = (vWaveScale / vScale);

	vPos += m_vecFrm[m_iCurFrm].vOffset;			// Object Position�� Offset��ŭ �߰� �̵� ��ġ. 
	vPos = CCamera::GetInstance()->GetRenderPos(vPos);

	if (m_pAnimator->GetObj()->GetName().compare(L"PLAYER") == 0)
	{
		//printf("��ǥ : %f %f\n", vPos.x, vPos.y);
	}

	// ������ �簢�� ���
	float destLeft = vPos.x - (m_vecFrm[m_iCurFrm].vSlice.x / 2.f) * vFinalScale.x;
	float destTop = vPos.y - (m_vecFrm[m_iCurFrm].vSlice.y / 2.f) * vFinalScale.y;
	float destRight = vPos.x + (m_vecFrm[m_iCurFrm].vSlice.x / 2.f) * vFinalScale.x;
	float destBottom = vPos.y + (m_vecFrm[m_iCurFrm].vSlice.y / 2.f) * vFinalScale.y;
	
	//float destRight = destLeft + m_vecFrm[m_iCurFrm].vSlice.x;
	//float destBottom = destTop + m_vecFrm[m_iCurFrm].vSlice.y;
	D2D1_RECT_F destRect = D2D1::RectF(destLeft, destTop, destRight, destBottom);

	// �ҽ� �簢�� ��� (�̹��� ������ �߶� ����)
	float srcLeft = m_vecFrm[m_iCurFrm].vLT.x;
	float srcTop = m_vecFrm[m_iCurFrm].vLT.y;
	float srcRight = srcLeft + m_vecFrm[m_iCurFrm].vSlice.x;
	float srcBottom = srcTop + m_vecFrm[m_iCurFrm].vSlice.y;
	D2D1_RECT_F srcRect = D2D1::RectF(srcLeft, srcTop, srcRight, srcBottom);


	_pRender->DrawBitmap(m_pBitmap, destRect, m_fAlpha,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
}
