#pragma once
#include "CObject.h"

//UI�� ����ȭ �Ǿ� ���� �θ� UI, �ڽ� UI
//������ �� ����ȭ�� �θ� UI�� �־���. �׷��� �����δ� ����� UI�� ��. 


class CUI :
    public CObject
{

private:
    vector<CUI*>        m_vecChildUI;
    CUI*                m_pParentUI;
    UI_TYPE             m_eUIType;


protected:
    bool                m_bCamAffected;     //UI�� ī�޶��� ������ �޴� ����. 
    bool                m_bMouseOn;         //UI���� ���콺�� �ִ���.
    bool                m_bLbtnDown;        //UI�� ���� ��ư�� ���� ���� �ִ���. 

protected:
    Vec2                m_vOffset;
    Vec2                m_vFinalPos;

public:
    void SetOffset(Vec2 _vOffset) { m_vOffset = _vOffset; }
    Vec2 GetOffset() const { return m_vOffset; }

    virtual Vec2 GetFinalPos() override
    {
        if (GetParent())
        {
            return GetParent()->GetFinalPos() + m_vOffset;
        }
        return GetPos() + m_vOffset;
    }

public:
    void SetUIType(UI_TYPE _eType) { m_eUIType = _eType; }
    UI_TYPE GetUIType() { return m_eUIType; }
    //  Vec2 GetFinalPos() { return m_vFinalPos; }
    CUI* GetParent() { return m_pParentUI; }
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }

    void AddChild(CUI* _pUI)
    {
        m_vecChildUI.push_back(_pUI);
        _pUI->m_pParentUI = this;
    }
    template<typename T>
    T* AddChild(Vec2 _vOffset)
    {
        T* pChild = new T;
        pChild->SetOffset(_vOffset);
        pChild->SetParent(this);
        m_vecChildUI.push_back(pChild);
        return pChild;
    }

public:
    void SetParent(CUI* _parent) { m_pParentUI = _parent; }
    const vector<CUI*>& GetChildsUI() { return m_vecChildUI; }

public:
    virtual void update();
    virtual void finalupdate();

    void update_child();
    void finalupdate_child();

    virtual void render(ID2D1HwndRenderTarget* _pRender);
    void render_child(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void MouseOn();             //���콺�� UI���� �ö�� ���� ��. 
    virtual void MouseLbtnDown();       //������ ��.
    virtual void MouseLbtnUp();         //������ ��.
    virtual void MouseLbtnClicked();    //Ŭ��. 
    void MouseOnCheck();

    virtual CUI* Clone() = 0;

public:
    CUI(bool _bcamAff);
    CUI(const CUI& _origin);
    virtual ~CUI();

    friend class CUIMgr;
};

