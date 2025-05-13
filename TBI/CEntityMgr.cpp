#include "global.h"

#include "CEntityMgr.h"
#include "Direct2DMgr.h"
#include "CSoundMgr.h"

CEntityMgr::CEntityMgr()
{

}

CEntityMgr::~CEntityMgr()
{

}

void CEntityMgr::EntityImageCutting()
{
	//돌////
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"rocks_basement"), L"rock", { 0,0 }, { 32,32 });

	//파리///
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"normal_fly", { 0,0 }, { 64,32 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"attack_fly", { 0,32 }, { 128,64 });
	EntityFlyDeadImage();

	//holf
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_002_horf"), L"horf_1", { 0,0 }, { 64,32 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_002_horf"), L"horf_2", { 0,32 }, { 64, 64 });
	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"horf_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"horf_2"));
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"horf");


	//horf tear
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"bulletatlas"), L"mon_tear_explode", { 576, 0 }, { 576 +64 * 4, 64 * 4 });

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"mon_tear_explode"), L"monster_normal_tear", { 0, 0 }, {64, 64});

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"mon_tear_explode"), L"monster_normal_tear_explode_1", { 0, 0 }, { 256, 64 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"mon_tear_explode"), L"monster_normal_tear_explode_2", { 0, 64 }, { 256, 128 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"mon_tear_explode"), L"monster_normal_tear_explode_3", { 0, 128 }, { 256, 192 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"mon_tear_explode"), L"monster_normal_tear_explode_4", { 0, 192 }, { 256, 256 });
	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"monster_normal_tear_explode_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"monster_normal_tear_explode_2"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"monster_normal_tear_explode_3"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"monster_normal_tear_explode_4"));
	pD2DMgr->DeleteBitmap(L"monster_normal_tear_explode");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"monster_normal_tear_explode");

	EntityAttackTear();
	//몬스터 죽는 모션
	EntityNormalDeadImage();

	//보스이미지
	EntityBossBabyPlumCutting();
	//보스 체력바
	BossHpBar();
}

void CEntityMgr::EntityBossBabyPlumCutting()
{
	///기본모션
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_idle_1", { 64 * 4, 64 * 1 }, { 64 * 5, 64 * 2 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_idle_2", { 64 * 0, 64 * 2 }, { 64 * 1, 64 * 3 });
	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_idle_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_idle_2"));
	pD2DMgr->DeleteBitmap(L"babyplum_idle");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"babyplum_idle");


	//내려찍기
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_takedown_1", { 64 * 5, 64 * 5 }, { 64 * 8, 64 * 6 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_takedown_2", { 64 * 0, 64 * 0 }, { 64 * 2, 64 * 1 });
	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_attack_takedown_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_attack_takedown_2"));
	pD2DMgr->DeleteBitmap(L"babyplum_attack_takedown");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"babyplum_attack_takedown");


	//한바퀴 돌기
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_spin_1", { 64 * 0, 64 * 4 }, { 64 * 6, 64 * 5 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_spin_2", { 64 * 0, 64 * 5 }, { 64 * 5, 64 * 6 });
	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_attack_spin_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_attack_spin_2"));
	pD2DMgr->DeleteBitmap(L"babyplum_attack_spin");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"babyplum_attack_spin");


	//뒤로 눈물쏘면서
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_start", { 64 * 0, 64 * 6 }, { 64 * 2, 64 * 7 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_end", { 64 * 2, 64 * 6 }, { 64 * 4, 64 * 7 });
	
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_front_left_1", { 64 * 4, 64 * 6 }, { 64 * 5, 64 * 7 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_front_left_2", { 64 * 5, 64 * 6 }, { 64 * 6, 64 * 7 });
	
	//좌측
	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_front_left_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_front_left_2"));
	pD2DMgr->DeleteBitmap(L"babyplum_attack_backbounce_front_left");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"babyplum_attack_backbounce_front_left");

	//우측
	tmp_vector.clear();
	tmp_vector.push_back(FlipBitmap(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_front_left_1"), true, false));
	tmp_vector.push_back(FlipBitmap(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_front_left_2"), true, false));
	pD2DMgr->DeleteBitmap(L"babyplum_attack_backbounce_front_right");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"babyplum_attack_backbounce_front_right");
	
	
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_back_left_1", { 64 * 6, 64 * 6 }, { 64 * 7, 64 * 7 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_attack_backbounce_back_left_2", { 64 * 7, 64 * 6 }, { 64 * 8, 64 * 7 });

	//좌측
	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_back_left_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_back_left_2"));
	pD2DMgr->DeleteBitmap(L"babyplum_attack_backbounce_back_left");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"babyplum_attack_backbounce_back_left");

	
	//우측
	tmp_vector.clear();
	tmp_vector.push_back(FlipBitmap(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_back_left_1"), true, false));
	tmp_vector.push_back(FlipBitmap(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_back_left_2"), true, false));
	pD2DMgr->DeleteBitmap(L"babyplum_attack_backbounce_back_right");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"babyplum_attack_backbounce_back_right");
	

	//사망모션
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"babyplum"), L"babyplum_dead_1", { 64 * 6, 64 * 4 }, { 64 * 7, 64 * 5 });
	
	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_attack_backbounce_start"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"babyplum_dead_1"));
	pD2DMgr->DeleteBitmap(L"babyplum_dead");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"babyplum_dead");

	//사망이펙트
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_077_largebloodexplosion"), L"dead_explosion_1", {145, 0 }, { 435, 100 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_077_largebloodexplosion"), L"dead_explosion_2", { 0, 100 }, { 435, 200 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_077_largebloodexplosion"), L"dead_explosion_3", { 0, 220 }, { 435, 320 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_077_largebloodexplosion"), L"dead_explosion_4", { 0, 340 }, { 145, 440 });

	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"dead_explosion_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"dead_explosion_2"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"dead_explosion_3"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"dead_explosion_4"));
	pD2DMgr->DeleteBitmap(L"dead_explosion");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"dead_explosion");
}

void CEntityMgr::EntityAttackTear()
{
	// Normal tear animation
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"bulletatlas"), L"monster_tear_list",
		D2D1::Point2F(0.f, 64.f), D2D1::Point2F(256.f, 128.f));

	// Generate tear frames
	m_vMonTears.clear();
	int num = 0;
	for (int i = 0; i < 2; i++) {
		int max = (i == 0) ? 8 : 5;
		for (int j = 0; j < max; j++) {
			std::wstring tearName = L"tear_" + std::to_wstring(num);
			pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_tear_list"), tearName,
				D2D1::Point2F(32.f * j, 32.f * i),
				D2D1::Point2F(32.f * (j + 1), 32.f * (i + 1)));
			m_vMonTears.push_back(pD2DMgr->GetStoredBitmap(tearName));
			num++;
		}
	}
}

void CEntityMgr::EntityFlyDeadImage()
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"fly_dead_1", { 0, 64 }, { 256, 128 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"fly_dead_2", { 0, 128 }, { 256, 192 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"monster_001_fly"), L"fly_dead_3", { 0, 192 }, { 256, 256 });

	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"fly_dead_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"fly_dead_2"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"fly_dead_3"));
	pD2DMgr->DeleteBitmap(L"fly_dead");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"fly_dead");
}

void CEntityMgr::EntityNormalDeadImage()
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_002_bloodpoof_small"), L"mon_dead_1", { 0, 0 }, { 128, 32 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_002_bloodpoof_small"), L"mon_dead_2", { 0, 32 }, { 128, 64 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_002_bloodpoof_small"), L"mon_dead_3", { 0, 64 }, { 128, 96 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_002_bloodpoof_small"), L"mon_dead_4", { 0, 96 }, { 128, 128 });

	tmp_vector.clear();
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"mon_dead_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"mon_dead_2"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"mon_dead_3"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"mon_dead_4"));
	pD2DMgr->DeleteBitmap(L"mon_dead");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"mon_dead");
}

void CEntityMgr::init()
{
	pD2DMgr = Direct2DMgr::GetInstance();
	EntityImageCutting();
}

void CEntityMgr::BossHpBar()
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_bosshealthbar"), L"boss_hpbar_frame", { 0, 32 }, { 150, 64 });
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_bosshealthbar"), L"boss_hpbar", { 0, 0 }, { 150, 32 });
}
