#pragma once
#include "CObject.h"

struct tMonInfo
{
    MON_TYPE    m_eMonType;
    float       m_fSpeed;           //�ӵ�. 
    int         m_iMaxHP;           //�ִ�ü��
    int         m_iCurHP;           //ü��
    float       m_fRecogRange;      //���� ����
    float       m_fAttRange;        //���� ����
    float       m_fAtt;             //���ݷ�
};

class AI;
class CellMap;

class CMonster :
    public CObject
{
protected:
    tMonInfo m_tInfo;
    AI* m_pAI;
    Vec2 m_vGridPos;
    CellMap* m_pOwner;

public:
    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }
    const tMonInfo& Getinfo() { return m_tInfo; }
    void SetRecogRange(float _fRecogRange) { m_tInfo.m_fRecogRange = _fRecogRange; }
    void SetAI(AI* _AI);
    void SetGridPos(Vec2 _gridPos) { m_vGridPos = _gridPos; }
    void SetOwner(CellMap* _pOwner) { m_pOwner = _pOwner; }

public:
    Vec2 GetGridPos() { return m_vGridPos; }

public:
    virtual void start() {};
    virtual void update();


public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);

    CLONE(CMonster)

public:
    CMonster();
    ~CMonster();
};

