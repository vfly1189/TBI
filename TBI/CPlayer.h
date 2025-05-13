#pragma once
#include "CObject.h"
#include "CKeyMgr.h"

enum class PLAYER_STATE {
    IDLE,
    WALK,
    ATTACK,
    HIT,
    GET_ITEM,
    DEAD,
};
/*
typedef struct _PlayerStat
{
    int			m_iCurHp;				//현재 hp
    int			m_iMaxHp;				//최대 hp

    float		m_fAttackDmg;			//공격력
    float		m_fAttackDmgCoef;		//공격력 배수

    float		m_fAttackSpd;			//공격속도

    float		m_fMoveSpd;				//이동속도

    float		m_fAttackRange;			//공격사거리

}PlayerStat;*/

class CSpriteUI;


class CPlayer :
    public CObject
{
private:
    PLAYER_STATE            m_eCurBodyState;
    PLAYER_STATE            m_ePrevBodyState;

    PLAYER_STATE            m_eCurHeadState;
    PLAYER_STATE            m_ePrevHeadState;


    vector<KEY>             m_PressedDirectionKeys;


    int                     m_iHeadDir;
    int                     m_iBodyDir;
    Vec2                    m_vMoveDir;

    int                     m_iHeadPrevDir;
    int                     m_iBodyPrevDir;

    wstring                 m_sCurBodyAnim;
    wstring                 m_sCurHeadAnim;
    wstring                 m_sCurAccessoriesAnim;

    PlayerStat              m_stPlayerStat;
    CSpriteUI*              m_obtainItem;

private:
    wstring                 m_sCharacter;
    int                     m_iCharacterIdx;

private:
    float                   m_fAccTimeTear;
    
private:
    float                   m_fItemGetTimer = 0.f;
    bool                    m_bGettingItem = false;

private:
    float m_fHitAccTime;
    float m_fHitDuration;
    bool  m_fHit;

private:
    float m_fDieAccTime;
    float m_fDieDuration;
    bool m_fDie;

private:
    void ResetAnimationLayers();

public:
    Vec2 GetMoveDir() { return m_vMoveDir; }

public:
    void SetPlayerStat(PlayerStat _stPlayerStat) { m_stPlayerStat = _stPlayerStat; }
    PlayerStat& GetPlayerStat() { return m_stPlayerStat; }
    void SetCharacterName(wstring _characterName) { m_sCharacter = _characterName; }
    wstring GetCharacterName() { return m_sCharacter; }
    void SetCharacterIdx(int _idx) { m_iCharacterIdx = _idx; }
    int GetCharactIdx() { return m_iCharacterIdx; }

    void AddCurHp(float _fCurHp) { m_stPlayerStat.m_fCurHp += _fCurHp; }
    void AddMaxHp(float _fMaxHp) { m_stPlayerStat.m_fMaxHp += _fMaxHp; }

    void AddAttackDmg(float _fAttackDmg) { m_stPlayerStat.m_fAttackDmg += _fAttackDmg; }
    void AddAttackDmgCoef(float _fAttackDmgCoef) { m_stPlayerStat.m_fAttackDmgCoef += _fAttackDmgCoef; }

    void AddAttackSpd(float _fAttackSpd) { m_stPlayerStat.m_fAttackSpd = _fAttackSpd; }
    void AddMoveSpd(float _fMoveSpd) { m_stPlayerStat.m_fMoveSpd += _fMoveSpd; }

    void AddAttackRange(float _fAttackRange) { m_stPlayerStat.m_fAttackRange += _fAttackRange; }

public:
    void ObtainItem(Item _obtainItem);

public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);
 

public:
    void update_move();
    void player_sfx();
    void CreateTear();

public:
    void update_direction(bool w, bool s, bool a, bool d, int& newDir, bool& keyPressed);
    void update_body_state();
    void update_animation(wstring& currentAnim, const wstring& newAnim, int layer);
    void update_body_animation();
    void update_head_state();
    void update_head_animation();
    void update_arrowKey();

    void update_hit();
    void update_die();

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);


public:
    CLONE(CPlayer);

public:
    CPlayer();
    ~CPlayer();

    friend class CPlayerMgr;
};

