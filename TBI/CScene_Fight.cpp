#include "global.h"

#include "Direct2DMgr.h"
#include "CCore.h"
#include "MapMgr.h"
#include "CCollisionMgr.h"
#include "CSoundMgr.h"
#include "CPlayerMgr.h"
#include "CKeyMgr.h"

#include "CScene_Fight.h"

#include "CDoor.h"
#include "CScene.h"
#include "CWall.h"
#include "CPanelUI.h"
#include "CPlayer.h"
#include "CSpriteUI.h"
#include "CellMap.h"
#include "CTextUI.h"

CScene_Fight::CScene_Fight()
{

}

CScene_Fight::~CScene_Fight()
{

}


void CScene_Fight::Enter()
{
	printf("SCENE_FIGHT\n");
	pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	CCamera::GetInstance()->SetLookAt(Vec2(0.f, 0.f));

	MapMgr::GetInstance()->MapGenerate();
	MapMgr::GetInstance()->ShowMap();

	
	vector<CellMap*>& cellMaps = MapMgr::GetInstance()->GetCellMaps();
	for (size_t i = 0; i < cellMaps.size(); ++i)
	{
		AddObject((CObject*)cellMaps[i]->GetCellMap(), GROUP_TYPE::IMAGE);

		vector<CObject*>& cellMapObjects = cellMaps[i]->GetObjects();	

		for (size_t j = 0; j < cellMapObjects.size(); j++)
		{
			GROUP_TYPE groupType = cellMapObjects[j]->GetObjType();

			AddObject(cellMapObjects[j], groupType);
		}	
	}

	CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
	player->SetPos(Vec2(0.f, 0.f));
	player->SetName(L"PLAYER");
	player->SetObjType(GROUP_TYPE::PLAYER);
	player->SetScale(Vec2(32.f, 32.f));
	player->SetRenderScale(player->GetScale() * 2.f);
	AddObject(player, GROUP_TYPE::PLAYER);

	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::ITEM);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DOOR);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::WALL);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::TEAR, GROUP_TYPE::WALL);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::TEAR, GROUP_TYPE::DOOR);

	wstring mainTitleBGMKey = L"genesis_retake_light_loop";
	//처음 시작했을 때, BGM틀기. 
	if (!CSoundMgr::GetInstance()->IsPlaySound(mainTitleBGMKey)) {
		CSoundMgr::GetInstance()->Play(mainTitleBGMKey, 0.2f);
	}

	CreateStatUI();
	CreateHpUI();
	start();
}

void CScene_Fight::Exit()
{
	DeleteAll();
}

void CScene_Fight::update()
{
	CScene::update();

	UpdateStatUI();
}

void CScene_Fight::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Fight::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
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
	constexpr float textFontSize = 11.f;

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


		/*
		statItem->SetColor(
			param.isFirstPanel ? ColorNormalize(255, 255, 255) : ColorNormalize(0, 0, 0),
			param.isFirstPanel ? ColorNormalize(255, 255, 255) : ColorNormalize(0, 0, 0)
		);*/

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
				*stat.prevValue = stat.currentValue; // 이전 값 직접 업데이트
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

	CPanelUI* hpPanel = new CPanelUI;
	hpPanel->SetObjType(GROUP_TYPE::UI);
	hpPanel->SetName(L"HpPanel");
	hpPanel->SetRadius(10.f, 10.f);
	hpPanel->SetScale(Vec2(160.f, 50.f));
	hpPanel->SetPos(Vec2(180.f, 35.f));
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


