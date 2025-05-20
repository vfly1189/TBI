#pragma once

class CEntityMgr
{
	SINGLE(CEntityMgr);

private:
	Direct2DMgr* pD2DMgr;
	vector<ID2D1Bitmap*> tmp_vector;
	vector<ID2D1Bitmap*> m_vMonTears;

public:
	ID2D1Bitmap* GetTearImage(int _idx) { return m_vMonTears[_idx]; }

public:
	void ProcessRockImages();
	void ProcessFlyImages();
	void ProcessHorfImages();
	void ProcessTearImages();
	void ProcessDeathAnimations();
	void ProcessBossImages();

	void ProcessBabyPlumImages();
	void ProcessBabyPlumBackBounceImages();
	void ProcessBabyPlumDeathImages();

	void ProcessBossHpBar();

public:
	void EntityImageCutting();


public:
	void init();

};


