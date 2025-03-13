#pragma once
#include "global.h"
#include "CCamera.h"

class CTextUI;
class CImage;
class CAnimator;

class CObject
{
private:
	GROUP_TYPE		m_ObjType;

	wstring			m_ObjName;
	Vec2			m_vPos;
	Vec2			m_vPrevPos;
	Vec2			m_vScale;
	Vec2			m_vRenderScale;

	//컴포넌트
	CAnimator*		m_pAnimator;
	CTextUI*		m_pTextUI;
	vector<CImage*> m_pImages;

	bool			m_bAlive;			//자기 자신이 활성화 or 비활성화. (삭제 전용)
	bool			m_bEnable;			//일시적인 활성화 or 비활성화. 

public:
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) {
		m_vScale = _vScale;
		m_vRenderScale = _vScale;
	}
	void SetObjType(GROUP_TYPE _eType) { m_ObjType = _eType; }
	void SetName(const wstring& _strName) { m_ObjName = _strName; }
	void SetRenderScale(Vec2 _vScale) { m_vRenderScale = _vScale; }
	
public:

	GROUP_TYPE GetObjType() { return m_ObjType; }
	Vec2 GetPos() { return m_vPos; }

	virtual Vec2 GetFinalPos() { return m_vPos; }

	Vec2 GetScale() { return m_vScale; }
	Vec2 GetRenderScale() { return m_vRenderScale; }

	const wstring& GetName() { return m_ObjName; }

public:
	bool IsDead() {
		return !m_bAlive;
	}

public:
	virtual void start() {};
	virtual void update();
	virtual void finalupdate();

	virtual void render(ID2D1HwndRenderTarget* _pRender);
	virtual void component_render(ID2D1HwndRenderTarget* _pRender);

	virtual CObject* Clone() = 0;
public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

public:
	void CreateTextUI(
		const std::wstring& _text
		, Vec2 _offsetLT, Vec2 _offsetRB
		, float _fontSize, D2D1::ColorF _colorText
		, bool _bDrawOutline
		, float _fOutlineThickness
		, D2D1::ColorF _colorOutline
		, int _horizontal);
	CTextUI* GetTextUI() { return m_pTextUI; }

public:
	vector<CImage*>& GetImages() { return m_pImages; }
	size_t GetImageCount() { return m_pImages.size(); }
	void AddImage(ID2D1Bitmap* _bitmap);

public:
	void CreateAnimator();
	CAnimator* GetAnimator() { return m_pAnimator; }

private:
	void SetDead() { m_bAlive = false; }

};