#include "global.h"
#include "CObject.h"

#include "CTimeMgr.h"
#include "Direct2DMgr.h"
#include "CPathMgr.h"

#include "CTextUI.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CImage.h"
#include "CRigidBody.h"

CObject::CObject()
	: m_vPos{}
	, m_ObjType(GROUP_TYPE::END)
	, m_vScale{}
	, m_bAlive(true)
	, m_bEnable(true)
	, m_pImages{}
	, m_pTextUI(nullptr)
	, m_pAnimator(nullptr)
{
	m_vRenderScale = m_vScale;
}

CObject::CObject(const CObject& _origin)
	: m_ObjName(_origin.m_ObjName)
	, m_ObjType(_origin.m_ObjType)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_bAlive(true)
	, m_bEnable(true)
	, m_pImages{}
	, m_pTextUI(nullptr)
	, m_pAnimator(nullptr)
{
	m_vRenderScale = m_vScale;
	
	if (_origin.m_pTextUI != nullptr) {
		m_pTextUI = new CTextUI(*_origin.m_pTextUI);
		m_pTextUI->m_pOwner = this;
	}

	if (_origin.m_pAnimator != nullptr) {
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}
}

CObject::~CObject() {
	Safe_Delete_Vec(m_pImages);
	m_pImages.clear();

	if (m_pTextUI != nullptr) delete m_pTextUI;
	if (m_pAnimator != nullptr) delete m_pAnimator;
}

void CObject::update()
{
	m_vPrevPos = m_vPos;
}

void CObject::finalupdate()
{
	if (m_pAnimator) m_pAnimator->finalupdate();
	if (!m_pImages.empty())
	{
		for (size_t i = 0; i < m_pImages.size(); ++i)
		{
			m_pImages[i]->finalupdate();
		}
	}
	if (m_pCollider) m_pCollider->finalupdate();

	if (m_pRigidBody) m_pRigidBody->finalupdate();
}

void CObject::render(ID2D1HwndRenderTarget* _pRender)
{
	if (GetImages().empty()) {
		Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vPos);

		float left = vRenderPos.x - m_vRenderScale.x / 2.f;
		float top = vRenderPos.y - m_vRenderScale.y / 2.f;
		float right = vRenderPos.x + m_vRenderScale.x / 2.f;
		float bottom = vRenderPos.y + m_vRenderScale.y / 2.f;

		D2D1_RECT_F rect = D2D1::RectF(left, top, right, bottom);

		ID2D1SolidColorBrush* pBrush = nullptr;
		HRESULT hr = _pRender->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black), &pBrush);

		if (SUCCEEDED(hr))
		{
			_pRender->DrawRectangle(rect, pBrush);
			pBrush->Release();
		}
	}
	component_render(_pRender);
}


void CObject::component_render(ID2D1HwndRenderTarget* _pRender)
{

	if (m_pAnimator != nullptr)
	{
		m_pAnimator->render(_pRender);
	}

	if (m_pTextUI != nullptr)
		m_pTextUI->render(_pRender);

	if (!m_pImages.empty())
	{
		for (size_t i = 0; i < m_pImages.size(); ++i)
		{
			m_pImages[i]->render(_pRender);
		}
	}

	if (m_pCollider != nullptr)
		m_pCollider->render(_pRender);
}


void CObject::CreateTextUI(
	const std::wstring& _text
	, Vec2 _offsetLT, Vec2 _offsetRB
	, float _fontSize, D2D1::ColorF _colorText
	, bool _bDrawOutline
	, float _fOutlineThickness
	, D2D1::ColorF _colorOutline
	, int _horizontal)
{
	if (!m_pTextUI)
	{
		m_pTextUI = new CTextUI;

		m_pTextUI->SetOwner(this);

		m_pTextUI->SetText(_text);
		m_pTextUI->SetFontSize(_fontSize);
		m_pTextUI->SetTextColor(_colorText);

		m_pTextUI->SetOffsetLT(_offsetLT);
		m_pTextUI->SetOffsetRB(_offsetRB);

		m_pTextUI->SetDrawOutline(_bDrawOutline);
		m_pTextUI->SetOutlineThickness(_fOutlineThickness);
		m_pTextUI->SetOutlineColor(_colorOutline);

		m_pTextUI->SetHorizontal(_horizontal);
	}
}

void CObject::AddImage(ID2D1Bitmap* _bitmap)
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	CImage* tmp = new CImage;
	tmp->SetBitmap(_bitmap);
	tmp->m_pOwner = this;
	m_pImages.push_back(tmp);
}

void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void CObject::CreateRigidBody()
{
	m_pRigidBody = new CRigidBody;
	m_pRigidBody->m_pOwner = this;
}


