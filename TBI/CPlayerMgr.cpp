#include "global.h"

#include "CPlayer.h"
#include "CObject.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CRigidBody.h"

#include "Direct2DMgr.h"
#include "CPlayerMgr.h"

// Move constant data to a separate file or namespace
namespace PlayerConstants 
{
    const std::vector<std::wstring> CHARACTER_ACCESSORIES_FILES = {
        L"",
        L"character_002_maggiesbeautifulgoldenlocks",
        L"character_003_cainseyepatch",
        L"character_004_judasfez"
    };

    const std::vector<Vec2> CHARACTER_ACCESSORIES_SLICE_SIZES = {
        Vec2(64.f, 64.f),
        Vec2(32.f, 32.f),
        Vec2(32.f, 64.f)
    };

    // Animation names as constants for consistency
    const std::wstring IDLE_ANIM = L"idle";
    const std::wstring FRONT_IDLE_ANIM = L"front_idle";
    const std::wstring ACCESSORIES_FRONT_IDLE_ANIM = L"accessories_front_idle";

    // Direction labels
    enum Direction {
        BACK = 0,
        RIGHT = 1,
        FRONT = 2,
        LEFT = 3
    };

    // Preset offset for head animations
    const Vec2 HEAD_OFFSET = Vec2(0.f, -22.f);
}

CPlayerMgr::CPlayerMgr() 
    : m_Player(nullptr)
    , m_bIsAlive(false)
    , pD2DMgr(nullptr) 
{

}

CPlayerMgr::~CPlayerMgr() 
{
    /*
    // Ensure proper cleanup of allocated resources
    if (m_Player) {
        delete m_Player;
        m_Player = nullptr;
    }*/
}

void CPlayerMgr::init() 
{
    m_bIsAlive = true;
    pD2DMgr = Direct2DMgr::GetInstance();
    CuttingTearImages();
}

void CPlayerMgr::CreateAndSettingPlayer()
{
    m_Player = new CPlayer;
    m_Player->CreateAnimator();
    m_Player->CreateRigidBody();
    m_bIsAlive = true;
}

void CPlayerMgr::SettingImageAndAnimations(int _characterIdx) 
{
    if (_characterIdx < 0 || _characterIdx >= 4) {
        // Handle invalid character index
        return;
    }

    //캐릭터 애니메이션 sprite 파일 이름
    std::wstring oriSprite = L"character_00" + std::to_wstring(_characterIdx + 1) + L"_" + character_names[_characterIdx];

    // Prepare all images
    CuttingAnimationImages(oriSprite, _characterIdx);
    CuttingAdditionalPoseImages(oriSprite, _characterIdx);

    // Create animations
    CreateWalkingAnimation();
    CreateAdditionalPoseAnimation();

    //아이작이 아닌 다른 캐릭터일 경우 악세사리
    if (_characterIdx != 0) {
        CuttingCharacterAccessoriesAnimationImages(_characterIdx);
        CreateAccessoriesAnimation(_characterIdx);
    }

    //default 애니메이션 재생
    auto animator = m_Player->GetAnimator();
    animator->Play(PlayerConstants::IDLE_ANIM, true, 1);
    animator->Play(PlayerConstants::FRONT_IDLE_ANIM, true, 2);

    if (_characterIdx != 0) {
        m_Player->m_sCurAccessoriesAnim = PlayerConstants::ACCESSORIES_FRONT_IDLE_ANIM;
        animator->Play(PlayerConstants::ACCESSORIES_FRONT_IDLE_ANIM, true, 3);
    }
}

void CPlayerMgr::CuttingAnimationImages(const std::wstring& _oriSprite, int _characterIdx) 
{
    // Create a helper method to reduce repeated code
    auto createSlicedBitmap = [this](const std::wstring& source, const std::wstring& targetName,
        D2D1_POINT_2F topLeft, D2D1_POINT_2F bottomRight) {
            pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(source), targetName, topLeft, bottomRight);
            return pD2DMgr->GetStoredBitmap(targetName);
        };

    //움직임 머리, 몸
    createSlicedBitmap(_oriSprite, L"player_animation_sprite",
        D2D1::Point2F(0.f, 0.f), D2D1::Point2F(256.f, 128.f));

    //idle 상태
    createSlicedBitmap(L"player_animation_sprite", L"player_idle",
        D2D1::Point2F(0.f, 32.f), D2D1::Point2F(32.f, 64.f));
    idle = pD2DMgr->GetStoredBitmap(L"player_idle");

    //정면 걷기
    createSlicedBitmap(L"player_animation_sprite", L"player_idle_walk_1",
        D2D1::Point2F(0.f, 32.f), D2D1::Point2F(256.f, 64.f));
    createSlicedBitmap(L"player_animation_sprite", L"player_idle_walk_2",
        D2D1::Point2F(192.f, 0.f), D2D1::Point2F(256.f, 32.f));

    std::vector<ID2D1Bitmap*> walking = {
        pD2DMgr->GetStoredBitmap(L"player_idle_walk_1"),
        pD2DMgr->GetStoredBitmap(L"player_idle_walk_2")
    };
    walking_sprite = CombineBitmapsX(walking);

    //오른쪽걷기
    createSlicedBitmap(L"player_animation_sprite", L"player_right_walk_1",
        D2D1::Point2F(0.f, 64.f), D2D1::Point2F(256.f, 96.f));
    createSlicedBitmap(L"player_animation_sprite", L"player_right_walk_2",
        D2D1::Point2F(0.f, 96.f), D2D1::Point2F(64.f, 128.f));

    std::vector<ID2D1Bitmap*> right_walking = {
        pD2DMgr->GetStoredBitmap(L"player_right_walk_1"),
        pD2DMgr->GetStoredBitmap(L"player_right_walk_2")
    };
    right_walking_sprite = CombineBitmapsX(right_walking);

    //정면idle 머리 , 정면 공격 머리
    createSlicedBitmap(L"player_animation_sprite", L"player_idle_head",
        D2D1::Point2F(0.f, 0.f), D2D1::Point2F(32.f, 32.f));
    createSlicedBitmap(L"player_animation_sprite", L"player_front_attack_head",
        D2D1::Point2F(32.f, 0.f), D2D1::Point2F(64.f, 32.f));

    std::vector<ID2D1Bitmap*> front_attack = {
        pD2DMgr->GetStoredBitmap(L"player_idle_head"),
        pD2DMgr->GetStoredBitmap(L"player_front_attack_head")
    };
    front_attack_sprite = CombineBitmapsX(front_attack);

    //오른쪽 공격 머리
    createSlicedBitmap(L"player_animation_sprite", L"player_right_head",
        D2D1::Point2F(64.f, 0.f), D2D1::Point2F(96.f, 32.f));
    createSlicedBitmap(L"player_animation_sprite", L"player_right_attack_head",
        D2D1::Point2F(96.f, 0.f), D2D1::Point2F(128.f, 32.f));

    std::vector<ID2D1Bitmap*> right_attack = {
        pD2DMgr->GetStoredBitmap(L"player_right_head"),
        pD2DMgr->GetStoredBitmap(L"player_right_attack_head")
    };
    right_attack_sprite = CombineBitmapsX(right_attack);

    //후방 공격 머리
    createSlicedBitmap(L"player_animation_sprite", L"player_back_head",
        D2D1::Point2F(128.f, 0.f), D2D1::Point2F(160.f, 32.f));
    createSlicedBitmap(L"player_animation_sprite", L"player_back_attack_head",
        D2D1::Point2F(160.f, 0.f), D2D1::Point2F(196.f, 32.f));

    std::vector<ID2D1Bitmap*> back_attack = {
        pD2DMgr->GetStoredBitmap(L"player_back_head"),
        pD2DMgr->GetStoredBitmap(L"player_back_attack_head")
    };
    back_attack_sprite = CombineBitmapsX(back_attack);

    //아이템 집었을때
    createSlicedBitmap(_oriSprite, L"player_item_get_animation_sprite",
        D2D1::Point2F(256.f, 0.f), D2D1::Point2F(512.f, 128.f));

    // Handle character-specific head position for item animations
    D2D1_POINT_2F itemGetHeadTopLeft = D2D1::Point2F(272.f, 144.f);
    D2D1_POINT_2F itemGetHeadBottomRight = D2D1::Point2F(272.f + 32.f, 144.f + 32.f);
    createSlicedBitmap(_oriSprite, L"player_item_get_head", itemGetHeadTopLeft, itemGetHeadBottomRight);

    // Item get idle animation
    createSlicedBitmap(L"player_item_get_animation_sprite", L"player_item_get_idle",
        D2D1::Point2F(0.f, 32.f), D2D1::Point2F(32.f, 64.f));

    // Item get front walking
    createSlicedBitmap(L"player_item_get_animation_sprite", L"player_item_get_front_walking_1",
        D2D1::Point2F(0.f, 32.f), D2D1::Point2F(256.f, 64.f));
    createSlicedBitmap(L"player_item_get_animation_sprite", L"player_item_get_front_walking_2",
        D2D1::Point2F(192.f, 0.f), D2D1::Point2F(256.f, 32.f));

    walking.clear();
    walking = {
        pD2DMgr->GetStoredBitmap(L"player_item_get_front_walking_1"),
        pD2DMgr->GetStoredBitmap(L"player_item_get_front_walking_2")
    };
    item_get_front_walking_sprite = CombineBitmapsX(walking);

    // Item get right walking
    createSlicedBitmap(L"player_item_get_animation_sprite", L"player_item_get_right_walking_1",
        D2D1::Point2F(0.f, 64.f), D2D1::Point2F(256.f, 96.f));
    createSlicedBitmap(L"player_item_get_animation_sprite", L"player_item_get_right_walking_2",
        D2D1::Point2F(0.f, 96.f), D2D1::Point2F(64.f, 128.f));

    right_walking.clear();
    right_walking = {
        pD2DMgr->GetStoredBitmap(L"player_item_get_right_walking_1"),
        pD2DMgr->GetStoredBitmap(L"player_item_get_right_walking_2")
    };
    item_get_right_walking_sprite = CombineBitmapsX(right_walking);

    // Hit animation
    createSlicedBitmap(L"character_001_isaac", L"player_hit_sprite",
        D2D1::Point2F(0.f, 128.f), D2D1::Point2F(256.f, 128.f + 192.f));
    createSlicedBitmap(L"player_hit_sprite", L"isaac_hit",
        D2D1::Point2F(128.f, 64.f), D2D1::Point2F(192.f, 128.f));
}

void CPlayerMgr::CuttingCharacterAccessoriesAnimationImages(int _characterIdx) {
    if (_characterIdx < 1 || _characterIdx > 3) {
        return; // Invalid character index
    }

    const Vec2& vSliceSize = PlayerConstants::CHARACTER_ACCESSORIES_SLICE_SIZES[_characterIdx - 1];
    const std::wstring& accessoriesFile = PlayerConstants::CHARACTER_ACCESSORIES_FILES[_characterIdx];

    // Create directional accessories sprites
    struct AccessorySlice {
        std::wstring name;
        float xOffset;
    };

    const AccessorySlice slices[] = {
        {L"accessories_back", 4.f},
        {L"accessories_right", 2.f},
        {L"accessories_front", 0.f},
        {L"accessories_left", 6.f}
    };

    for (const auto& slice : slices) {
        pD2DMgr->SplitBitmap(
            pD2DMgr->GetStoredBitmap(accessoriesFile),
            slice.name,
            D2D1::Point2F(vSliceSize.x * slice.xOffset, 0.f),
            D2D1::Point2F(vSliceSize.x * (slice.xOffset + 2.f), vSliceSize.y)
        );
    }
}

void CPlayerMgr::CuttingTearImages() {
    // Normal tear animation
    pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"tears_multidimensional"), L"player_normal_tear",
        D2D1::Point2F(0.f, 0.f), D2D1::Point2F(256.f, 64.f));

    // Generate tear frames
    m_vTears.clear();
    int num = 0;
    for (int i = 0; i < 2; i++) {
        int max = (i == 0) ? 8 : 5;
        for (int j = 0; j < max; j++) {
            std::wstring tearName = L"tear_" + std::to_wstring(num);
            pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_normal_tear"), tearName,
                D2D1::Point2F(32.f * j, 32.f * i),
                D2D1::Point2F(32.f * (j + 1), 32.f * (i + 1)));
            m_vTears.push_back(pD2DMgr->GetStoredBitmap(tearName));
            num++;
        }
    }

    // Tear explosion animation
    pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"bulletatlas"), L"player_normal_tear_explode",
        D2D1::Point2F(256.f, 256.f), D2D1::Point2F(512.f, 512.f));

    // Process tear explosion frames
    std::vector<ID2D1Bitmap*> vTearExplode;
    for (int i = 1; i <= 4; i++) {
        std::wstring frameName = L"tear_explode_" + std::to_wstring(i);
        pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_normal_tear_explode"), frameName,
            D2D1::Point2F(0.f, 64.f * (i - 1)),
            D2D1::Point2F(256.f, 64.f * i));
        vTearExplode.push_back(pD2DMgr->GetStoredBitmap(frameName));
    }
    tear_explode_sprite = CombineBitmapsX(vTearExplode);

    // Store tear explosion animation
    pD2DMgr->DeleteBitmap(L"tear_explode");
    pD2DMgr->StoreCreateMap(tear_explode_sprite, L"tear_explode");
}

void CPlayerMgr::CuttingAdditionalPoseImages(const std::wstring& _oriSprite, int _characterIdx) {
    // Create additional pose sprite
    pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(_oriSprite), L"player_additional_pose",
        D2D1::Point2F(0.f, 128.f), D2D1::Point2F(256.f, 128.f + 192.f));

    // Hit animation
    pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_additional_pose"), L"player_hit",
        D2D1::Point2F(128.f, 64.f), D2D1::Point2F(192.f, 128.f));

    // Death animation
    pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_additional_pose"), L"player_die_1",
        D2D1::Point2F(0.f, 0.f), D2D1::Point2F(64.f, 64.f));
    pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_additional_pose"), L"player_die_2",
        D2D1::Point2F(192.f, 0.f), D2D1::Point2F(256.f, 64.f));

    // Combine death animation frames
    std::vector<ID2D1Bitmap*> dieFrames = {
        pD2DMgr->GetStoredBitmap(L"player_die_1"),
        pD2DMgr->GetStoredBitmap(L"player_die_2")
    };

    pD2DMgr->DeleteBitmap(L"player_die");
    pD2DMgr->StoreCreateMap(CombineBitmapsX(dieFrames), L"player_die");
}

void CPlayerMgr::PlayerHit(float _dmg) {
    if (m_Player) {
        m_Player->AddCurHp(_dmg);
    }
}

void CPlayerMgr::PlayerHeal(float _value)
{
    if (m_Player) {
        m_Player->AddCurHp(_value);

        if (m_Player->GetPlayerStat().m_fCurHp > m_Player->GetPlayerStat().m_fMaxHp)
        {
            m_Player->GetPlayerStat().m_fCurHp = m_Player->GetPlayerStat().m_fMaxHp;
        }
    }
}

void CPlayerMgr::CreateWalkingAnimation() {
    if (!m_Player || !m_Player->GetAnimator()) {
        return;
    }

    auto animator = m_Player->GetAnimator();
    const float attackAnimSpeed = m_Player->GetPlayerStat().m_fAttackSpd / 2.f;

    // Basic animation setup with common parameters
    auto createBasicAnim = [this, animator](const std::wstring& name, ID2D1Bitmap* sprite,
        const Vec2& offset = Vec2(0.f, 0.f), bool loop = true) {
            animator->CreateAnimation(name, sprite, Vec2(0.f, 0.f), Vec2(32.f, 32.f),
                Vec2(0.f, 0.f), 1.f, 1, loop, offset);
        };

    // Walking animation setup
    auto createWalkingAnim = [this, animator](const std::wstring& name, ID2D1Bitmap* sprite,
        const Vec2& sliceStep, float speed = 0.09f,
        const Vec2& offset = Vec2(0.f, 0.f)) {
            animator->CreateAnimation(name, sprite, Vec2(0.f, 0.f), Vec2(32.f, 32.f),
                sliceStep, speed, 10, true, offset);
        };

    // Basic idle animation
    createBasicAnim(L"idle", walking_sprite);

    // Walking animations
    createWalkingAnim(L"idle_walking", walking_sprite, Vec2(32.f, 0.f));
    createWalkingAnim(L"right_walking", right_walking_sprite, Vec2(32.f, 0.f));

    // Left walking (flipped right walking)
    left_walking_sprite = FlipBitmap(right_walking_sprite, true, false);
    animator->CreateAnimation(L"left_walking", left_walking_sprite, Vec2(288.f, 0.f), Vec2(32.f, 32.f),
        Vec2(-32.f, 0.f), 0.09f, 10, true, Vec2(0.f, 0.f));

    // Head animations
    createBasicAnim(L"front_idle", front_attack_sprite, PlayerConstants::HEAD_OFFSET);
    createBasicAnim(L"right_idle", right_attack_sprite, PlayerConstants::HEAD_OFFSET);

    // Left idle head (flipped)
    left_attack_sprite = FlipBitmap(right_attack_sprite, true, false);
    animator->CreateAnimation(L"left_idle", left_attack_sprite, Vec2(32.f, 0.f), Vec2(32.f, 32.f),
        Vec2(0.f, 0.f), 1.f, 1, true, PlayerConstants::HEAD_OFFSET);

    // Back idle head
    createBasicAnim(L"back_idle", back_attack_sprite, PlayerConstants::HEAD_OFFSET);

    // Attack animations
    auto createAttackAnim = [this, animator, attackAnimSpeed](const std::wstring& name, ID2D1Bitmap* sprite,
        const Vec2& startPos, const Vec2& step) {
            animator->CreateAnimation(name, sprite, startPos, Vec2(32.f, 32.f), step,
                attackAnimSpeed, 2, true, PlayerConstants::HEAD_OFFSET);
        };

    // Directional attack animations
    createAttackAnim(attack_animation_name[0], back_attack_sprite, Vec2(32.f, 0.f), Vec2(-32.f, 0.f));
    createAttackAnim(attack_animation_name[1], right_attack_sprite, Vec2(32.f, 0.f), Vec2(-32.f, 0.f));
    createAttackAnim(attack_animation_name[2], front_attack_sprite, Vec2(32.f, 0.f), Vec2(-32.f, 0.f));
    createAttackAnim(attack_animation_name[3], left_attack_sprite, Vec2(0.f, 0.f), Vec2(32.f, 0.f));

    // Item holding animations
    createBasicAnim(L"item_get_idle_head", pD2DMgr->GetStoredBitmap(L"player_item_get_head"), PlayerConstants::HEAD_OFFSET);
    createBasicAnim(L"item_get_idle", pD2DMgr->GetStoredBitmap(L"player_item_get_idle"));

    // Item walking animations
    createWalkingAnim(L"item_get_front_walking", item_get_front_walking_sprite, Vec2(32.f, 0.f), 0.1f);
    createWalkingAnim(L"item_get_right_walking", item_get_right_walking_sprite, Vec2(32.f, 0.f), 0.1f);

    // Item left walking (flipped)
    item_get_left_walking_sprite = FlipBitmap(item_get_right_walking_sprite, true, false);
    animator->CreateAnimation(L"item_get_left_walking", item_get_left_walking_sprite, Vec2(288.f, 0.f), Vec2(32.f, 32.f),
        Vec2(-32.f, 0.f), 0.1f, 10, true, Vec2(0.f, 0.f));
}

void CPlayerMgr::CreateAccessoriesAnimation(int _characterIdx) {
    if (_characterIdx < 1 || _characterIdx > 3 || !m_Player || !m_Player->GetAnimator()) {
        return;
    }

    auto animator = m_Player->GetAnimator();
    const float attackAnimSpeed = m_Player->GetPlayerStat().m_fAttackSpd / 2.f;
    const Vec2& sliceSize = PlayerConstants::CHARACTER_ACCESSORIES_SLICE_SIZES[_characterIdx - 1];

    // Animations for different directions
    const std::wstring directions[] = { L"back", L"right", L"front", L"left" };
    const std::wstring bitmaps[] = { L"accessories_back", L"accessories_right", L"accessories_front", L"accessories_left" };

    // Create all directional accessories animations
    for (int i = 0; i < 4; i++) {
        // Idle animation
        std::wstring idleAnimName = L"accessories_" + directions[i] + L"_idle";
        animator->CreateAnimation(idleAnimName, pD2DMgr->GetStoredBitmap(bitmaps[i]),
            Vec2(0.f, 0.f), sliceSize, Vec2(0.f, 0.f),
            1.f, 1, true, PlayerConstants::HEAD_OFFSET);

        // Attack animation
        std::wstring attackAnimName = L"accessories_" + directions[i] + L"_attack";
        animator->CreateAnimation(attackAnimName, pD2DMgr->GetStoredBitmap(bitmaps[i]),
            Vec2(sliceSize.x, 0.f), sliceSize, Vec2(-sliceSize.x, 0.f),
            attackAnimSpeed, 2, true, PlayerConstants::HEAD_OFFSET);
    }
}

void CPlayerMgr::CreateAdditionalPoseAnimation() {
    if (!m_Player || !m_Player->GetAnimator()) {
        return;
    }

    auto animator = m_Player->GetAnimator();

    // Hit animation
    animator->CreateAnimation(L"player_hit", pD2DMgr->GetStoredBitmap(L"player_hit"),
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        0.8f, 1, false, PlayerConstants::HEAD_OFFSET);

    // Death animation
    animator->CreateAnimation(L"player_die", pD2DMgr->GetStoredBitmap(L"player_die"),
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        0.5f, 2, false, PlayerConstants::HEAD_OFFSET);
}