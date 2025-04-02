#pragma once
class CItemMgr
{
	SINGLE(CItemMgr);

private:
	ID2D1Bitmap* item_get_effect;

public:
	bool SaveItems(const string& filename, const std::vector<Item>& items);
	bool LoadItems(const string& filename, std::vector<Item>& items);

public:
	void init();

public:
	void CuttingItemPedestal();//¹ÞÄ§´ë
	void CuttingItemStarFlash();
};

