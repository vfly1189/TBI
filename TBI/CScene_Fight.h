#pragma once
#include "CScene.h"
#include "CItemMgr.h"

enum class StatType
{
    Attack,
    Attack_Speed,
    Move_Speed,
    Attack_Range,
    CurHp,
};

enum class PickUpType
{
    Coin,
    Bomb,
    Key,
};

struct StatUpdateInfo {
    StatType type;          // 스탯 종류
    float currentValue;     // 현재 값
    float* prevValue;       // 이전 값 포인터
    float multiplier;       // 표시 배율 (예: 0.01)
};

struct PickUpUpdateInfo {
    PickUpType type;          // 스탯 종류
    int currentValue;     // 현재 값
    int* prevValue;       // 이전 값 포인터
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

private:
    CPanelUI* hpPanel;
    vector<CSpriteUI*> m_vecHearts;

protected:
    CPanelUI* activeItemPanel;
    CSpriteUI* chargeBar;

private:
    CPanelUI* pickUpItemPanel;
    PickUp m_stPrevPickups;
    unordered_map<PickUpType, CSpriteUI*> m_mapPickUpText;  // 스탯 텍스트 매핑

    PlayerStat m_stPrevPlayerStat;

private:
    CPanelUI* miniMapPanel;
    CSpriteUI* miniMapImage;

    Vec2 m_vPrevGridPos;

private:
    CSpriteUI* bossHpBarFrame;
    CSpriteUI* bossHpBar;
    CSpriteUI* bossHpBarText;

private:
    bool m_bGameOver;

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
    void UpdateHpUI();

    void CreateActiveItemUI();
    void UpdateActiveItemUI();

    void CreatePickUpUI();
    void UpdatePickUpUI();

    void CuttingMiniMapImages();
    void CreateMiniMapOriginal();
    void CreateMiniMap();
    void UpdateMiniMap();

    void CreateBossHpBar();
    void UpdateBossHpBar();


public:
    void PlayerDiedScreen();
    void CheckPlayerIsDie();
    bool IsGameOver() { return m_bGameOver; }


public:
    void Setup();

public:
    CScene_Fight();
    ~CScene_Fight();

    friend class CPlayer;
};

