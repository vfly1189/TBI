#include "global.h"
#include "CImage.h"
#include "CObject.h"
#include "Direct2DMgr.h"
#include "CUI.h"


//CImage����
//�̹��� ������ ���� ��ü��� vOffset���� ���ص���
//������ �̹����� ������ ��ü�� ��������(parent�� �����Ұ��)�� ������� �������� ������ ��ġ�� �����ؾߵ�
//parent�߽� ���� offset�� �����Ұ�.


CImage::CImage()
	: m_pOwner(nullptr)
	, m_pBitmap(nullptr)
	, m_fRatio(1.0f)
	, m_vOffSet(Vec2(0.f, 0.f))
{

}

CImage::~CImage()
{

}


void CImage::finalupdate()
{
	//Vec2 vObjectPos = m_pOwner->GetPos();
	//m_vFinalPos = m_vOffSet + vObjectPos;

	//CImage�� ���ٴ°� �̹����� ���� ��� CSpriteUI�� ����. -> CUI*�� ĳ�����ص� ��������������?
	//�ٸ� Ŭ�������� �̰ɾ���?
	//�׳� CSpriteUI�� image������Ʈ ����� CSpriteUI��ü�� childȭ ��Ű�°� �� ���� �ʳ��ؼ�

	CUI* pOwnerUI = dynamic_cast<CUI*>(m_pOwner);
	if (pOwnerUI)
	{
		m_vFinalPos = pOwnerUI->GetFinalPos() + m_vOffSet;
	}
	else
	{
		m_vFinalPos = m_pOwner->GetPos() + m_vOffSet;
	}
}

void CImage::render(ID2D1HwndRenderTarget* _renderTarget)
{
	/////////////��������////////////////////////
	// 1. �ܼ� GetPos -> ����Ű�� �̵��ϸ� Image�� ������ ������
	// 2. GetRenderPos -> ����Ű�� �̵��ϸ� Image�� Camera�� �°� �̵���
	// 3. GetRealPos -> ????

	Vec2 vPos = m_pOwner->GetFinalPos();
	//m_vFinalPos
	m_pOwner->GetRenderScale();

	
	//�ӽ������� GROUP_TYPE�� UI�ų� DEFAULT�� ��쿡�� RenderPos ���� x -> ȭ��󿡼� ������ ��ġ
	// -> MAINȭ���� �޹��, STARTȭ���� HP,XP,��ȭ ǥ�� UI, (ĳ������,���⼱��)��
	if (m_pOwner->GetObjType() == GROUP_TYPE::UI || m_pOwner->GetObjType() == GROUP_TYPE::DEFAULT)
	{
		//vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	}
	else
		m_vFinalPos = CCamera::GetInstance()->GetRenderPos(m_vFinalPos);
	/////////////��������////////////////////////

	//Vec2 vScale = m_pOwner->GetRenderScale();
	Vec2 vScale;
	if (m_bScaleCustom)
	{
		vScale = m_pOwner->GetRenderScale() * m_vScale;
	}
	else
		vScale = m_pOwner->GetRenderScale();
	

	if (nullptr == m_pBitmap) return;

	float left = m_vFinalPos.x - (vScale.x / 2.f);
	float top = m_vFinalPos.y - (vScale.y / 2.f);
	float right = m_vFinalPos.x + (vScale.x / 2.f);
	float down = m_vFinalPos.y + (vScale.y / 2.f);
	
	D2D1_RECT_F rect = D2D1::RectF(left, top, right, down);

	// 5. HP ������ ���� ä���� �ʺ� ����մϴ�.
	float fillWidth = (right - left) * m_fRatio;

	
	// ä������ �κ��� ������ ����
	D2D1_RECT_F fillRect = D2D1::RectF(
		left,         // ���� X ��ǥ
		top,          // ���� Y ��ǥ
		left + fillWidth, // �� X ��ǥ�� HP ������ ���� ����
		down          // �� Y ��ǥ
	);
	// 6. ���� �̹������� �߶� ������ �����մϴ�.
	D2D1_SIZE_F bitmapSize = m_pBitmap->GetSize();
	D2D1_RECT_F sourceRect = D2D1::RectF(
		0,                           // ���� �̹����� X ������
		0,                           // ���� �̹����� Y ������
		bitmapSize.width * m_fRatio, // ���� �̹����� X ������ ������ ���� ����
		bitmapSize.height            // ���� �̹����� Y ����
	);

	//Weapon�� �ƴ� �� ����Ǵ� ������ �ڵ�. EX)����, ĳ���� ���. 
	D2D1_MATRIX_3X2_F originalMaxrix;
	_renderTarget->GetTransform(&originalMaxrix);

	//_renderTarget->DrawBitmap(m_pBitmap, rect);
	_renderTarget->DrawBitmap(
		m_pBitmap,
		fillRect,                // �������� ��� ���� (HP ���� ����)
		1.0f,                    // ������
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		&sourceRect              // ���� �̹������� �߶� ���� (HP ���� ����)
	);
}

