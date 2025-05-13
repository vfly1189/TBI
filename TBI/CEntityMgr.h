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
	void EntityImageCutting();
	void EntityBossBabyPlumCutting();
	void EntityAttackTear();
	void BossHpBar();

public:
	void EntityFlyDeadImage();
	void EntityNormalDeadImage();

public:
	void init();

};


