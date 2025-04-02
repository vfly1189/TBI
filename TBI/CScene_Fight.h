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
    StatType type;          // 스탯 종류
    float currentValue;     // 현재 값
    float* prevValue;       // 이전 값 포인터
    float multiplier;       // 표시 배율 (예: 0.01)
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
    unordered_map<StatType, CSpriteUI*> m_mapStatTexts;  // 스탯 텍스트 매핑

    PlayerStat m_stPrevPlayerStat;

public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.

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

