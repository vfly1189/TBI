#pragma once

typedef struct _PickUp
{
	int m_iCoin;
	int m_iBomb;
	int m_iKey;
}PickUp;


class CItemMgr
{
	SINGLE(CItemMgr);

private:
	Item*			m_possessedActiveItem;
	vector<Item *>	m_possessedPassiveItem;

	PickUp			m_stPickUps;
	
	int				m_iPrevActiveCharge;


	vector<ID2D1Bitmap*> tmp_vector;

public:
	void InitializeGraphics();

public:
	void SetActiveItemCharge(int _value);
	void UseActiveItem();
	void SetPossessedActiveItem(int _iItemNum);

public:
	Item* GetPossessedActiveItem() { return m_possessedActiveItem; }
	vector<Item *>& GetPossessedPassiveItem() { return m_possessedPassiveItem; }
	int GetPrevActiveCharge() { return m_iPrevActiveCharge; }

	PickUp& GetPickUpItem() { return m_stPickUps; }
	void ResetPickUp() { m_stPickUps = { 0,0,0 }; }


public:
	bool SaveItems(const string& filename, const std::vector<Item>& items);
	bool LoadItems(const string& filename, std::vector<Item>& items);

public:
	void init();
	void update();

public:
	void CuttingItemPedestal();//¹ÞÄ§´ë
	void CuttingItemStarFlash();
	void CuttingActiveItemChargeBar();
	void CuttingPickUpImage();
	void CuttingBombSpark();

	void CuttingExplosion();

};

