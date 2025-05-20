#include "global.h"

#include "CEntityMgr.h"
#include "Direct2DMgr.h"
#include "CSoundMgr.h"

CEntityMgr::CEntityMgr() = default;
CEntityMgr::~CEntityMgr() = default;

void CEntityMgr::EntityImageCutting()
{
	ProcessRockImages();
	ProcessFlyImages();
	ProcessHorfImages();
	ProcessTearImages();
	ProcessDeathAnimations();
	ProcessBossImages();
}

void CEntityMgr::init()
{
	pD2DMgr = Direct2DMgr::GetInstance();
	EntityImageCutting();
}

void CEntityMgr::ProcessRockImages()
{
	// 돌 이미지 처리
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"rocks_basement"), L"rock", { 0, 0 }, { 32, 32 });
}

void CEntityMgr::ProcessFlyImages()
{
	// 파리 이미지 처리
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"normal_fly", { 0, 0 }, { 64, 32 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"attack_fly", { 0, 32 }, { 128, 64 });

	// 파리 사망 이미지
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"fly_dead_1", { 0, 64 }, { 256, 128 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"fly_dead_2", { 0, 128 }, { 256, 192 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"fly_dead_3", { 0, 192 }, { 256, 256 });

	std::vector<ID2D1Bitmap*> flyDeadFrames = {
		pD2DMgr->GetStoredBitmap(L"fly_dead_1"),
		pD2DMgr->GetStoredBitmap(L"fly_dead_2"),
		pD2DMgr->GetStoredBitmap(L"fly_dead_3")
	};

	pD2DMgr->DeleteBitmap(L"fly_dead");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(flyDeadFrames), L"fly_dead");
}

void CEntityMgr::ProcessHorfImages()
{
	// Horf 몬스터 이미지 처리
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_002_horf"), L"horf_1", { 0, 0 }, { 64, 32 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_002_horf"), L"horf_2", { 0, 32 }, { 64, 64 });

	std::vector<ID2D1Bitmap*> horfFrames = {
		pD2DMgr->GetStoredBitmap(L"horf_1"),
		pD2DMgr->GetStoredBitmap(L"horf_2")
	};

	pD2DMgr->StoreCreateMap(CombineBitmapsX(horfFrames), L"horf");
}


void CEntityMgr::ProcessTearImages()
{
	// 눈물 이미지 처리
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"bulletatlas"), L"mon_tear_explode", { 576, 0 }, { 576 + 64 * 4, 64 * 4 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"mon_tear_explode"), L"monster_normal_tear", { 0, 0 }, { 64, 64 });

	// 눈물 폭발 프레임 처리
	const std::vector<std::pair<std::wstring, std::pair<D2D1_POINT_2F, D2D1_POINT_2F>>> tearExplodeFrames = {
		{L"monster_normal_tear_explode_1", {{0, 0}, {256, 64}}},
		{L"monster_normal_tear_explode_2", {{0, 64}, {256, 128}}},
		{L"monster_normal_tear_explode_3", {{0, 128}, {256, 192}}},
		{L"monster_normal_tear_explode_4", {{0, 192}, {256, 256}}}
	};

	std::vector<ID2D1Bitmap*> explodeFrameBitmaps;
	for (const auto& frame : tearExplodeFrames) {
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"mon_tear_explode"),
			frame.first, frame.second.first, frame.second.second);
		explodeFrameBitmaps.push_back(pD2DMgr->GetStoredBitmap(frame.first));
	}

	pD2DMgr->DeleteBitmap(L"monster_normal_tear_explode");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(explodeFrameBitmaps), L"monster_normal_tear_explode");

	// 몬스터 공격 눈물 처리
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"bulletatlas"), L"monster_tear_list",
		D2D1::Point2F(0.f, 64.f), D2D1::Point2F(256.f, 128.f));

	// 눈물 프레임 생성
	m_vMonTears.clear();
	int num = 0;
	const int maxFramesPerRow[] = { 8, 5 }; // 각 행에 있는 최대 프레임 수

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < maxFramesPerRow[i]; j++) {
			std::wstring tearName = L"tear_" + std::to_wstring(num);
			D2D1_POINT_2F startPoint = { 32.f * j, 32.f * i };
			D2D1_POINT_2F endPoint = { 32.f * (j + 1), 32.f * (i + 1) };

			pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_tear_list"), tearName, startPoint, endPoint);
			m_vMonTears.push_back(pD2DMgr->GetStoredBitmap(tearName));
			num++;
		}
	}
}

void CEntityMgr::ProcessDeathAnimations()
{
	// 일반 몬스터 사망 애니메이션 처리
	const std::vector<std::pair<std::wstring, std::pair<D2D1_POINT_2F, D2D1_POINT_2F>>> deadFrames = {
		{L"mon_dead_1", {{0, 0}, {128, 32}}},
		{L"mon_dead_2", {{0, 32}, {128, 64}}},
		{L"mon_dead_3", {{0, 64}, {128, 96}}},
		{L"mon_dead_4", {{0, 96}, {128, 128}}}
	};

	std::vector<ID2D1Bitmap*> deadFrameBitmaps;
	for (const auto& frame : deadFrames) {
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_002_bloodpoof_small"),
			frame.first, frame.second.first, frame.second.second);
		deadFrameBitmaps.push_back(pD2DMgr->GetStoredBitmap(frame.first));
	}

	pD2DMgr->DeleteBitmap(L"mon_dead");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(deadFrameBitmaps), L"mon_dead");
}

void CEntityMgr::ProcessBossImages()
{
	ProcessBabyPlumImages();
	ProcessBossHpBar();
}

void CEntityMgr::ProcessBabyPlumImages()
{
	// 기본 모션
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_idle_1", { 64 * 4, 64 * 1 }, { 64 * 5, 64 * 2 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_idle_2", { 64 * 0, 64 * 2 }, { 64 * 1, 64 * 3 });

	std::vector<ID2D1Bitmap*> idleFrames = {
		pD2DMgr->GetStoredBitmap(L"babyplum_idle_1"),
		pD2DMgr->GetStoredBitmap(L"babyplum_idle_2")
	};

	pD2DMgr->DeleteBitmap(L"babyplum_idle");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(idleFrames), L"babyplum_idle");

	// 내려찍기
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_takedown_1", { 64 * 5, 64 * 5 }, { 64 * 8, 64 * 6 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_takedown_2", { 64 * 0, 64 * 0 }, { 64 * 2, 64 * 1 });

	std::vector<ID2D1Bitmap*> takedownFrames = {
		pD2DMgr->GetStoredBitmap(L"babyplum_attack_takedown_1"),
		pD2DMgr->GetStoredBitmap(L"babyplum_attack_takedown_2")
	};

	pD2DMgr->DeleteBitmap(L"babyplum_attack_takedown");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(takedownFrames), L"babyplum_attack_takedown");

	// 한바퀴 돌기
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_spin_1", { 64 * 0, 64 * 4 }, { 64 * 6, 64 * 5 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_spin_2", { 64 * 0, 64 * 5 }, { 64 * 5, 64 * 6 });

	std::vector<ID2D1Bitmap*> spinFrames = {
		pD2DMgr->GetStoredBitmap(L"babyplum_attack_spin_1"),
		pD2DMgr->GetStoredBitmap(L"babyplum_attack_spin_2")
	};

	pD2DMgr->DeleteBitmap(L"babyplum_attack_spin");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(spinFrames), L"babyplum_attack_spin");

	ProcessBabyPlumBackBounceImages();
	ProcessBabyPlumDeathImages();

	// 입장 모션
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_start", { 64 * 0, 64 * 1 }, { 64 * 5, 64 * 2 });
}

void CEntityMgr::ProcessBossHpBar()
{
	// 보스 체력바
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_bosshealthbar"), L"boss_hpbar_frame", { 0, 32 }, { 150, 64 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_bosshealthbar"), L"boss_hpbar", { 0, 0 }, { 150, 32 });
}

void CEntityMgr::ProcessBabyPlumBackBounceImages()
{
	// 뒤로 눈물쏘면서
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_start", { 64 * 0, 64 * 6 }, { 64 * 2, 64 * 7 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_end", { 64 * 2, 64 * 6 }, { 64 * 4, 64 * 7 });

	// 앞쪽 (좌측/우측) 방향 프레임
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_front_left_1", { 64 * 4, 64 * 6 }, { 64 * 5, 64 * 7 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_front_left_2", { 64 * 5, 64 * 6 }, { 64 * 6, 64 * 7 });

	// 좌측 방향 애니메이션
	std::vector<ID2D1Bitmap*> frontLeftFrames = {
		pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_front_left_1"),
		pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_front_left_2")
	};

	pD2DMgr->DeleteBitmap(L"babyplum_attack_backbounce_front_left");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(frontLeftFrames), L"babyplum_attack_backbounce_front_left");

	// 우측 방향 애니메이션 (좌측 프레임 플립)
	std::vector<ID2D1Bitmap*> frontRightFrames = {
		FlipBitmap(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_front_left_1"), true, false),
		FlipBitmap(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_front_left_2"), true, false)
	};

	pD2DMgr->DeleteBitmap(L"babyplum_attack_backbounce_front_right");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(frontRightFrames), L"babyplum_attack_backbounce_front_right");

	// 뒤쪽 (좌측/우측) 방향 프레임
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_back_left_1", { 64 * 6, 64 * 6 }, { 64 * 7, 64 * 7 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_back_left_2", { 64 * 7, 64 * 6 }, { 64 * 8, 64 * 7 });

	// 좌측 방향 애니메이션
	std::vector<ID2D1Bitmap*> backLeftFrames = {
		pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_back_left_1"),
		pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_back_left_2")
	};

	pD2DMgr->DeleteBitmap(L"babyplum_attack_backbounce_back_left");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(backLeftFrames), L"babyplum_attack_backbounce_back_left");

	// 우측 방향 애니메이션 (좌측 프레임 플립)
	std::vector<ID2D1Bitmap*> backRightFrames = {
		FlipBitmap(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_back_left_1"), true, false),
		FlipBitmap(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_back_left_2"), true, false)
	};

	pD2DMgr->DeleteBitmap(L"babyplum_attack_backbounce_back_right");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(backRightFrames), L"babyplum_attack_backbounce_back_right");
}

void CEntityMgr::ProcessBabyPlumDeathImages()
{
	// 사망 모션
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_dead_1", { 64 * 6, 64 * 4 }, { 64 * 7, 64 * 5 });

	std::vector<ID2D1Bitmap*> deadFrames = {
		pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_start"),
		pD2DMgr->GetStoredBitmap(L"babyplum_dead_1")
	};

	pD2DMgr->DeleteBitmap(L"babyplum_dead");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(deadFrames), L"babyplum_dead");

	// 사망 이펙트
	const std::vector<std::pair<std::wstring, std::pair<D2D1_POINT_2F, D2D1_POINT_2F>>> explosionFrames = {
		{L"dead_explosion_1", {{145, 0}, {435, 100}}},
		{L"dead_explosion_2", {{0, 100}, {435, 200}}},
		{L"dead_explosion_3", {{0, 220}, {435, 320}}},
		{L"dead_explosion_4", {{0, 340}, {145, 440}}}
	};

	std::vector<ID2D1Bitmap*> explosionBitmaps;
	for (const auto& frame : explosionFrames) {
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_077_largebloodexplosion"),
			frame.first, frame.second.first, frame.second.second);
		explosionBitmaps.push_back(pD2DMgr->GetStoredBitmap(frame.first));
	}

	pD2DMgr->DeleteBitmap(L"dead_explosion");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(explosionBitmaps), L"dead_explosion");
}