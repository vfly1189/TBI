#include "global.h"
#include "CItemMgr.h"

#include "Direct2DMgr.h"

CItemMgr::CItemMgr()
	: m_stPickUps{0,0,0}
{

}


CItemMgr::~CItemMgr()
{

}

void CItemMgr::SetActiveItemCharge(int _value)
{
	m_possessedActiveItem->m_iCurCharge += _value;

	if (m_possessedActiveItem->m_iCurCharge < 0) m_possessedActiveItem->m_iCurCharge = 0;
	if (m_possessedActiveItem->m_iCurCharge > m_possessedActiveItem->m_iMaxCharge)
		m_possessedActiveItem->m_iCurCharge = m_possessedActiveItem->m_iMaxCharge;
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

	m_possessedActiveItem = &items[33];

	CuttingItemPedestal();
	CuttingItemStarFlash();
	CuttingActiveItemChargeBar();
	CuttingPickUpImage();
}

void CItemMgr::update()
{
	m_iPrevActiveCharge = m_possessedActiveItem->m_iCurCharge;
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

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_023_starflash"), L"item_get_effect_1",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(128.f, 32.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_023_starflash"), L"item_get_effect_2",
		D2D1::Point2F(0.f, 32.f), D2D1::Point2F(128.f, 64.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"effect_023_starflash"), L"item_get_effect_3",
		D2D1::Point2F(0.f, 64.f), D2D1::Point2F(128.f, 96.f));

	vector<ID2D1Bitmap*> vItem_Get_Effect;
	vItem_Get_Effect.push_back(pD2DMgr->GetStoredBitmap(L"item_get_effect_1"));
	vItem_Get_Effect.push_back(pD2DMgr->GetStoredBitmap(L"item_get_effect_2"));
	vItem_Get_Effect.push_back(pD2DMgr->GetStoredBitmap(L"item_get_effect_3"));
	pD2DMgr->DeleteBitmap(L"item_get_effect");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(vItem_Get_Effect), L"item_get_effect");
}

void CItemMgr::CuttingActiveItemChargeBar()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_chargebar"), L"empty_charge",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(0.f + 16.f, 0.f + 32.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_chargebar"), L"full_charge",
		D2D1::Point2F(16.f, 0.f), D2D1::Point2F(16.f + 16.f, 0.f + 32.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_chargebar"), L"three_space_charge",
		D2D1::Point2F(16.f * 1, 32.f), D2D1::Point2F(16.f * 1 + 16.f, 32.f + 32.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_chargebar"), L"four_space_charge",
		D2D1::Point2F(16.f * 0, 32.f), D2D1::Point2F(16.f * 0 + 16.f, 32.f + 32.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_chargebar"), L"five_space_charge",
		D2D1::Point2F(16.f * 4, 0.f), D2D1::Point2F(16.f * 4 + 16.f, 0.f + 32.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_chargebar"), L"six_space_charge",
		D2D1::Point2F(16.f * 3, 0.f), D2D1::Point2F(16.f * 3 + 16.f, 0.f + 32.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"ui_chargebar"), L"eight_space_charge",
		D2D1::Point2F(16.f * 4, 32.f), D2D1::Point2F(16.f * 4 + 16.f, 32.f + 32.f));
}

void CItemMgr::CuttingPickUpImage()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	///hud 이미지///////////////////
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"hudpickups"), L"item_pickup_coin",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(0.f + 16.f, 0.f + 16.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"hudpickups"), L"item_pickup_key",
		D2D1::Point2F(16.f, 0.f), D2D1::Point2F(16.f + 16.f, 0.f + 16.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"hudpickups"), L"item_pickup_bomb",
		D2D1::Point2F(0.f, 16.f), D2D1::Point2F(0.f + 16.f, 16.f + 16.f));
	///hud 이미지///////////////////


	/////드랍 이미지////////////////////////
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_001_heart"), L"pickup_drop_heart",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(32.f, 32.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_001_heart"), L"pickup_drop_hearthalf",
		D2D1::Point2F(32.f, 0.f), D2D1::Point2F(64.f, 32.f));


	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_002_coin"), L"pickup_drop_coin",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(64.f, 48.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_002_coin"), L"pickup_drop_coin_animation_1",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(64.f, 16.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_002_coin"), L"pickup_drop_coin_animation_2",
		D2D1::Point2F(0.f, 16.f), D2D1::Point2F(64.f, 32.f)); 
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_002_coin"), L"pickup_drop_coin_animation_3",
		D2D1::Point2F(0.f, 32.f), D2D1::Point2F(64.f, 48.f));
	tmp_vector.clear();

	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"pickup_drop_coin_animation_1"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"pickup_drop_coin_animation_2"));
	tmp_vector.push_back(pD2DMgr->GetStoredBitmap(L"pickup_drop_coin_animation_3"));
	pD2DMgr->DeleteBitmap(L"pickup_drop_coin_animation");
	pD2DMgr->StoreCreateMap(CombineBitmapsX(tmp_vector), L"pickup_drop_coin_animation");



	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_003_key"), L"pickup_drop_key",
		D2D1::Point2F(0.f, 4.f), D2D1::Point2F(15.f, 4.f + 21.f));
	

	

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"pickup_016_bomb"), L"pickup_drop_bomb",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(32.f, 32.f));
	/////드랍 이미지////////////////////////
}
