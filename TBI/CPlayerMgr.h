#pragma once

class CPlayer;
class Direct2DMgr;




class CPlayerMgr
{
	SINGLE(CPlayerMgr);

private:
	Direct2DMgr* pD2DMgr;

	CPlayer* m_Player;

	ID2D1Bitmap* idle;

	ID2D1Bitmap* walking_sprite; 
	ID2D1Bitmap* right_walking_sprite;
	ID2D1Bitmap* left_walking_sprite;

	ID2D1Bitmap* front_attack_sprite;
	ID2D1Bitmap* right_attack_sprite;
	ID2D1Bitmap* left_attack_sprite;
	ID2D1Bitmap* back_attack_sprite;

	ID2D1Bitmap* tear_explode_sprite;

	ID2D1Bitmap* item_get_front_walking_sprite;
	ID2D1Bitmap* item_get_right_walking_sprite;
	ID2D1Bitmap* item_get_left_walking_sprite;
	ID2D1Bitmap* item_get_back_walking_sprite;



	vector<ID2D1Bitmap*> m_vTears;
	vector<ID2D1Bitmap*> m_vAnimationBitmaps;
public:
	CPlayer* GetPlayer() { return m_Player; }
	ID2D1Bitmap* GetTearImage(int _idx) { return m_vTears[_idx]; }

public:
	void SettingImageAndAnimations(int _characterIdx);

public:
	void CuttingAnimationImages(wstring _oriSprite, int _characterIdx);
	void CuttingCharacterAccessoriesAnimationImages(int _characterIdx);
	void CuttingTearImages();


public:
	void CreateWalkingAnimation();
	void CreateAccessoriesAnimation(int _characterIdx);


public:
	void init();
};

