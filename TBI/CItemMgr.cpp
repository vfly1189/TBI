#include "global.h"
#include "CItemMgr.h"

#include "Direct2DMgr.h"

CItemMgr::CItemMgr() 
	: m_stPickUps{ 0,0,0 }
	, m_iPrevActiveCharge(0)
{

}

CItemMgr::~CItemMgr()
{

}

void CItemMgr::SetActiveItemCharge(int _value)
{
	m_possessedActiveItem->m_iCurCharge += _value;

	// 범위 제한
	m_possessedActiveItem->m_iCurCharge = max(0, min(m_possessedActiveItem->m_iCurCharge, m_possessedActiveItem->m_iMaxCharge));
}

void CItemMgr::UseActiveItem()
{
	m_possessedActiveItem->m_iCurCharge = 0;
}

void CItemMgr::SetPossessedActiveItem(int _iItemNum)
{
	m_possessedActiveItem = &items[_iItemNum];
}


bool CItemMgr::SaveItems(const std::string& filename, const std::vector<Item>& items) {
	return false;
}

// 파일 읽기 함수
bool CItemMgr::LoadItems(const std::string& filename, std::vector<Item>& items) {
	return false;
}

void CItemMgr::init()
{
	for (size_t i = 0; i < items.size(); i++)
		items[i].m_sItemTag = items_tags[i];


	int randActiveItem = rand() % 3 + 32;
	m_possessedActiveItem = &items[randActiveItem];

	InitializeGraphics();
}

void CItemMgr::update()
{
	m_iPrevActiveCharge = m_possessedActiveItem->m_iCurCharge;
}

void CItemMgr::InitializeGraphics()
{
	CuttingItemPedestal();
	CuttingItemStarFlash();
	CuttingActiveItemChargeBar();
	CuttingPickUpImage();
	CuttingBombSpark();
	CuttingExplosion();
}

void CItemMgr::CuttingItemPedestal()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"levelitem_001_itemaltar"), L"item_pedestal",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(32.f, 32.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"levelitem_001_itemaltar"), L"item_pedestal_shadow",
		D2D1::Point2F(41.f, 8.f), D2D1::Point2F(41.f + 13.5f, 8.f + 5.f));
}

void CItemMgr::CuttingItemStarFlash()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	std::vector<ID2D1Bitmap*> vItem_Get_Effect;

	// 이펙트 비트맵 분리
	for (int i = 0; i < 3; i++) {
		std::wstring effectName = L"item_get_effect_" + std::to_wstring(i + 1);
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_023_starflash"), effectName,
			D2D1::Point2F(0.f, 32.f * i), D2D1::Point2F(128.f, 32.f * (i + 1)));
		vItem_Get_Effect.push_back(pD2DMgr->GetStoredBitmap(effectName));
	}

	// 효과 비트맵 결합
	pD2DMgr->DeleteBitmap(L"item_get_effect");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(vItem_Get_Effect), L"item_get_effect");
}

void CItemMgr::CuttingActiveItemChargeBar()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	// 충전 바 스프라이트 정의
	struct ChargeBarSprite {
		std::wstring name;
		float x, y;
	};

	// 충전 바 스프라이트 위치 데이터
	const ChargeBarSprite chargeSprites[] = {
		{ L"empty_charge", 0.f, 0.f },
		{ L"full_charge", 16.f, 0.f },
		{ L"three_space_charge", 16.f, 32.f },
		{ L"four_space_charge", 0.f, 32.f },
		{ L"five_space_charge", 16.f * 4, 0.f },
		{ L"six_space_charge", 16.f * 3, 0.f },
		{ L"eight_space_charge", 16.f * 4, 32.f }
	};

	// 모든 스프라이트 한 번에 처리
	for (const auto& sprite : chargeSprites) {
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_chargebar"), sprite.name,
			D2D1::Point2F(sprite.x, sprite.y), D2D1::Point2F(sprite.x + 16.f, sprite.y + 32.f));
	}
}

void CItemMgr::CuttingPickUpImage()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	// HUD 이미지 처리
	const struct HUDPickup {
		std::wstring name;
		float x, y;
	} hudPickups[] = {
		{ L"item_pickup_coin", 0.f, 0.f },
		{ L"item_pickup_key", 16.f, 0.f },
		{ L"item_pickup_bomb", 0.f, 16.f }
	};

	for (const auto& pickup : hudPickups) {
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"hudpickups"), pickup.name,
			D2D1::Point2F(pickup.x, pickup.y), D2D1::Point2F(pickup.x + 16.f, pickup.y + 16.f));
	}

	// 하트 이미지 처리
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_001_heart"), L"pickup_drop_heart",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(32.f, 32.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_001_heart"), L"pickup_drop_hearthalf",
		D2D1::Point2F(32.f, 0.f), D2D1::Point2F(64.f, 32.f));

	// 코인 이미지 및 애니메이션 처리
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_002_coin"), L"pickup_drop_coin",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(64.f, 48.f));

	std::vector<ID2D1Bitmap*> coinAnimFrames;
	for (int i = 0; i < 3; i++) {
		std::wstring frameName = L"pickup_drop_coin_animation_" + std::to_wstring(i + 1);
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_002_coin"), frameName,
			D2D1::Point2F(0.f, 16.f * i), D2D1::Point2F(64.f, 16.f * (i + 1)));
		coinAnimFrames.push_back(pD2DMgr->GetStoredBitmap(frameName));
	}

	pD2DMgr->DeleteBitmap(L"pickup_drop_coin_animation");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(coinAnimFrames), L"pickup_drop_coin_animation");

	// 키 이미지 처리
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_003_key"), L"pickup_drop_key",
		D2D1::Point2F(0.f, 4.f), D2D1::Point2F(15.f, 4.f + 21.f));

	// 폭탄 이미지 처리
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_016_bomb"), L"pickup_drop_bomb",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(32.f, 32.f));
}

void CItemMgr::CuttingBombSpark()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"bomb_spark"), L"bomb_spark_1",
		D2D1::Point2F(0, 0), D2D1::Point2F(32 * 4, 32));

	std::vector<ID2D1Bitmap*> sparkFrames;
	sparkFrames.push_back(pD2DMgr->GetStoredBitmap(L"bomb_spark_1"));

	pD2DMgr->DeleteBitmap(L"bomb_spark");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(sparkFrames), L"bomb_spark");
}

void CItemMgr::CuttingExplosion()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	std::vector<ID2D1Bitmap*> explosionFrames;

	// 폭발 애니메이션 프레임 처리
	for (int i = 0; i < 3; i++) {
		std::wstring frameName = L"explosion_" + std::to_wstring(i + 1);
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_029_explosion"), frameName,
			D2D1::Point2F(0, 96.f * i), D2D1::Point2F(96.f * 4, 96.f * (i + 1)));
		explosionFrames.push_back(pD2DMgr->GetStoredBitmap(frameName));
	}

	pD2DMgr->DeleteBitmap(L"explosion");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(explosionFrames), L"explosion");
}

