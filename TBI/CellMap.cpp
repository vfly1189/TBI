#include "global.h"
#include "CellMap.h"

#include "MapMgr.h"
#include "Direct2DMgr.h"
#include "CPlayerMgr.h"
#include "CSoundMgr.h"
#include "CItemMgr.h"

#include "CPlayer.h"
#include "CCollectiblesItem.h"
#include "CWall.h"
#include "CDoor.h"
#include "CRock.h"

#include "CFly_Mon.h"
#include "CHorf_Mon.h"
#include "CBaby_Plum.h"

#include "CSpriteUI.h"

#include "CCollider.h"
#include "CRigidBody.h"

#include "CMonFactory.h"

// Pre-defined constants to avoid recalculation
Vec2 DOOR_POSITIONS[4] = {
    Vec2(0.f, -235.f),
    Vec2(395.f, 0.f),
    Vec2(0.f, 235.f),
    Vec2(-395.f, 0.f)
};

// Wall configurations - position and scale
struct WallConfig {
    Vec2 position;
    Vec2 scale;
};

// Pre-calculated wall configurations when no doors exist
const WallConfig WALL_CONFIGS[4] = {
    { Vec2(0.f, -230.f), Vec2(700.f, 20.f) },    // Up
    { Vec2(375.f, 0.f), Vec2(20.f, 400.f) },     // Right
    { Vec2(0.f, 225.f), Vec2(700.f, 20.f) },     // Down
    { Vec2(-375.f, 0.f), Vec2(20.f, 400.f) }     // Left
};

// Pre-calculated wall configurations when doors exist
const WallConfig WALL_WITH_DOOR_CONFIGS[4][2] = {
    { { Vec2(-195.f, -230.f), Vec2(340.f, 20.f) }, { Vec2(195.f, -230.f), Vec2(340.f, 20.f) } },      // Up
    { { Vec2(375.f, 120.f), Vec2(20.f, 180.f) }, { Vec2(375.f, -120.f), Vec2(20.f, 180.f) } },        // Right
    { { Vec2(-195.f, 225.f), Vec2(340.f, 20.f) }, { Vec2(195.f, 225.f), Vec2(340.f, 20.f) } },        // Down
    { { Vec2(-375.f, 120.f), Vec2(20.f, 180.f) }, { Vec2(-375.f, -120.f), Vec2(20.f, 180.f) } }       // Left
};

// Door collider configurations
struct DoorColliderConfig {
    Vec2 offset;
    Vec2 scale;
};

const DoorColliderConfig DOOR_COLLIDER_CONFIGS[4] = {
    { Vec2(0.f, 0.f), Vec2(22.f, 10.f) * 2.f },  // Up
    { Vec2(0.f, 0.f), Vec2(10.f, 24.f) * 2.f },  // Right
    { Vec2(0.f, 0.f), Vec2(22.f, 10.f) * 2.f },  // Down
    { Vec2(0.f, 0.f), Vec2(10.f, 24.f) * 2.f }   // Left
};

// Door rotation angles
const float DOOR_ROTATIONS[4] = { 0.f, 90.f, 180.f, 270.f };

CellMap::CellMap(wstring _strMapBaseSprite, Vec2 _vRealPos, Vec2 _vGridPos, ROOM_INFO _eRoomType)
    : m_bIsClear(false)
    , m_iMobCount(0)
    , m_iFlyCount(0)
    , m_bFlySoundIsPlayed(false)
    , m_bIsChecked(false)
    , m_vMapPos(_vRealPos)
    , m_vGridMapPos(_vGridPos)
    , m_eRoomType(_eRoomType)
{
    Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
    m_vecDoorExist.resize(4, false);

    // Initialize entities grid for normal rooms
    if (_eRoomType == ROOM_INFO::NORMAL) {
        m_vecEntitiesPos.resize(7);
        for (int i = 0; i < m_vecEntitiesPos.size(); i++)
            m_vecEntitiesPos[i].resize(13);
        MapMgr::GetInstance()->SetEntities(m_vecEntitiesPos);
    }

    // Create the base map sprite
    cellMap = new CSpriteUI;
    cellMap->SetObjType(GROUP_TYPE::TILE);
    cellMap->SetScale(Vec2(234.f, 156.f) * 4.f);
    cellMap->SetPos(_vRealPos);
    cellMap->AddImage(pD2DMgr->GetStoredBitmap(_strMapBaseSprite));

    // Create room structure
    CreateDoors();
    CreateWalls();

    // Create room contents based on room type
    switch (_eRoomType) {
    case ROOM_INFO::TREASURE:
        CreateTreasureItem();
        break;
    case ROOM_INFO::START:
        CreateControlExplain();
        break;
    case ROOM_INFO::NORMAL:
        CreateEntities();
        break;
    case ROOM_INFO::BOSS:
        CreateBoss(_vRealPos);
        break;
    }
}

CellMap::~CellMap()
{
    // Deletion of objects happens elsewhere
}

void CellMap::CreateDoors()
{
    Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
    vector<vector<UINT>>& gridMap = MapMgr::GetInstance()->GetGridMap();

    for (int i = 0; i < 4; i++) {
        int ny = dy4[i] + (int)m_vGridMapPos.y;
        int nx = dx4[i] + (int)m_vGridMapPos.x;

        // Skip if out of bounds or empty neighboring room
        if (ny < 0 || nx < 0 ||
            ny >= (int)MapMgr::GetInstance()->GetMapMaxHeight() ||
            nx >= (int)MapMgr::GetInstance()->GetMapMaxWidth() ||
            gridMap[ny][nx] == (UINT)ROOM_INFO::EMPTY) {
            continue;
        }

        // Door exists in this direction
        m_vecDoorExist[i] = true;

        // Create door
        CDoor* door = new CDoor;
        door->SetObjType(GROUP_TYPE::DOOR);
        door->SetClose();
        door->SetOwner(this);
        door->SetDoorDir(i);
        door->SetPos(DOOR_POSITIONS[i] + m_vMapPos);
        door->SetRotation(DOOR_ROTATIONS[i]);
        door->CreateCollider();

        // Set door collider
        door->GetCollider()->SetOffsetPos(DOOR_COLLIDER_CONFIGS[i].offset);
        door->GetCollider()->SetScale(DOOR_COLLIDER_CONFIGS[i].scale);

        // Set door type and appearance
        UINT neighborRoomType = gridMap[ny][nx];
        ROOM_INFO doorRoomType;

        if (neighborRoomType == (UINT)ROOM_INFO::BOSS || m_eRoomType == ROOM_INFO::BOSS) {
            doorRoomType = ROOM_INFO::BOSS;
            door->AddImage(pD2DMgr->GetStoredBitmap(L"bossroom_door_close_left"));
            door->AddImage(pD2DMgr->GetStoredBitmap(L"bossroom_door_close_right"));
            door->AddImage(pD2DMgr->GetStoredBitmap(L"bossroom_door"));
        }
        else if (neighborRoomType == (UINT)ROOM_INFO::TREASURE || m_eRoomType == ROOM_INFO::TREASURE) {
            doorRoomType = ROOM_INFO::TREASURE;
            door->AddImage(pD2DMgr->GetStoredBitmap(L"treasureroom_door_close_left"));
            door->AddImage(pD2DMgr->GetStoredBitmap(L"treasureroom_door_close_right"));
            door->AddImage(pD2DMgr->GetStoredBitmap(L"treasureroom_door"));
        }
        else {
            doorRoomType = ROOM_INFO::NORMAL;
            door->AddImage(pD2DMgr->GetStoredBitmap(L"normal_door_close_left"));
            door->AddImage(pD2DMgr->GetStoredBitmap(L"normal_door_close_right"));
            door->AddImage(pD2DMgr->GetStoredBitmap(L"normal_door"));
        }

        door->SetRoomType(doorRoomType);
        door->SetScale(Vec2(64.f, 48.f) * 2.f);

        m_vecObjects.push_back(door);
    }
}

void CellMap::CreateWalls()
{
    for (int i = 0; i < 4; i++) {
        if (!m_vecDoorExist[i]) {
            // No door - create solid wall
            CWall* wall = new CWall;
            wall->SetObjType(GROUP_TYPE::WALL);
            wall->SetWallPos(i);
            wall->SetName(L"WALL");
            wall->SetPos(m_vMapPos + WALL_CONFIGS[i].position);
            wall->SetScale(WALL_CONFIGS[i].scale);
            m_vecObjects.push_back(wall);
        }
        else {
            // Door exists - create split walls
            for (int j = 0; j < 2; j++) {
                CWall* wall = new CWall;
                wall->SetObjType(GROUP_TYPE::WALL);
                wall->SetWallPos(i);
                wall->SetPos(m_vMapPos + WALL_WITH_DOOR_CONFIGS[i][j].position);
                wall->SetScale(WALL_WITH_DOOR_CONFIGS[i][j].scale);
                m_vecObjects.push_back(wall);
            }
        }
    }
}

void CellMap::CreateTreasureItem()
{
    int randNum = rand() % 32 + 1;

    wstring item_number;
    if (randNum < 10)
        item_number = L"00" + std::to_wstring(randNum) + L"_";
    else if (randNum < 100)
        item_number = L"0" + std::to_wstring(randNum) + L"_";
    else
        item_number = std::to_wstring(randNum) + L"_";

    wstring item_image_tag = L"collectibles_" + item_number + items_tags[randNum - 1];

    CCollectiblesItem* newItem = new CCollectiblesItem(m_vMapPos, randNum);
    newItem->SetObjType(GROUP_TYPE::ITEM);
    newItem->SetScale(Vec2(100.f, 100.f));
    m_vecObjects.push_back(newItem);
}

void CellMap::CreateControlExplain()
{
    Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
    CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
    int characterIdx = player->GetCharactIdx();

    // Define buttons to create
    struct ButtonInfo {
        wstring name;
        Vec2 slicePos;
        Vec2 sliceSize;
        Vec2 position;
        Vec2 scale;
    };

    // Extract control image
    pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls"), L"control",
        D2D1::Point2F(0.f, 97.f * characterIdx), D2D1::Point2F(325.f, 97.f * (characterIdx + 1)));

    // Create control explanation background
    CSpriteUI* controlExplain = cellMap->AddChild<CSpriteUI>(Vec2(0.f, -20.f));
    controlExplain->SetScale(Vec2(325.f, 97.f) * 2.f);
    controlExplain->AddImage(pD2DMgr->GetStoredBitmap(L"control"));

    // Button definitions
    const std::vector<ButtonInfo> buttons = {
        {L"control_button_w",          Vec2(96.f, 16.f),   Vec2(16.f, 16.f),  Vec2(-280.f, 50.f), Vec2(16.f, 16.f) * 2.f},
        {L"control_button_a",          Vec2(0.f, 0.f),     Vec2(16.f, 16.f),  Vec2(-255.f, 50.f), Vec2(16.f, 16.f) * 2.f},
        {L"control_button_s",          Vec2(32.f, 16.f),   Vec2(16.f, 16.f),  Vec2(-230.f, 50.f), Vec2(16.f, 16.f) * 2.f},
        {L"control_button_d",          Vec2(48.f, 0.f),    Vec2(16.f, 16.f),  Vec2(-205.f, 50.f), Vec2(16.f, 16.f) * 2.f},
        {L"control_button_up_arrow",   Vec2(128.f, 64.f),  Vec2(16.f, 16.f),  Vec2(-115.f, 50.f), Vec2(16.f, 16.f) * 2.f},
        {L"control_button_left_arrow", Vec2(96.f, 64.f),   Vec2(16.f, 16.f),  Vec2(-90.f, 50.f),  Vec2(16.f, 16.f) * 2.f},
        {L"control_button_down_arrow", Vec2(112.f, 64.f),  Vec2(16.f, 16.f),  Vec2(-65.f, 50.f),  Vec2(16.f, 16.f) * 2.f},
        {L"control_button_right_arrow",Vec2(80.f, 64.f),   Vec2(16.f, 16.f),  Vec2(-40.f, 50.f),  Vec2(16.f, 16.f) * 2.f},
        {L"control_button_e",          Vec2(64.f, 0.f),    Vec2(16.f, 16.f),  Vec2(90.f, 50.f),   Vec2(16.f, 16.f) * 2.f},
        {L"control_button_space",      Vec2(0.f, 48.f),    Vec2(32.f, 16.f),  Vec2(260.f, 50.f),  Vec2(32.f, 16.f) * 2.f}
    };

    // Create all buttons
    for (const auto& btn : buttons) {
        pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), btn.name,
            D2D1::Point2F(btn.slicePos.x, btn.slicePos.y),
            D2D1::Point2F(btn.slicePos.x + btn.sliceSize.x, btn.slicePos.y + btn.sliceSize.y));

        CSpriteUI* buttonSprite = cellMap->AddChild<CSpriteUI>(btn.position);
        buttonSprite->SetScale(btn.scale);
        buttonSprite->AddImage(pD2DMgr->GetStoredBitmap(btn.name));
    }
}

void CellMap::CreateEntities()
{
    Vec2 basePos = m_vMapPos + Vec2(-480.f, -270.f);

    for (size_t i = 0; i < 7; i++) {
        for (size_t j = 0; j < 13; j++) {
            Vec2 entityPos = basePos + Vec2(150.f + 54.f * j, 90.f + 61.f * i);

            switch (m_vecEntitiesPos[i][j]) {
            case (UINT)ENTITY_TYPE::ROCK: {
                CRock* rock = new CRock;
                rock->SetName(L"Rock");
                rock->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"rock"));
                rock->SetScale(Vec2(54.f, 61.f));
                rock->SetObjType(GROUP_TYPE::ENTITY);
                rock->SetPos(entityPos);
                rock->GetCollider()->SetOffsetPos(Vec2(0.f, -5.f));
                rock->GetCollider()->SetScale(Vec2(54.f, 61.f));
                m_vecObjects.push_back(rock);
                break;
            }
            case (UINT)ENTITY_TYPE::FLY: {
                CFly_Mon* fly = (CFly_Mon*)CMonFactory::CreateMonster(MON_TYPE::FLY, entityPos, m_vGridMapPos, 1);
                fly->SetOwner(this);
                m_iMobCount++;
                m_iFlyCount++;
                m_vecObjects.push_back(fly);
                break;
            }
            case (UINT)ENTITY_TYPE::HORF: {
                CHorf_Mon* horf = (CHorf_Mon*)CMonFactory::CreateMonster(MON_TYPE::HORF, entityPos, m_vGridMapPos, 1);
                horf->SetOwner(this);
                m_iMobCount++;
                m_vecObjects.push_back(horf);
                break;
            }
            }
        }
    }
}

void CellMap::CreateBoss(Vec2 _vSpawnPos)
{
    CBaby_Plum* babyplum = (CBaby_Plum*)CMonFactory::CreateMonster(MON_TYPE::BABY_PLUM, _vSpawnPos, m_vGridMapPos, 0);
    babyplum->SetOwner(this);
    MapMgr::GetInstance()->SetBoss(babyplum);
    m_iMobCount++;
    m_vecObjects.push_back(babyplum);
}

void CellMap::update()
{
    // Manage fly sounds
    if (m_iFlyCount > 0) {
        if (!m_bFlySoundIsPlayed) {
            CSoundMgr::GetInstance()->Play(L"insect swarm");
            m_bFlySoundIsPlayed = true;
        }
    }
    else if (m_bFlySoundIsPlayed) {
        CSoundMgr::GetInstance()->Stop(L"insect swarm");
        m_bFlySoundIsPlayed = false;
    }

    // Check if room is cleared
    if (m_iMobCount <= 0 && !m_bIsClear) {
        m_bIsClear = true;
        CSoundMgr::GetInstance()->Play(L"door heavy open", 0.5f);
        CItemMgr::GetInstance()->SetActiveItemCharge(1);
    }

    // Check if room has been visited
    Vec2 vCurPos = MapMgr::GetInstance()->GetCurPos();
    if (!m_bIsChecked && vCurPos.x == m_vGridMapPos.x && vCurPos.y == m_vGridMapPos.y) {
        m_bIsChecked = true;
    }
}

void CellMap::finalupdate()
{
    // Empty implementation - maintain for interface compliance
}

void CellMap::render(ID2D1HwndRenderTarget* _pRender)
{
    // Empty implementation - rendering handled elsewhere
}