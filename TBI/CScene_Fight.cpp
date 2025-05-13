#include "global.h"

#include "Direct2DMgr.h"
#include "CCore.h"
#include "MapMgr.h"
#include "CCollisionMgr.h"
#include "CSoundMgr.h"
#include "CPlayerMgr.h"
#include "CKeyMgr.h"
#include "CEventMgr.h"
#include "CItemMgr.h"

#include "CScene_Fight.h"

#include "CAnimation.h"
#include "CAnimator.h"

#include "CDoor.h"
#include "CScene.h"
#include "CWall.h"
#include "CImage.h"
#include "CPanelUI.h"
#include "CPlayer.h"
#include "CSpriteUI.h"
#include "CellMap.h"
#include "CBaby_Plum.h"
#include "CTextUI.h"


void CScene_Fight::Setup()
{
	pD2DMgr = Direct2DMgr::GetInstance();

	CCamera::GetInstance()->SetLookAt(Vec2(0.f, 0.f));

	m_bGameOver = false;

	m_vPrevGridPos = Vec2(0, 0);
}

CScene_Fight::CScene_Fight()
	: m_bGameOver(false)
{

}

CScene_Fight::~CScene_Fight()
{

}

void CScene_Fight::Enter()
{
	printf("SCENE_FIGHT\n");
	
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	
	Setup(); // 초기화

	MapMgr::GetInstance()->MapGenerate();
	MapMgr::GetInstance()->ShowMap();
	
	vector<vector<CellMap*>>& cellMaps = MapMgr::GetInstance()->GetAllCellMaps();
	for(size_t i = 0; i<cellMaps.size(); i++)
	{
		for (size_t j = 0; j < cellMaps[i].size(); j++)
		{
			if (cellMaps[i][j] == nullptr) continue;

			AddObject(cellMaps[i][j]->GetCellMap(), GROUP_TYPE::TILE);
			vector<CObject*>& cellMapObjects = cellMaps[i][j]->GetObjects();

			for (size_t k = 0; k < cellMapObjects.size(); k++)
			{
				GROUP_TYPE groupType = cellMapObjects[k]->GetObjType();

				AddObject(cellMapObjects[k], groupType);
			}
		}
	}
	
	
	CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
	player->SetPos(Vec2(0.f, 0.f));
	player->SetName(L"PLAYER");
	player->SetObjType(GROUP_TYPE::PLAYER);
	player->SetScale(Vec2(32.f, 32.f));
	player->SetRenderScale(player->GetScale() * 2.f);
	AddObject(player, GROUP_TYPE::PLAYER);

	CSpriteUI* test = new CSpriteUI;
	test->SetPos(Vec2(0.f, 0.f));
	test->SetScale(Vec2(1305.f, 100.f) /2.f);
	test->SetObjType(GROUP_TYPE::IMAGE);
	test->CreateAnimator();
	test->GetAnimator()->CreateAnimation(L"dead_explosion", Direct2DMgr::GetInstance()->GetStoredBitmap(L"dead_explosion")
		, Vec2(0.f, 0.f), Vec2(145.f, 100.f), Vec2(145.f, 0.f), 0.1f, 9, true, Vec2(0.f, 0.f));
	test->GetAnimator()->Play(L"dead_explosion", true, 1);
	//test->AddImage(pD2DMgr->GetStoredBitmap(L"dead_explosion"));

	AddObject(test, GROUP_TYPE::IMAGE);
	

	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::ITEM);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DOOR);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::WALL);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::TEAR);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::ENTITY);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::PICKUP_ITEM);

	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::TEAR, GROUP_TYPE::WALL);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::TEAR, GROUP_TYPE::DOOR);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::TEAR, GROUP_TYPE::ENTITY);

	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::ENTITY, GROUP_TYPE::WALL);



	wstring mainTitleBGMKey = L"genesis_retake_light_loop";
	//처음 시작했을 때, BGM틀기. 
	if (!CSoundMgr::GetInstance()->IsPlaySound(mainTitleBGMKey)) {
		CSoundMgr::GetInstance()->Play(mainTitleBGMKey, 0.2f);
	}

	CreateStatUI();
	CreateHpUI();
	CreateActiveItemUI();
	CreatePickUpUI();

	CuttingMiniMapImages();
	CreateMiniMap();

	CreateBossHpBar();

	start();
}

void CScene_Fight::Exit()
{
	DeleteAll();
	MapMgr::GetInstance()->reset();
	//충돌도 전부 초기화 해주기. 
	CCollisionMgr::GetInstance()->Reset();
}

void CScene_Fight::update()
{
	//CScene::update();
	CheckPlayerIsDie();

	if (m_bGameOver)
	{
		if (KEY_TAP(KEY::ESC))
		{
			ChangeScene(SCENE_TYPE::MAIN);
		}
	}

	Vec2 vLookAt = CCamera::GetInstance()->GetLookAt();
	Vec2 vLookAtLT = vLookAt - Vec2(480.f, 270.f);
	Vec2 vLookAtRB = vLookAt + Vec2(480.f, 270.f);

	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++)
	{
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++)
		{
			if (!m_arrObj[typeIDX][objIDX]->IsEnable() ) {
				
				continue;
			}


			if (!m_arrObj[typeIDX][objIDX]->IsDead()) {
				//m_arrObj[typeIDX][objIDX]->update();

				if (typeIDX == (UINT)GROUP_TYPE::TILE ||
					typeIDX == (UINT)GROUP_TYPE::WALL ||
					typeIDX == (UINT)GROUP_TYPE::DOOR ||
					typeIDX == (UINT)GROUP_TYPE::IMAGE) {

					Vec2 vPos = m_arrObj[typeIDX][objIDX]->GetFinalPos();
					if ((vPos.x > vLookAtLT.x && vPos.x < vLookAtRB.x) &&
						(vPos.y > vLookAtLT.y && vPos.y < vLookAtRB.y)) {
						m_arrObj[typeIDX][objIDX]->update();
					}
				}
				else
					m_arrObj[typeIDX][objIDX]->update();
			}
		}
	}
	

	UpdateStatUI();
	UpdateHpUI();
	UpdateActiveItemUI();
	UpdatePickUpUI();
	UpdateMiniMap();
	UpdateBossHpBar();
}

void CScene_Fight::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Fight::render(ID2D1HwndRenderTarget* _pRender)
{
	Vec2 vLookAt = CCamera::GetInstance()->GetLookAt();
	Vec2 vLookAtLT = vLookAt - Vec2(480.f, 270.f);
	Vec2 vLookAtRB = vLookAt + Vec2(480.f, 270.f);

	
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		auto& objVector = m_arrObj[typeIDX]; // Vector 참조를 사용하여 반복 접근 줄임
		auto ObjVecIter = objVector.begin();

		for (; ObjVecIter != objVector.end();) {
			if ((*ObjVecIter)->IsDead()) {
				// Dead 상태일 경우 객체를 삭제
				ObjVecIter = objVector.erase(ObjVecIter);
			}
			else {

				if ((*ObjVecIter)->IsEnable() == false) {
					++ObjVecIter;
					continue;
				}


				// 렌더링 조건 처리
				if (typeIDX == (UINT)GROUP_TYPE::TILE ||
					typeIDX == (UINT)GROUP_TYPE::WALL ||
					typeIDX == (UINT)GROUP_TYPE::DOOR ||
					typeIDX == (UINT)GROUP_TYPE::IMAGE) {

					Vec2 vPos = (*ObjVecIter)->GetFinalPos();
					if ((vPos.x > vLookAtLT.x && vPos.x < vLookAtRB.x) &&
						(vPos.y > vLookAtLT.y && vPos.y < vLookAtRB.y)) {
						(*ObjVecIter)->render(_pRender);
					}
				}
				else {
					(*ObjVecIter)->render(_pRender);
				}
				++ObjVecIter;
			}
		}
	}
	

	Vec2 vCurGridPos = MapMgr::GetInstance()->GetCurPos();	
	CellMap* curCellMap = MapMgr::GetInstance()->GetCellMap(vCurGridPos);
	curCellMap->update();


	//CScene::render(_pRender);
}

void CScene_Fight::CreateStatUI()
{
	CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
	PlayerStat stPlayerStat = player->GetPlayerStat();

	//아이콘 분할 데이터 배열화
	struct IconSplitData {
		LPCWSTR name;
		D2D1_POINT_2F start;
		D2D1_POINT_2F end;
	};

	IconSplitData iconData[] = {
		{L"stat_attack_icon",       {0, 0},   {16, 16}},
		{L"stat_move_speed_icon",   {16, 0},  {32, 16}},
		{L"stat_attack_speed_icon", {32, 0},  {48, 16}},
		{L"stat_attack_range_icon", {0, 16},  {16, 32}}
	};

	// 반복문으로 아이콘 분할 처리
	ID2D1Bitmap* hudBitmap = pD2DMgr->GetStoredBitmap(L"hudstats2");
	for (const auto& data : iconData) {
		pD2DMgr->SplitBitmap(hudBitmap, data.name, data.start, data.end);
	}

	// Stat UI 생성 파라미터 구조체
	struct StatUIParams {
		float yOffset;
		LPCWSTR iconName;
		float statValue;
		bool isFirstPanel = false;
	};

	// Stat UI 데이터 배열
	StatUIParams statParams[] = {
		{-80.f, L"stat_attack_icon",       stPlayerStat.m_fAttackDmg,  true},
		{-40.f, L"stat_attack_speed_icon", stPlayerStat.m_fAttackSpd},
		{  0.f, L"stat_move_speed_icon",   stPlayerStat.m_fMoveSpd * 0.01f},
		{ 40.f, L"stat_attack_range_icon", stPlayerStat.m_fAttackRange * 0.01f}
	};

	// 공통 설정 상수화
	Vec2 panelScale(67.f, 32.f);
	Vec2 iconPos(-25.f, 0.f);
	Vec2 textPos(10.f, 0.f);
	Vec2 iconScale(16.f, 16.f);
	constexpr float textFontSize = 20.f;

	// 메인 패널 생성
	statPanel = new CPanelUI;
	statPanel->SetObjType(GROUP_TYPE::UI);
	statPanel->SetRadius(10.f, 10.f);
	statPanel->SetScale(Vec2(67.f, 200.f));
	statPanel->SetPos(Vec2(40.f, 300.f));
	statPanel->SetName(L"STAT_UI_ROOT");
	statPanel->SetMouseOnAlpha(0.f);
	statPanel->SetNormalAlpha(0.f);
	//statPanel->SetColor(ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255));
	AddObject(statPanel, GROUP_TYPE::UI);

	// Stat 항목 생성 루프
	int paramIndex = 0;
	for (const auto& param : statParams) {
		CPanelUI* statItem = statPanel->AddChild<CPanelUI>(Vec2(0.f, param.yOffset));
		statItem->SetScale(panelScale);
		statItem->SetMouseOnAlpha(0.f);
		statItem->SetNormalAlpha(0.f);

		// 아이콘 생성
		CSpriteUI* icon = statItem->AddChild<CSpriteUI>(iconPos);
		icon->SetName(param.iconName);
		icon->SetObjType(GROUP_TYPE::UI);
		icon->AddImage(pD2DMgr->GetStoredBitmap(param.iconName));
		icon->SetScale(iconScale * 2.f);

		// 텍스트 생성
		CSpriteUI* text = statItem->AddChild<CSpriteUI>(textPos);
		wchar_t buffer[20];
		swprintf_s(buffer, L"%.2f", param.statValue);

		// 각 스탯 타입에 따라 고유 이름 지정
		switch (paramIndex) { // paramIndex는 0~3의 루프 인덱스
		case 0: text->SetName(L"AttackText"); break;
		case 1: text->SetName(L"AttackSpeedText"); break;
		case 2: text->SetName(L"MoveSpeedText"); break;
		case 3: text->SetName(L"AttackRangeText"); break;
		}
		
		text->SetObjType(GROUP_TYPE::UI);
		text->SetScale(Vec2(50.f, 32.f));
		text->CreateTextUI(
			buffer,
			-(text->GetScale() / 2.f),
			(text->GetScale() / 2.f),
			textFontSize,
			D2D1::ColorF::White,
			true, 1.f, D2D1::ColorF::Black, 0
		);

		// 맵에 저장 (클래스 멤버 변수 추가 필요)
		m_mapStatTexts[static_cast<StatType>(paramIndex)] = text;
		paramIndex++;
	}
}

void CScene_Fight::UpdateStatUI()
{
	CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
	const PlayerStat& current = player->GetPlayerStat();

	// 업데이트 정보 배열
	StatUpdateInfo stats[] = {
		{StatType::Attack,        current.m_fAttackDmg,    &m_stPrevPlayerStat.m_fAttackDmg,    1.0f},
		{StatType::Attack_Speed,  current.m_fAttackSpd,    &m_stPrevPlayerStat.m_fAttackSpd,    1.0f},
		{StatType::Move_Speed,    current.m_fMoveSpd,      &m_stPrevPlayerStat.m_fMoveSpd,      0.01f},
		{StatType::Attack_Range,  current.m_fAttackRange,  &m_stPrevPlayerStat.m_fAttackRange,  0.01f},
	};

	for (auto& stat : stats) {
		if (abs(stat.currentValue - *stat.prevValue) > FLT_EPSILON) {
			auto it = m_mapStatTexts.find(stat.type);
			if (it != m_mapStatTexts.end()) {
				wchar_t buffer[20];
				const float displayValue = stat.currentValue * stat.multiplier;
				swprintf_s(buffer, L"%.2f", displayValue);
				it->second->GetTextUI()->SetText(buffer); // wstring 변환 생략
				//*stat.prevValue = stat.currentValue; // 이전 값 직접 업데이트
			}
		}
	}
}

void CScene_Fight::CreateHpUI()
{
	ID2D1Bitmap* heartBitmap = pD2DMgr->GetStoredBitmap(L"ui_hearts");
	pD2DMgr->SplitBitmap(heartBitmap, L"full_heart", { 0,0 }, { 16,16 });
	pD2DMgr->SplitBitmap(heartBitmap, L"half_heart", { 16,0 }, { 32,16 });
	pD2DMgr->SplitBitmap(heartBitmap, L"empty_heart", { 32,0 }, { 48,16 });

	// 이미지 캐싱
	auto fullImg = pD2DMgr->GetStoredBitmap(L"full_heart");
	auto halfImg = pD2DMgr->GetStoredBitmap(L"half_heart");
	auto emptyImg = pD2DMgr->GetStoredBitmap(L"empty_heart");

	hpPanel = new CPanelUI;
	hpPanel->SetObjType(GROUP_TYPE::UI);
	hpPanel->SetName(L"HpPanel");
	hpPanel->SetMouseOnAlpha(0.f);
	hpPanel->SetNormalAlpha(0.f);
	hpPanel->SetScale(Vec2(160.f, 50.f));
	hpPanel->SetPos(Vec2(180.f, 40.f));
	// ... 패널 설정 코드 유지 ...

	CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
	const PlayerStat& stat = player->GetPlayerStat();

	// 체력 계산
	const int maxHearts = static_cast<int>(stat.m_fMaxHp);
	const int fullHearts = static_cast<int>(stat.m_fCurHp);
	const bool hasHalf = (stat.m_fCurHp - fullHearts) >= 0.5f;
	const int emptyHearts = maxHearts - fullHearts - (hasHalf ? 1 : 0);

	// 그리드 상수
	int COL_COUNT = 6;
	float START_X = -80.f + 13.f;
	float START_Y = -25.f + 13.f;
	float HEART_SIZE = 26.f;
	Vec2 HEART_SCALE(13.f * 2, 13.f * 2);

	// 모든 하트 생성
	for (int i = 0; i < maxHearts; ++i)
	{
		const int col = i % COL_COUNT;
		const int row = i / COL_COUNT;
		const Vec2 pos(START_X + HEART_SIZE * col, START_Y + HEART_SIZE * row);

		CSpriteUI* heart = hpPanel->AddChild<CSpriteUI>(pos);
		m_vecHearts.push_back(heart);
		heart->SetScale(HEART_SCALE);
		heart->SetObjType(GROUP_TYPE::UI);

		if (i < fullHearts) {
			heart->AddImage(fullImg);
		}
		else if (i == fullHearts && hasHalf) {
			heart->AddImage(halfImg);
		}
		else {
			heart->AddImage(emptyImg);
		}
	}

	AddObject(hpPanel, GROUP_TYPE::UI);
}

void CScene_Fight::UpdateHpUI()
{
	CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
	const PlayerStat& current = player->GetPlayerStat();

	if (current.m_fCurHp != m_stPrevPlayerStat.m_fCurHp || current.m_fMaxHp != m_stPrevPlayerStat.m_fMaxHp)
	{
		DeleteObject(hpPanel);
		CreateHpUI();
	}

	m_stPrevPlayerStat = current;
}

void CScene_Fight::CreateActiveItemUI()
{
	// 패널 생성 및 초기화
	activeItemPanel = new CPanelUI;
	activeItemPanel->SetObjType(GROUP_TYPE::UI);
	activeItemPanel->SetName(L"ActiveItemPanel");
	activeItemPanel->SetMouseOnAlpha(0.f);
	activeItemPanel->SetNormalAlpha(0.f);
	activeItemPanel->SetScale(Vec2(85.f, 66.f));
	activeItemPanel->SetPos(Vec2(50.f, 40.f));

	// 현재 활성 아이템 가져오기
	Item* activeItem = CItemMgr::GetInstance()->GetPossessedActiveItem();

	if (activeItem != nullptr)
	{
		// ChargeBar 생성 및 초기화
		chargeBar = activeItemPanel->AddChild<CSpriteUI>(Vec2(30.f, 0.f));
		chargeBar->SetObjType(GROUP_TYPE::UI);
		chargeBar->SetScale(Vec2(16.f, 32.f) * 2.f);

		// 공통 이미지 캐싱 및 추가
		auto* pDirect2DMgr = Direct2DMgr::GetInstance();
		chargeBar->AddImage(pDirect2DMgr->GetStoredBitmap(L"empty_charge"));
		chargeBar->AddImage(pDirect2DMgr->GetStoredBitmap(L"full_charge"));

		// MaxCharge에 따른 이미지 매핑
		static const std::unordered_map<int, wstring> chargeImageMap = {
			{3, L"three_space_charge"},
			{4, L"four_space_charge"},
			{5, L"five_space_charge"},
			{6, L"six_space_charge"},
			{8, L"eight_space_charge"}
		};

		auto it = chargeImageMap.find(activeItem->m_iMaxCharge);
		if (it != chargeImageMap.end())
		{
			chargeBar->AddImage(pDirect2DMgr->GetStoredBitmap(it->second));
		}

		// ChargeBar 비율 설정
		vector<CImage*>& images = chargeBar->GetImages();
		float ratioY = ChargeBarRatio(activeItem->m_iCurCharge, activeItem->m_iMaxCharge);
		images[1]->SetRatioY(ratioY);

		// 활성 아이템 이미지 태그 생성 및 추가
		wstringstream itemImageTag;
		itemImageTag << L"collectibles_"
			<< setw(3) << setfill(L'0') << activeItem->m_iNumber
			<< L"_" << activeItem->m_sItemTag;

		CSpriteUI* activeItemImage = activeItemPanel->AddChild<CSpriteUI>(Vec2(-10.f, -5.f));
		activeItemImage->SetObjType(GROUP_TYPE::UI);
		activeItemImage->AddImage(pDirect2DMgr->GetStoredBitmap(itemImageTag.str()));
		activeItemImage->SetScale(Vec2(32.f, 32.f) * 2.f);
	}

	AddObject(activeItemPanel, GROUP_TYPE::UI);
}

void CScene_Fight::UpdateActiveItemUI()
{
	// 현재 활성 아이템 가져오기
	Item* activeItem = CItemMgr::GetInstance()->GetPossessedActiveItem();

	if (activeItem && chargeBar != nullptr)
	{
		// ChargeBar 비율 업데이트
		float ratioY = ChargeBarRatio(activeItem->m_iCurCharge, activeItem->m_iMaxCharge);
		chargeBar->GetImages()[1]->SetRatioY(ratioY);
	}
	else
	{
		// 패널 재생성 (아이템이 없는 경우)
		DeleteObject(activeItemPanel);
		CreateActiveItemUI();
	}
}

void CScene_Fight::CreatePickUpUI()
{
	// 1. 공용 상수 정의
	Vec2 MAIN_PANEL_SCALE(74.f, 77.f);
	Vec2 ITEM_PANEL_SCALE(74.f, 20.f);
	Vec2 IMAGE_OFFSET(-25.f, 0.f);
	Vec2 IMAGE_SCALE(Vec2(16.f, 16.f) * 2.f);
	Vec2 TEXT_SCALE(50.f, 32.f);

	// 2. 데이터 소스 캐싱
	PickUp& pickUpInfo = CItemMgr::GetInstance()->GetPickUpItem();

	// 3. UI 생성 헬퍼 함수
	auto CreateItemUI = [&](CPanelUI* parent, float yOffset, const wchar_t* bitmapName, int count, int idx) {
		wchar_t buffer[20];
		swprintf_s(buffer, L"%02d", count);

		// 패널 생성
		CPanelUI* panel = parent->AddChild<CPanelUI>(Vec2(0.f, yOffset));
		panel->SetScale(ITEM_PANEL_SCALE);
		panel->SetMouseOnAlpha(0.f);
		panel->SetNormalAlpha(0.f);

		// 아이콘 생성
		CSpriteUI* icon = panel->AddChild<CSpriteUI>(IMAGE_OFFSET);
		icon->SetObjType(GROUP_TYPE::UI);
		icon->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(bitmapName));
		icon->SetScale(IMAGE_SCALE);

		// 텍스트 생성
		CSpriteUI* text = panel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
		text->SetObjType(GROUP_TYPE::UI);
		text->SetScale(TEXT_SCALE);
		text->CreateTextUI(
			buffer,
			-(text->GetScale() / 2.f),
			(text->GetScale() / 2.f),
			20,
			D2D1::ColorF::White,
			true, 1.f, D2D1::ColorF::Black, 2
		);

		// 맵에 저장 (클래스 멤버 변수 추가 필요)
		m_mapPickUpText[static_cast<PickUpType>(idx)] = text;
	};

	// 4. 메인 패널 설정
	pickUpItemPanel = new CPanelUI;
	pickUpItemPanel->SetObjType(GROUP_TYPE::UI);
	pickUpItemPanel->SetName(L"PickUpItemPanel");
	pickUpItemPanel->SetScale(MAIN_PANEL_SCALE);
	pickUpItemPanel->SetPos(Vec2(40.f, 120.f));
	pickUpItemPanel->SetMouseOnAlpha(0.f);
	pickUpItemPanel->SetNormalAlpha(0.f);

	// 5. 아이템 UI 일괄 생성
	CreateItemUI(pickUpItemPanel, -25.f, L"item_pickup_coin", pickUpInfo.m_iCoin , 0);
	CreateItemUI(pickUpItemPanel, 0.f, L"item_pickup_bomb", pickUpInfo.m_iBomb , 1);
	CreateItemUI(pickUpItemPanel, 25.f, L"item_pickup_key", pickUpInfo.m_iKey, 2);

	AddObject(pickUpItemPanel, GROUP_TYPE::UI);
}

void CScene_Fight::UpdatePickUpUI()
{
	PickUp& current = CItemMgr::GetInstance()->GetPickUpItem();

	// 업데이트 정보 배열
	PickUpUpdateInfo stats[] = {
		{PickUpType::Coin,        current.m_iCoin,    &m_stPrevPickups.m_iCoin},
		{PickUpType::Bomb,  current.m_iBomb,    &m_stPrevPickups.m_iBomb},
		{PickUpType::Key,    current.m_iKey,      &m_stPrevPickups.m_iKey},
	};

	for (auto& stat : stats) {
		if (abs(stat.currentValue - *stat.prevValue) != 0) {
			auto it = m_mapPickUpText.find(stat.type);
			if (it != m_mapPickUpText.end()) {
				wchar_t buffer[20];
				const int displayValue = stat.currentValue;
				swprintf_s(buffer, L"%02d", displayValue);
				it->second->GetTextUI()->SetText(buffer); // wstring 변환 생략
			}
		}
	}

	m_stPrevPickups = current;
}

void CScene_Fight::CuttingMiniMapImages()
{
	// 기존 미니맵 배경 생성
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"minimap1"), L"minimap_background", { 0,0 }, { 53,47 });

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"minimap1"), L"minimap_cell_background", { 64,0 }, { 64 + 13, 0 + 12 });

	// 셀 이미지 분할
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"minimap1"), L"minimap_cell_1", { 27,224 }, { 27 + 9, 224 + 8 });//미확인셀
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"minimap1"), L"minimap_cell_2", { 27,192 }, { 27 + 9, 192 + 8 });//현재셀
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"minimap1"), L"minimap_cell_3", { 27,160 }, { 27 + 9, 160 + 8 });//확인된셀

	//방 아이콘
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"minimap_icons"), L"boss_room", { 18, 18 }, { 18 + 9, 18 + 8 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"minimap_icons"), L"treasure_room", { 66 ,2 }, { 66 + 9, 2 + 8 });
}

void CScene_Fight::CreateMiniMapOriginal()
{
	// 그리드 맵 기반 동적 비트맵 생성
	vector<vector<UINT>>& gridMap = MapMgr::GetInstance()->GetGridMap();
	vector<vector<CellMap*>>& cellMaps = MapMgr::GetInstance()->GetAllCellMaps();


	const UINT rows = gridMap.size();
	const UINT cols = rows > 0 ? gridMap[0].size() : 0;

	
	// 1. 비트맵 렌더 타겟 생성
	ID2D1BitmapRenderTarget* pGridRenderTarget = nullptr;
	HRESULT hr = pD2DMgr->GetRenderTarget()->CreateCompatibleRenderTarget(
		D2D1::SizeF(cols * 18.f + 54.f * 2.f, rows * 16.f + 48.f * 2.f),
		&pGridRenderTarget
	);

	if (SUCCEEDED(hr))
	{
		pGridRenderTarget->BeginDraw();

		// 2. 셀 이미지 로드
		ID2D1Bitmap* pCellImage = pD2DMgr->GetStoredBitmap(L"minimap_cell_1");
		ID2D1Bitmap* pCellImage2 = pD2DMgr->GetStoredBitmap(L"minimap_cell_3");

		ID2D1Bitmap* pCellImageBackground = pD2DMgr->GetStoredBitmap(L"minimap_cell_background");
		if (pCellImage)
		{
			// 이미지 원본 크기 정보
			D2D1_SIZE_F imageSize = pCellImage->GetSize();
			D2D1_RECT_F srcRect = D2D1::RectF(0, 0, imageSize.width, imageSize.height);
			D2D1_RECT_F srcRect2 = D2D1::RectF(0, 0, pCellImageBackground->GetSize().width, pCellImageBackground->GetSize().height);

			// 모든 그리드 셀에 이미지 렌더링
			for (UINT y = 0; y < rows; ++y)
			{
				for (UINT x = 0; x < cols; ++x)
				{
					// 값이 0인 셀은 건너뜀
					if (gridMap[y][x] == 0)
						continue;

					// 8x8 대상 영역 계산
					D2D1_RECT_F destRect2 = D2D1::RectF(
						(x * 18.f) - 4.f + 54.f,
						(y * 16.f) - 4.f + 48.f,
						(x * 18.f) - 4.f + 26.f + 54.f,
						(y * 16.f) - 4.f + 24.f + 48.f
					);
					// 이미지 렌더링 (9x8 -> 8x8 자동 스케일링)
					pGridRenderTarget->DrawBitmap(
						pCellImageBackground,
						destRect2,
						1.0f,
						D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
						srcRect2
					);
				}
			}


			//2번째
			// 모든 그리드 셀에 이미지 렌더링
			for (UINT y = 0; y < rows; ++y)
			{
				for (UINT x = 0; x < cols; ++x)
				{
					// 값이 0인 셀은 건너뜀
					if (gridMap[y][x] == 0)
						continue;

					// 8x8 대상 영역 계산
					D2D1_RECT_F destRect = D2D1::RectF(
						x * 18.f + 54.f,
						y * 16.f + 48.f,
						(x + 1) * 18.f + 54.f,
						(y + 1) * 16.f + 48.f
					);

					// 이미지 렌더링 (9x8 -> 8x8 자동 스케일링)
					if (cellMaps[y][x]->IsChecked())
					{
						pGridRenderTarget->DrawBitmap(
							pCellImage2,
							destRect,
							1.0f,
							D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
							srcRect
						);
					}
					else
					{
						pGridRenderTarget->DrawBitmap(
							pCellImage,
							destRect,
							1.0f,
							D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
							srcRect
						);
					}


					if (gridMap[y][x] == 5)
					{
						// 이미지 렌더링 (9x8 -> 8x8 자동 스케일링)
						pGridRenderTarget->DrawBitmap(
							pD2DMgr->GetStoredBitmap(L"boss_room"),
							destRect,
							1.0f,
							D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
							srcRect
						);
					}
					else if (gridMap[y][x] == 4)
					{
						// 이미지 렌더링 (9x8 -> 8x8 자동 스케일링)
						pGridRenderTarget->DrawBitmap(
							pD2DMgr->GetStoredBitmap(L"treasure_room"),
							destRect,
							1.0f,
							D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
							srcRect
						);
					}
				}
			}

		}

		pGridRenderTarget->EndDraw();

		// 3. 생성된 비트맵 저장
		ID2D1Bitmap* pGridBitmap = nullptr;
		pGridRenderTarget->GetBitmap(&pGridBitmap);	
		pD2DMgr->DeleteBitmap(L"minimap_grid");
		pD2DMgr->StoreCreateMap(pGridBitmap, L"minimap_grid");
		pGridRenderTarget->Release();
	}
}

void CScene_Fight::CreateMiniMap()
{
	CreateMiniMapOriginal();
	// UI 패널 설정
	miniMapPanel = new CPanelUI;
	miniMapPanel->SetObjType(GROUP_TYPE::UI);
	miniMapPanel->SetScale(Vec2(55.f, 49.f) * 2.f );
	miniMapPanel->SetPos(Vec2(900.f, 50.f));
	miniMapPanel->SetNormalAlpha(0.f);
	miniMapPanel->SetMouseOnAlpha(0.f);
	miniMapPanel->AddImage(pD2DMgr->GetStoredBitmap(L"minimap_background"));


	miniMapImage = miniMapPanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	miniMapImage->SetObjType(GROUP_TYPE::UI);
	miniMapImage->SetScale(Vec2(51.f, 45.f) * 2.f);
	
	AddObject(miniMapPanel, GROUP_TYPE::UI);
}

void CScene_Fight::UpdateMiniMap()
{
	Vec2 vCurPos = MapMgr::GetInstance()->GetCurPos();

	if (vCurPos.x != m_vPrevGridPos.x || vCurPos.y != m_vPrevGridPos.y)
	{
		CreateMiniMapOriginal();
		// 셀 이미지 분할

		ID2D1Bitmap* newImage = pD2DMgr->SplitBitmapNoSave(pD2DMgr->GetStoredBitmap(L"minimap_grid")
		, { vCurPos.x * 18.f + 9.f - 54.f + 54.f , vCurPos.y * 16.f + 8.f - 48.f + 48.f }
		, { vCurPos.x * 18.f + 9.f + 54.f + 54.f, vCurPos.y * 16.f + 8.f + 48.f + 48.f });

		miniMapImage->DeleteImages();
		miniMapImage->AddImage(newImage);
		miniMapImage->AddImage(pD2DMgr->GetStoredBitmap(L"minimap_cell_2"));

		vector<CImage*>& images = miniMapImage->GetImages();
		images[1]->ScaleCustomizing(Vec2(0.1666f, 0.1777f), true); // 18/108 , 16/90
	}

	m_vPrevGridPos = vCurPos;
}

void CScene_Fight::CreateBossHpBar()
{
	bossHpBarFrame = new CSpriteUI;
	bossHpBarFrame->SetName(L"보스체력바\n");
	bossHpBarFrame->SetObjType(GROUP_TYPE::UI);
	bossHpBarFrame->SetScale(Vec2(150.f, 32.f) * 2.f);
	bossHpBarFrame->AddImage(pD2DMgr->GetStoredBitmap(L"boss_hpbar_frame"));
	bossHpBarFrame->SetPos(Vec2(480.f,500.f));
	bossHpBarFrame->SetEnable(false);
	
	AddObject(bossHpBarFrame, GROUP_TYPE::UI);

	bossHpBar = bossHpBarFrame->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	bossHpBar->SetObjType(GROUP_TYPE::UI);
	bossHpBar->SetScale(Vec2(150.f, 32.f) * 2.f);
	bossHpBar->AddImage(pD2DMgr->GetStoredBitmap(L"boss_hpbar"));

	bossHpBarText = bossHpBarFrame->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	bossHpBarText->SetObjType(GROUP_TYPE::UI);
	bossHpBarText->SetScale(Vec2(50.f, 32.f) * 2.f);

	CBaby_Plum* boss = MapMgr::GetInstance()->GetBoss();
	wchar_t buffer[20];
	swprintf_s(buffer, L"%d / %d", boss->Getinfo().m_iCurHP, boss->Getinfo().m_iMaxHP);
	bossHpBarText->CreateTextUI(
		buffer,
		-(bossHpBarText->GetScale() / 2.f),
		(bossHpBarText->GetScale() / 2.f),
		15.f,
		D2D1::ColorF::White,
		true, 1.f, D2D1::ColorF::Black, 0
	);
}

void CScene_Fight::UpdateBossHpBar()
{	
	CBaby_Plum* boss = MapMgr::GetInstance()->GetBoss();
	
	if (boss->Getinfo().m_iCurHP <= 0)
	{
		bossHpBarFrame->SetEnable(false);
		return;
	}

	Vec2 m_vBossPos = MapMgr::GetInstance()->GetBossRoomGridPos();
	Vec2 m_vCurPos = MapMgr::GetInstance()->GetCurPos();

	if (m_vCurPos == m_vBossPos)
	{
		bossHpBarFrame->SetEnable(true);
	}
	

	
	bossHpBar->GetImages()[0]->SetRatioX((float)boss->Getinfo().m_iCurHP / (float)boss->Getinfo().m_iMaxHP);

	wchar_t buffer[20];
	swprintf_s(buffer, L"%d / %d", boss->Getinfo().m_iCurHP, boss->Getinfo().m_iMaxHP);
	bossHpBarText->GetTextUI()->SetText(buffer); // wstring 변환 생략
}

void CScene_Fight::PlayerDiedScreen()
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"death portraits"), L"death_main_panel", { 197, 0 }, { 197 + 220, 255 });
	
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"backselectwidget"), L"back_select_widget", { 5, 126 }, {5+92 , 126+88 }); //종료 버튼


	//5 126 92 88

	CSoundMgr::GetInstance()->StopAllSound();
	CSoundMgr::GetInstance()->Play(L"you died", 0.5f);

	CPanelUI* backPanel = new CPanelUI;
	backPanel->SetObjType(GROUP_TYPE::UI);
	backPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	backPanel->SetNormalAlpha(0.5f);
	backPanel->SetMouseOnAlpha(0.5f);
	backPanel->SetScale(CCore::GetInstance()->GetResolution());
	backPanel->SetPos(Vec2(480.f, 270.f));

	CPanelUI* deathMainPanel = backPanel->AddChild<CPanelUI>(Vec2(0.f, 0.f));
	deathMainPanel->SetMouseOnAlpha(0.f);
	deathMainPanel->SetNormalAlpha(0.f);
	deathMainPanel->SetObjType(GROUP_TYPE::UI);
	deathMainPanel->SetScale(Vec2(220.f, 255.f) * 2.f);
	deathMainPanel->AddImage(pD2DMgr->GetStoredBitmap(L"death_main_panel"));

	CPanelUI* deathEndPanel = backPanel->AddChild<CPanelUI>(Vec2(-380.f, 200.f));
	deathEndPanel->SetMouseOnAlpha(0.f);
	deathEndPanel->SetNormalAlpha(0.f);
	deathEndPanel->SetObjType(GROUP_TYPE::UI);
	deathEndPanel->SetScale(Vec2(92.f, 88.f) * 2.f);
	deathEndPanel->AddImage(pD2DMgr->GetStoredBitmap(L"back_select_widget"));

	CPanelUI* deathEndPanelESCText = deathEndPanel->AddChild<CPanelUI>(Vec2(0.f, 30.f));
	deathEndPanelESCText->SetMouseOnAlpha(0.f);
	deathEndPanelESCText->SetNormalAlpha(0.f);
	deathEndPanelESCText->SetObjType(GROUP_TYPE::UI);
	deathEndPanelESCText->SetScale(Vec2(71.f, 45.f));
	wchar_t buffer[20];
	swprintf_s(buffer, L"Esc");
	
	deathEndPanelESCText->CreateTextUI(
		buffer,
		-(deathEndPanelESCText->GetScale() / 2.f),
		(deathEndPanelESCText->GetScale() / 2.f),
		44.f,
		D2D1::ColorF::White,
		true, 1.f, D2D1::ColorF::Black, 0
	);
	
	AddObject(backPanel, GROUP_TYPE::UI);
}

void CScene_Fight::CheckPlayerIsDie()
{
	if (CPlayerMgr::GetInstance()->PlayerIsAlive() == false && !m_bGameOver)
	{
		m_bGameOver = true;
		PlayerDiedScreen();
	}
}
