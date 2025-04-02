#pragma once
#include "CScene.h"

enum class StatType
{
    Attack,
    Attack_Speed,
    Move_Speed,
    Attack_Range,
    CurHp,
};

struct StatUpdateInfo {
    StatType type;          // ���� ����
    float currentValue;     // ���� ��
    float* prevValue;       // ���� �� ������
    float multiplier;       // ǥ�� ���� (��: 0.01)
};


class Direct2DMgr;
class CPanelUI;

class CScene_Fight :
    public CScene
{
private:
    Direct2DMgr* pD2DMgr;

private:
    CPanelUI* statPanel;
    unordered_map<StatType, CSpriteUI*> m_mapStatTexts;  // ���� �ؽ�Ʈ ����

    PlayerStat m_stPrevPlayerStat;

public:
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    void CreateStatUI();
    void UpdateStatUI();

    void CreateHpUI();

public:
    CScene_Fight();
    ~CScene_Fight();
};

