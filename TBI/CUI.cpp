#include "global.h"
#include "CUI.h"

#include "CCamera.h"
#include "CKeyMgr.h"

#include "SelectGDI.h"

CUI::CUI(bool _bCamAff)
	: m_pParentUI(nullptr)
	, m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, m_eUIType(UI_TYPE::DEFAULT)
{
}

CUI::CUI(const CUI& _origin)
	: CObject(_origin)
	, m_pParentUI(nullptr)
	, m_bCamAffected(_origin.m_bCamAffected)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
{
	//���⼭ ���� �����ڸ� ������, �θ𿡼� �⺻�����ڷ� ���� ������.
	//���� ���� �����ڸ� ��������� �־���߸� ��. 
	for (auto& child : _origin.m_vecChildUI) {
		AddChild(child->Clone());
	}
}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}


void CUI::update()
{

	//child ui update
	update_child();
}

void CUI::finalupdate()
{
	CObject::finalupdate();
	m_vFinalPos = GetPos() + m_vOffset;
	//UI�� ���� ��ǥ�� ���Ѵ�.
	if (GetParent()) {
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}

	//UI ���콺 üũ
	MouseOnCheck();

	finalupdate_child();
}



void CUI::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
	render_child(_pRender);
}

void CUI::update_child()
{
	for (auto& child : m_vecChildUI) {
		child->update();
	}
}

void CUI::finalupdate_child()
{
	for (auto& child : m_vecChildUI) {
		child->finalupdate();
	}
}



void CUI::render_child(ID2D1HwndRenderTarget* _pRender)
{
	for (auto& child : m_vecChildUI) {

		child->render(_pRender);
	}
}


void CUI::MouseOn()
{

}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}


void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;
	Vec2 vScale = GetScale();

	if (m_bCamAffected) {
		vMousePos = CCamera::GetInstance()->GetRealPos(vMousePos);
	}

	if (m_vFinalPos.x - vScale.x / 2.f <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + vScale.x / 2.f
		&& m_vFinalPos.y - vScale.y / 2.f <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + vScale.y / 2.f)
	{
		m_bMouseOn = true;
	}
	else {
		m_bMouseOn = false;
	}
}

//������ ���� �� UI�� ������ ��Ŀ���� �ٲ��� ��. -> ĵ���� UI���� ������ ������ �ؾ߸� ��. 