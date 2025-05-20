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
    int m_iCurPage;         //0 : Ÿ��Ʋ, 1 : �޴� , 2 : ĳ���� ����

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
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

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

