#pragma once
#include "CScene.h"

class CObject;
class Direct2DMgr;
class CSpriteUI;

class CScene_Main :
    public CScene
{
private:
    Direct2DMgr* pD2DMgr;


private:
    CSpriteUI* Cursor;
    int m_iCursorPos;
    int m_iCurPage;         //0 : 타이틀, 1 : 메뉴 , 2 : 캐릭터 선택

private:
    bool m_bPageControl;

private:
    CSpriteUI* m_CurShowingCharacter;
    int m_iCurCharacterIndex;

private:
    float m_fAccTime;
    bool m_bChangeSceneFlag;

private: 
    CSpriteUI* veil;
    float m_fFadeAlpha;
    float m_fFadeDuration;

public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.

public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    void ChangePage();

public:
    void CreateTitle(Vec2 _vResolution);
    void CreateGameMenu(Vec2 _vResolution);
    void CreateSelectCharacter(Vec2 _vResolution);
    void CreateLoadImage(Vec2 _Resolution);
    void CreateVeil(Vec2 _Resolution);

public:
    void CharacterCutting(int i, int j);
    void CharacterNameCutting();
    void CharacterSelectArrowCutting();

public:
    void MoveCursor(int direction);

public:
    void ChangeCharacter(int direction);
    void UpdateCharacterImage();
    void UpdateCharacterNameTag();


public:
    void HandlePageChange();
    void HandleInputs();
    void HandleSceneTransition();

    void PlayPageChangeSound();
    void UpdateCameraPosition();
    void CheckForSceneChange();

    void InitializePlayerForGameStart();
    void UpdateFadeEffect();

public:
    CScene_Main();
    ~CScene_Main();
};

