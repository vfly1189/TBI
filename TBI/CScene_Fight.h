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
    StatType type;          // ���� ����
    float currentValue;     // ���� ��
    float* prevValue;       // ���� �� ������
    float multiplier;       // ǥ�� ���� (��: 0.01)
};

struct PickUpUpdateInfo {
    PickUpType type;          // ���� ����
    int currentValue;     // ���� ��
    int* prevValue;       // ���� �� ������
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

private:
    CPanelUI* hpPanel;
    vector<CSpriteUI*> m_vecHearts;

protected:
    CPanelUI* activeItemPanel;
    CSpriteUI* chargeBar;

private:
    CPanelUI* pickUpItemPanel;
    PickUp m_stPrevPickups;
    unordered_map<PickUpType, CSpriteUI*> m_mapPickUpText;  // ���� �ؽ�Ʈ ����

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

