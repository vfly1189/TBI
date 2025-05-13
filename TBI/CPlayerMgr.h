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

	vector<ID2D1Bitmap*> tmp_vector;

private:
	bool m_bIsAlive;

public:
	void CreateAndSettingPlayer();

	CPlayer* GetPlayer() { return m_Player; }
	ID2D1Bitmap* GetTearImage(int _idx) { return m_vTears[_idx]; }

public:
	bool PlayerIsAlive() { return m_bIsAlive; }

public:
	void SettingImageAndAnimations(int _characterIdx);

public:
	void CuttingAnimationImages(const std::wstring& _oriSprite, int _characterIdx);
	void CuttingCharacterAccessoriesAnimationImages(int _characterIdx);
	void CuttingTearImages();
	void CuttingAdditionalPoseImages(const std::wstring& _oriSprite, int _characterIdx);

public:
	void PlayerHit(float _dmg);

public:
	void CreateWalkingAnimation();
	void CreateAccessoriesAnimation(int _characterIdx);
	void CreateAdditionalPoseAnimation();

public:
	void init();


	friend class CPlayer;
};

