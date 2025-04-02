#include "global.h"
#include "CItemMgr.h"

#include "Direct2DMgr.h"

CItemMgr::CItemMgr()
{

}


CItemMgr::~CItemMgr()
{

}

bool CItemMgr::SaveItems(const std::string& filename, const std::vector<Item>& items) {
   
}

// 파일 읽기 함수
bool CItemMgr::LoadItems(const std::string& filename, std::vector<Item>& items) {
   
}

void CItemMgr::init()
{
	for (size_t i = 0; i < items.size(); i++)
		items[i].m_sItemTag = items_tags[i];

	CuttingItemPedestal();
	CuttingItemStarFlash();
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

	pD2DMgr->StoreCreateMap(CombineBitmapsX(vItem_Get_Effect), L"item_get_effect");
}
