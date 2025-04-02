#include "global.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CDoor.h"
#include "CTear.h"
#include "CSpriteUI.h"
#include "CCollectiblesItem.h"
#include "CItem.h"
#include "CScene.h"

#include "CImage.h"

#include "CSceneMgr.h"
#include "CTimeMgr.h"
#include "Direct2DMgr.h"
#include "CCamera.h"
#include "CKeyMgr.h"
#include "CSoundMgr.h"

CPlayer::CPlayer()
    : m_iBodyDir(2)
    , m_iHeadDir(2)
    , m_eCurHeadState(PLAYER_STATE::IDLE)
    , m_eCurBodyState(PLAYER_STATE::IDLE)
    , m_ePrevHeadState(PLAYER_STATE::IDLE)
    , m_ePrevBodyState(PLAYER_STATE::IDLE)
    , m_sCurBodyAnim(L"idle")
    , m_sCurHeadAnim(L"front_idle")
{
    CreateCollider();
    GetCollider()->SetOffsetPos(Vec2(0.f, -5.f));
    GetCollider()->SetScale(Vec2(32.f, 45.f));

    
}

CPlayer::~CPlayer()
{

}

void CPlayer::ResetAnimationLayers()
{
    if (GetAnimator())
    {
        // 아이템 획득 시 사용했던 레이어 초기화
        GetAnimator()->PauseAnimation(m_sCurBodyAnim); // body는 레이어 2
        GetAnimator()->PauseAnimation(m_sCurHeadAnim); // head는 레이어 1

        if (m_sCharacter.compare(L"isaac") != 0)
        {
            printf("정지 애니메 : %ls\n", m_sCurAccessoriesAnim.c_str());
            GetAnimator()->PauseAnimation(m_sCurAccessoriesAnim);
        }
    }
}

void CPlayer::ObtainItem(Item _obtainItem)
{
    printf("아이템 번호 : %d", _obtainItem.m_iNumber);

    m_stPlayerStat.m_iMaxHp += _obtainItem.m_iAddMaxHp;
    m_stPlayerStat.m_fAttackDmg += _obtainItem.m_fAddAttackDmg;
    m_stPlayerStat.m_fAttackDmgCoef += _obtainItem.m_fAddAttackDmgCoef;
    m_stPlayerStat.m_fAttackRange += _obtainItem.m_fAddAttackRange;

    m_stPlayerStat.m_fAttackSpd -= _obtainItem.m_fAddAttackSpd;
    if (m_stPlayerStat.m_fAttackSpd <= 0.1f)
        m_stPlayerStat.m_fAttackSpd = 0.1f;

    m_stPlayerStat.m_fMoveSpd += _obtainItem.m_fAddMoveSpd;
    if (_obtainItem.m_fAddMoveSpd != 0.f)
      GetRigidBody()->SetMaxVelocity(GetRigidBody()->GetMaxVelocity() + Vec2(_obtainItem.m_fAddMoveSpd, _obtainItem.m_fAddMoveSpd));

    wstring item_image_tag = L"collectibles_";
    if (_obtainItem.m_iNumber < 10)
        item_image_tag += L"00" + std::to_wstring(_obtainItem.m_iNumber);
    else if (_obtainItem.m_iNumber < 100)
        item_image_tag += L"0" + std::to_wstring(_obtainItem.m_iNumber);
    else
        item_image_tag += std::to_wstring(_obtainItem.m_iNumber);

    item_image_tag += L"_" + _obtainItem.m_sItemTag;

    m_obtainItem = new CSpriteUI;
    m_obtainItem->SetObjType(GROUP_TYPE::ITEM);
    m_obtainItem->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(item_image_tag));

    m_obtainItem->CreateAnimator();
    m_obtainItem->GetAnimator()->CreateAnimation(L"item_get_effect_starflash", Direct2DMgr::GetInstance()->GetStoredBitmap(L"item_get_effect")
        , Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 0.083f, 12, false, Vec2(0.f, 0.f));
    m_obtainItem->GetAnimator()->Play(L"item_get_effect_starflash", false, 1);

    m_obtainItem->SetScale(Vec2(32.f, 32.f) * 2.f);

    CSceneMgr::GetInstance()->GetCurScene()->AddObject(m_obtainItem, GROUP_TYPE::ITEM);
    CSoundMgr::GetInstance()->Play(L"power up1", 0.2f);
}

void CPlayer::update()
{
    m_fAccTimeTear += fDT;
  
    if (m_obtainItem != nullptr)
    {
        m_obtainItem->SetPos(GetPos() + Vec2(0.f,-80.f));
    }

 
	CObject::update();

	update_move();
    
    update_body_state();
    update_body_animation();

    update_head_state();
    update_head_animation();

    update_arrowKey();

    if (KEY_TAP(KEY::L))
    {
        m_stPlayerStat.m_fAttackSpd -= 0.1f;
        if (m_stPlayerStat.m_fAttackSpd <= 0.1f)
        {
            printf("공속제한!!!\n");
            m_stPlayerStat.m_fAttackSpd = 0.1f;
        }
    }

    if (KEY_TAP(KEY::K))
    {
        printf("현재 hp : %d\n", m_stPlayerStat.m_iCurHp);
        printf("최대 hp : %d\n", m_stPlayerStat.m_iMaxHp);

        printf("현재 공격력 : %f\n", m_stPlayerStat.m_fAttackDmg);
        printf("현재 공격력 계수 : %f\n", m_stPlayerStat.m_fAttackDmgCoef);
        printf("현재 공격속도 : %f\n", m_stPlayerStat.m_fAttackSpd);
        printf("현재 이동속도 : %f\n", m_stPlayerStat.m_fMoveSpd);
        printf("현재 사거리 : %f\n", m_stPlayerStat.m_fAttackRange);
    }

    if (KEY_TAP(KEY::J))
    {
        printf("%f %f\n", GetPos().x, GetPos().y);
    }

    player_sfx();
    CreateTear();

    if (m_fItemGetTimer > 0)
    {
        m_fItemGetTimer -= fDT;
        if (m_fItemGetTimer <= 0)
        {
            m_bGettingItem = false;
            
            if (m_obtainItem != nullptr)
                DeleteObject((CObject*)m_obtainItem);

            // 1. 레이어 정리
            ResetAnimationLayers();

            // 2. 상태 강제 업데이트
            m_ePrevBodyState = PLAYER_STATE::GET_ITEM; // 의도적인 불일치 생성
            m_ePrevHeadState = PLAYER_STATE::GET_ITEM;

            // 3. 애니메이션 이름 재설정
            m_sCurBodyAnim.clear();
            m_sCurHeadAnim.clear();
            m_sCurAccessoriesAnim.clear();

            m_eCurBodyState = PLAYER_STATE::IDLE;
            m_eCurHeadState = PLAYER_STATE::IDLE;
        }
    }
}

void CPlayer::finalupdate()
{
	CObject::finalupdate();
}

void CPlayer::render(ID2D1HwndRenderTarget* _pRender)
{
	//CObject::render(_pRender);

    component_render(_pRender);
}

void CPlayer::update_move()
{
	Vec2 vPos = GetPos();

    /*
	if (KEY_HOLD(KEY::W)) {
		vPos.y -= 200.f * fDT;
	}
	if (KEY_HOLD(KEY::S)) {
		vPos.y += 200.f * fDT;
	}
	if (KEY_HOLD(KEY::A)) {
		vPos.x -= 200.f * fDT;
	}
	if (KEY_HOLD(KEY::D)) {
		vPos.x += 200.f * fDT;
	}*/

    CRigidBody* pRigid = GetRigidBody();
 
    m_vMoveDir = Vec2(0.f, 0.f); // 방향 벡터 초기화

    if (KEY_HOLD(KEY::W)) m_vMoveDir.y -= 1.f;
    if (KEY_HOLD(KEY::S)) m_vMoveDir.y += 1.f;
    if (KEY_HOLD(KEY::A)) m_vMoveDir.x -= 1.f;
    if (KEY_HOLD(KEY::D)) m_vMoveDir.x += 1.f;

    // 방향 벡터 정규화 및 속도 계산
    if (!m_vMoveDir.IsZero()) {
        m_vMoveDir.Normalize();
        Vec2 moveVec = m_vMoveDir * m_stPlayerStat.m_fMoveSpd; // 최종 속도

        //printf("속도 : %f %f\n", m_vMoveDir.x, m_vMoveDir.y);
        pRigid->SetVelocity(moveVec);
    }

    //printf("속도 : %f %f\n", m_vMoveDir.x, m_vMoveDir.y);
	SetPos(vPos);
}



void CPlayer::player_sfx()
{
    if (m_fItemGetTimer > 0) return;

    bool arrowUp = KEY_HOLD(KEY::UP) || KEY_TAP(KEY::UP);
    bool arrowRight = KEY_HOLD(KEY::RIGHT) || KEY_TAP(KEY::RIGHT);
    bool arrowLeft = KEY_HOLD(KEY::LEFT) || KEY_TAP(KEY::LEFT);
    bool arrowDown = KEY_HOLD(KEY::DOWN) || KEY_TAP(KEY::DOWN);

    if ((arrowUp || arrowRight || arrowLeft || arrowDown) && m_fAccTimeTear >= m_stPlayerStat.m_fAttackSpd)
    {
        int randomTearSound = rand() % 2 + 1;

        wstring randomTearSoundString = L"tear fire " + std::to_wstring(randomTearSound);

        printf("sound : %ls\n", randomTearSoundString.c_str());

        CSoundMgr::GetInstance()->Play(randomTearSoundString, 0.2f);
        //m_fAccTimeTear = 0.f;
    }
}

void CPlayer::CreateTear()
{
    if (m_fItemGetTimer > 0) return;

    bool arrowUp = KEY_HOLD(KEY::UP) || KEY_TAP(KEY::UP);
    bool arrowRight = KEY_HOLD(KEY::RIGHT) || KEY_TAP(KEY::RIGHT);
    bool arrowLeft = KEY_HOLD(KEY::LEFT) || KEY_TAP(KEY::LEFT);
    bool arrowDown = KEY_HOLD(KEY::DOWN) || KEY_TAP(KEY::DOWN);

    if (!m_PressedDirectionKeys.empty() && m_fAccTimeTear >= m_stPlayerStat.m_fAttackSpd)
    {
        Vec2 vTearPos = GetPos();
        vTearPos.y -= GetScale().y / 2.f;

        //미사일
        CTear* pTear = new CTear(GetPos());
        pTear->SetName(L"Missile_Player");
        pTear->SetPos(vTearPos);
        pTear->SetScale(Vec2(32.f, 32.f) * 2.f);

        // 입력 순서 중 첫 번째 키 선택
        KEY firstKey = m_PressedDirectionKeys.front();
        Vec2 dir;

        switch (firstKey) {
        case KEY::UP:
            dir = Vec2(0.f, -1.f) + Vec2(m_vMoveDir.x, 0.f) * 0.2f;
            break;
        case KEY::RIGHT:
            dir = Vec2(1.f, 0.f) + Vec2(0.f, m_vMoveDir.y) * 0.2f;
            break;
        case KEY::LEFT:
            dir = Vec2(-1.f, 0.f) + Vec2(0.f, m_vMoveDir.y) * 0.2f;
            break;
        case KEY::DOWN:
            dir = Vec2(0.f, 1.f) + Vec2(m_vMoveDir.x, 0.f) * 0.2f;
            break;
        }

        pTear->SetDir(dir);
        CreateObject(pTear, GROUP_TYPE::TEAR);
        m_fAccTimeTear = 0.f;
    }
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
    if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::DOOR)
    {
        Vec2 vPos = CCamera::GetInstance()->GetLookAt();
        int door_dir = dynamic_cast<CDoor*>(_pOther->GetOwner())->GetDoorDir();

        if      (door_dir == 0) SetPos(vPos - Vec2(0.f,540.f) + Vec2(0.f, 200.f));
        else if (door_dir == 1) SetPos(vPos + Vec2(960.f, 0.f) - Vec2(350.f, 0.f));
        else if (door_dir == 2) SetPos(vPos + Vec2(0.f, 540.f) - Vec2(0.f, 200.f));
        else if (door_dir == 3) SetPos(vPos - Vec2(960.f, 0.f) + Vec2(350.f, 0.f));
    }
    else if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::ITEM)
    {
        if (m_obtainItem != nullptr) return;

        m_fItemGetTimer = 1.0f; // 1초 타이머 설정
        m_bGettingItem = true;
        m_eCurBodyState = PLAYER_STATE::GET_ITEM;
        m_eCurHeadState = PLAYER_STATE::GET_ITEM;

        Item getItem = dynamic_cast<CCollectiblesItem*>(_pOther->GetOwner())->GetItemInfo();
        ResetAnimationLayers();
        ObtainItem(getItem);
    }
}

void CPlayer::update_arrowKey()
{
    // 키 입력 처리 (업데이트 함수 어딘가에 추가)
    const KEY directionKeys[] = { KEY::UP, KEY::RIGHT, KEY::LEFT, KEY::DOWN };

    for (auto key : directionKeys) {
        if (KEY_TAP(key)) { // 새로 눌린 키만 등록
            m_PressedDirectionKeys.push_back(key);
        }
        if (KEY_AWAY(key)) { // 떼어진 키 제거
            auto it = std::find(m_PressedDirectionKeys.begin(),
                m_PressedDirectionKeys.end(), key);
            if (it != m_PressedDirectionKeys.end()) {
                m_PressedDirectionKeys.erase(it);
            }
        }
    }
}

void CPlayer::OnCollisionExit(CCollider* _pOther)
{
  
}


void CPlayer::update_direction(bool w, bool s, bool a, bool d, int& newDir, bool& keyPressed)
{
    if (w || s) {
        newDir = w ? 0 : 2;
        keyPressed = true;
    }
    else if (a || d) {
        newDir = d ? 1 : 3;
        keyPressed = true;
    }
    else {
        newDir = 2; // 기본 방향 (아래)
        keyPressed = false;
    }
}

void CPlayer::update_body_state()
{
    bool w = KEY_HOLD(KEY::W) || KEY_TAP(KEY::W);
    bool s = KEY_HOLD(KEY::S) || KEY_TAP(KEY::S);
    bool a = KEY_HOLD(KEY::A) || KEY_TAP(KEY::A);
    bool d = KEY_HOLD(KEY::D) || KEY_TAP(KEY::D);

    bool bKeyPressed = false;
    int newBodyDir = m_iBodyDir;

    update_direction(w, s, a, d, newBodyDir, bKeyPressed);

    m_iBodyPrevDir = m_iBodyDir;
    m_iBodyDir = newBodyDir;

    if (m_fItemGetTimer > 0)
        return;
    
    // 상태 업데이트
    m_ePrevBodyState = m_eCurBodyState;
    m_eCurBodyState = bKeyPressed ? PLAYER_STATE::WALK : PLAYER_STATE::IDLE;
}

void CPlayer::update_animation(wstring& currentAnim, const wstring& newAnim, int layer)
{
    if (!GetAnimator()) return;

    // 강제 업데이트 조건 추가
    bool bForceUpdate = m_sCurBodyAnim.empty() || m_sCurHeadAnim.empty();

    if (currentAnim != newAnim || bForceUpdate) {

        GetAnimator()->PauseAnimation(currentAnim); // 명시적 레이어 지정
        currentAnim = newAnim;
        GetAnimator()->Play(currentAnim, true, layer);
    }
}

void CPlayer::update_body_animation()
{
    if (m_fItemGetTimer > 0)
    {
        static const wstring itemAnimations[] = {
          L"item_get_front_walking",
          L"item_get_right_walking",
          L"item_get_front_walking",
          L"item_get_left_walking"
        };
        
        bool w = KEY_HOLD(KEY::W) || KEY_TAP(KEY::W);
        bool s = KEY_HOLD(KEY::S) || KEY_TAP(KEY::S);
        bool a = KEY_HOLD(KEY::A) || KEY_TAP(KEY::A);
        bool d = KEY_HOLD(KEY::D) || KEY_TAP(KEY::D);

        wstring newAnim;
        if (!(w || s || a || d))
            newAnim = L"item_get_idle";
        else
            newAnim = itemAnimations[m_iBodyDir];

        update_animation(m_sCurBodyAnim, newAnim, 2);
        return;
    }

    // 강제 업데이트 트리거
    if (m_sCurBodyAnim.empty() || m_ePrevBodyState == PLAYER_STATE::GET_ITEM)
    {
        m_ePrevBodyState = m_eCurBodyState;
        m_iBodyPrevDir = m_iBodyDir;

        static const wstring dirAnimations[] = { L"idle_walking", L"right_walking", L"idle_walking", L"left_walking" };
        wstring newBodyAnim = (m_eCurBodyState == PLAYER_STATE::IDLE) ? L"idle" : dirAnimations[m_iBodyDir];

        update_animation(m_sCurBodyAnim, newBodyAnim, 1); // 레이어 1로 복귀
        return;
    }


    if (m_ePrevBodyState == m_eCurBodyState && m_iBodyPrevDir == m_iBodyDir)
        return;

    static const wstring dirAnimations[] = { L"idle_walking", L"right_walking", L"idle_walking", L"left_walking" };
    wstring newBodyAnim = (m_eCurBodyState == PLAYER_STATE::IDLE) ? L"idle" : dirAnimations[m_iBodyDir];

    update_animation(m_sCurBodyAnim, newBodyAnim, 1);
}

void CPlayer::update_head_state()
{
    if (m_fItemGetTimer > 0)
        return;

    bool w = KEY_HOLD(KEY::W) || KEY_TAP(KEY::W);
    bool s = KEY_HOLD(KEY::S) || KEY_TAP(KEY::S);
    bool a = KEY_HOLD(KEY::A) || KEY_TAP(KEY::A);
    bool d = KEY_HOLD(KEY::D) || KEY_TAP(KEY::D);

    bool up = KEY_HOLD(KEY::UP) || KEY_TAP(KEY::UP);
    bool right = KEY_HOLD(KEY::RIGHT) || KEY_TAP(KEY::RIGHT);
    bool left = KEY_HOLD(KEY::LEFT) || KEY_TAP(KEY::LEFT);
    bool down = KEY_HOLD(KEY::DOWN) || KEY_TAP(KEY::DOWN);

    bool dummy;
    int newHeadDir = m_iHeadDir;

    // 방향키 우선 적용, 없으면 WASD 방향 적용
    if (up) newHeadDir = 0;
    else if (right) newHeadDir = 1;
    else if (down) newHeadDir = 2;
    else if (left) newHeadDir = 3;
    else update_direction(w, s, a, d, newHeadDir, dummy);

    // 상태 업데이트
    m_iHeadPrevDir = m_iHeadDir;
    m_ePrevHeadState = m_eCurHeadState;
    m_iHeadDir = newHeadDir;
    m_eCurHeadState = (up || right || left || down) ? PLAYER_STATE::ATTACK : PLAYER_STATE::IDLE;
}

void CPlayer::update_head_animation()
{
    if (m_fItemGetTimer > 0)
    {
        update_animation(m_sCurHeadAnim, L"item_get_idle_head", 1);
        return;
    }

    ///공속 설정///
    for (auto& attackAnimations : attack_animation_name)
    {
        GetAnimator()->FindAnimation(attackAnimations)->SetDuration(m_stPlayerStat.m_fAttackSpd / 2.f);

        if (m_sCharacter.compare(L"isaac") != 0)
        {
            wstring accessoriesAnim = L"accessories_" + attackAnimations;
            GetAnimator()->FindAnimation(accessoriesAnim)->SetDuration(m_stPlayerStat.m_fAttackSpd / 2.f);
        }
    }
    ///공속 설정///

    //아이템 얻고 난뒤 강제로//
    if (m_sCurHeadAnim.empty() || m_ePrevHeadState == PLAYER_STATE::GET_ITEM)
    {
        static const wstring idleAnimations[] = { L"back_idle", L"right_idle", L"front_idle", L"left_idle" };
        static const wstring attackAnimations[] = { L"back_attack", L"right_attack", L"front_attack", L"left_attack" };

        wstring newHeadAnim = (m_eCurHeadState == PLAYER_STATE::ATTACK)
            ? attackAnimations[m_iHeadDir]
            : (m_eCurBodyState == PLAYER_STATE::WALK)
            ? idleAnimations[m_iBodyDir]
            : L"front_idle";

        if (m_sCharacter.compare(L"isaac") == 0)
            update_animation(m_sCurHeadAnim, newHeadAnim, 2);
        else
        {
            wstring tmp = (m_eCurHeadState == PLAYER_STATE::ATTACK)
                ? attackAnimations[m_iHeadDir]
                : (m_eCurBodyState == PLAYER_STATE::WALK)
                ? idleAnimations[m_iBodyDir]
                : L"front_idle";
            wstring newAccessoriesAnim = L"accessories_" + tmp;

            update_animation(m_sCurHeadAnim, newHeadAnim, 2);
            update_animation(m_sCurAccessoriesAnim, newAccessoriesAnim, 3);
        }
    }
    //아이템 얻고 난뒤 강제로//

    if (m_ePrevHeadState == m_eCurHeadState && m_iHeadDir == m_iHeadPrevDir)
        return;


    static const wstring idleAnimations[] = { L"back_idle", L"right_idle", L"front_idle", L"left_idle" };
    static const wstring attackAnimations[] = { L"back_attack", L"right_attack", L"front_attack", L"left_attack" };


    wstring newHeadAnim = (m_eCurHeadState == PLAYER_STATE::ATTACK)
        ? attackAnimations[m_iHeadDir]
        : (m_eCurBodyState == PLAYER_STATE::WALK)
        ? idleAnimations[m_iBodyDir]
        : L"front_idle";

    if(m_sCharacter.compare(L"isaac") == 0)
        update_animation(m_sCurHeadAnim, newHeadAnim, 2);
    else
    {
        //wstring newAccessoriesAnim = L"accessories_" + idleAnimations[m_iHeadDir];

        wstring tmp = (m_eCurHeadState == PLAYER_STATE::ATTACK)
            ? attackAnimations[m_iHeadDir]
            : (m_eCurBodyState == PLAYER_STATE::WALK)
            ? idleAnimations[m_iBodyDir]
            : L"front_idle";
        wstring newAccessoriesAnim = L"accessories_" + tmp;

        update_animation(m_sCurHeadAnim, newHeadAnim, 2);
        update_animation(m_sCurAccessoriesAnim, newAccessoriesAnim, 3);
    }
}