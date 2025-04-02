#include "global.h"

#include "CPlayer.h"
#include "CObject.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CRigidBody.h"

#include "Direct2DMgr.h"
#include "CPlayerMgr.h"

const vector<wstring> characterAccessoriesPngFileString =
{
	L"",
	L"character_002_maggiesbeautifulgoldenlocks",
	L"character_003_cainseyepatch",
	L"character_004_judasfez"
};

const vector<Vec2> characterAccessoriesSliceSize =
{
	Vec2(64.f, 64.f),
	Vec2(32.f, 32.f),
	Vec2(32.f, 64.f)
};

CPlayerMgr::CPlayerMgr()
{

}
CPlayerMgr::~CPlayerMgr()
{

}

void CPlayerMgr::SettingImageAndAnimations(int _characterIdx)
{
	//////////////�⺻ sprite///////////////
	wstring oriSprite = L"character_00" + to_wstring(_characterIdx + 1) + L"_" + character_names[_characterIdx];

	printf("sdfs : %ls\n", oriSprite.c_str());

	CuttingAnimationImages(oriSprite, _characterIdx);
	CreateWalkingAnimation();

	if (_characterIdx != 0)
	{
		CuttingCharacterAccessoriesAnimationImages(_characterIdx);
		CreateAccessoriesAnimation(_characterIdx);
	}

	m_Player->GetAnimator()->Play(L"idle", true, 1);  // ���̾�1: ��
	m_Player->GetAnimator()->Play(L"front_idle", true, 2);  // ���̾�2: �Ӹ�

	if (_characterIdx != 0)
	{
		m_Player->m_sCurAccessoriesAnim = L"accessories_front_idle";
		m_Player->GetAnimator()->Play(L"accessories_front_idle", true, 3);
	}
}

void CPlayerMgr::CuttingAnimationImages(wstring _oriSprite, int _characterIdx)
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(_oriSprite), L"player_animation_sprite",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(256.f, 128.f));

	//�⺻����
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_idle",
		D2D1::Point2F(0.f, 32.f), D2D1::Point2F(32.f, 64.f));
	idle = pD2DMgr->GetStoredBitmap(L"player_idle");

	//����ȱ�
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_idle_walk_1",
		D2D1::Point2F(0.f, 32.f), D2D1::Point2F(256.f, 64.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_idle_walk_2",
		D2D1::Point2F(192.f, 0.f), D2D1::Point2F(256.f, 32.f));

	vector<ID2D1Bitmap*> walking;
	walking.push_back(pD2DMgr->GetStoredBitmap(L"player_idle_walk_1"));
	walking.push_back(pD2DMgr->GetStoredBitmap(L"player_idle_walk_2"));
	walking_sprite = CombineBitmapsX(walking);


	//����ȱ�
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_right_walk_1",
		D2D1::Point2F(0.f, 64.f), D2D1::Point2F(256.f, 96.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_right_walk_2",
		D2D1::Point2F(0.f, 96.f), D2D1::Point2F(64.f, 128.f));

	vector<ID2D1Bitmap*> right_walking;
	right_walking.push_back(pD2DMgr->GetStoredBitmap(L"player_right_walk_1"));
	right_walking.push_back(pD2DMgr->GetStoredBitmap(L"player_right_walk_2"));
	right_walking_sprite = CombineBitmapsX(right_walking);



	//����idle+���ݸ��
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_idle_head",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(32.f, 32.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_front_attack_head",
		D2D1::Point2F(32.f, 0.f), D2D1::Point2F(64.f, 32.f));

	vector<ID2D1Bitmap*> front_attack;
	front_attack.push_back(pD2DMgr->GetStoredBitmap(L"player_idle_head"));
	front_attack.push_back(pD2DMgr->GetStoredBitmap(L"player_front_attack_head"));
	front_attack_sprite = CombineBitmapsX(front_attack);

	//����idle+���ݸ��
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_right_head",
		D2D1::Point2F(64.f, 0.f), D2D1::Point2F(96.f, 32.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_right_attack_head",
		D2D1::Point2F(96.f, 0.f), D2D1::Point2F(128.f, 32.f));

	vector<ID2D1Bitmap*> right_attack;
	right_attack.push_back(pD2DMgr->GetStoredBitmap(L"player_right_head"));
	right_attack.push_back(pD2DMgr->GetStoredBitmap(L"player_right_attack_head"));
	right_attack_sprite = CombineBitmapsX(right_attack);

	//�ĸ�idle+���ݸ��
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_back_head",
		D2D1::Point2F(128.f, 0.f), D2D1::Point2F(160.f, 32.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_animation_sprite"), L"player_back_attack_head",
		D2D1::Point2F(160.f, 0.f), D2D1::Point2F(196.f, 32.f));

	vector<ID2D1Bitmap*> back_attack;
	back_attack.push_back(pD2DMgr->GetStoredBitmap(L"player_back_head"));
	back_attack.push_back(pD2DMgr->GetStoredBitmap(L"player_back_attack_head"));
	back_attack_sprite = CombineBitmapsX(back_attack);




	
	//������ ������� �ִϸ��̼�
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(_oriSprite), L"player_item_get_animation_sprite",
		D2D1::Point2F(256.f, 0.f), D2D1::Point2F(512.f, 128.f));

	//�Ӹ�
	if(_characterIdx == 0 || _characterIdx == 2 || _characterIdx == 3)
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(_oriSprite), L"player_item_get_head",
			D2D1::Point2F(272.f, 144.f), D2D1::Point2F(272.f + 32.f, 144.f + 32.f));
	else if (_characterIdx == 1)
	{
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(_oriSprite), L"player_item_get_head",
			D2D1::Point2F(272.f, 144.f), D2D1::Point2F(272.f + 32.f, 144.f + 32.f));
	}

	//����idle
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_item_get_animation_sprite"), L"player_item_get_idle",
		D2D1::Point2F(0.f, 32.f), D2D1::Point2F(32.f, 64.f));

	
	//����walking
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_item_get_animation_sprite"), L"player_item_get_front_walking_1",
		D2D1::Point2F(0.f, 32.f), D2D1::Point2F(256.f, 64.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_item_get_animation_sprite"), L"player_item_get_front_walking_2",
		D2D1::Point2F(192.f, 0.f), D2D1::Point2F(256.f, 32.f));
	walking.clear();
	walking.push_back(pD2DMgr->GetStoredBitmap(L"player_item_get_front_walking_1"));
	walking.push_back(pD2DMgr->GetStoredBitmap(L"player_item_get_front_walking_2"));
	item_get_front_walking_sprite = CombineBitmapsX(walking);

	
	//����walking
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_item_get_animation_sprite"), L"player_item_get_right_walking_1",
		D2D1::Point2F(0.f, 64.f), D2D1::Point2F(256.f, 96.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_item_get_animation_sprite"), L"player_item_get_right_walking_2",
		D2D1::Point2F(0.f, 96.f), D2D1::Point2F(64.f, 128.f));

	right_walking.clear();
	right_walking.push_back(pD2DMgr->GetStoredBitmap(L"player_item_get_right_walking_1"));
	right_walking.push_back(pD2DMgr->GetStoredBitmap(L"player_item_get_right_walking_2"));
	item_get_right_walking_sprite = CombineBitmapsX(right_walking);
}

void CPlayerMgr::CuttingCharacterAccessoriesAnimationImages(int _characterIdx)
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vSliceSize = characterAccessoriesSliceSize[_characterIdx - 1];

	printf("vSliceSize: %f %f\n", vSliceSize.x, vSliceSize.y);

	if (_characterIdx == 1 || _characterIdx == 2 || _characterIdx == 3)
	{
		///�Ӹ� �ĸ�
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(characterAccessoriesPngFileString[_characterIdx]), L"accessories_back",
			D2D1::Point2F(vSliceSize.x * 4.f, 0.f), D2D1::Point2F(vSliceSize.x * 6.f, vSliceSize.y));

		///�Ӹ� ������
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(characterAccessoriesPngFileString[_characterIdx]), L"accessories_right",
			D2D1::Point2F(vSliceSize.x * 2.f, 0.f), D2D1::Point2F(vSliceSize.x * 4.f, vSliceSize.y));

		///�Ӹ� ����
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(characterAccessoriesPngFileString[_characterIdx]), L"accessories_front",
			D2D1::Point2F(0.f, 0.f), D2D1::Point2F(0.f + vSliceSize.x * 2.f, vSliceSize.y));

		///�Ӹ� ����
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(characterAccessoriesPngFileString[_characterIdx]), L"accessories_left",
			D2D1::Point2F(vSliceSize.x * 6.f, 0.f), D2D1::Point2F(vSliceSize.x * 8.f, vSliceSize.y));
	}
}

void CPlayerMgr::CuttingTearImages()
{
	////////////////////////����///////////////////////////////
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"tears_multidimensional"), L"player_normal_tear",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(256.f, 64.f));

	int num = 0;
	for (int i = 0; i < 2; i++)
	{
		int max = 8;
		if (i == 1) max = 5;
		for (int j = 0; j < max; j++)
		{
			pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_normal_tear"), L"tear_" + std::to_wstring(num),
				D2D1::Point2F(32.f * j , 32.f * i), D2D1::Point2F(32.f * (j + 1), 32.f * (i +  1)));

			m_vTears.push_back(pD2DMgr->GetStoredBitmap(L"tear_" + std::to_wstring(num)));
			num++;
		}
	}
	////////////////////////����///////////////////////////////

	////////////////////////���� �����°�///////////////////////////////
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"bulletatlas"), L"player_normal_tear_explode",
		D2D1::Point2F(256.f, 256.f), D2D1::Point2F(512.f, 512.f));


	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_normal_tear_explode"), L"tear_explode_1",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(256.f, 64.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_normal_tear_explode"), L"tear_explode_2",
		D2D1::Point2F(0.f, 64.f), D2D1::Point2F(256.f, 128.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_normal_tear_explode"), L"tear_explode_3",
		D2D1::Point2F(0.f, 128.f), D2D1::Point2F(256.f, 192.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"player_normal_tear_explode"), L"tear_explode_4",
		D2D1::Point2F(0.f, 192.f), D2D1::Point2F(256.f, 256.f));
	vector<ID2D1Bitmap*> vTearExplode;
	vTearExplode.push_back(pD2DMgr->GetStoredBitmap(L"tear_explode_1"));
	vTearExplode.push_back(pD2DMgr->GetStoredBitmap(L"tear_explode_2"));
	vTearExplode.push_back(pD2DMgr->GetStoredBitmap(L"tear_explode_3"));
	vTearExplode.push_back(pD2DMgr->GetStoredBitmap(L"tear_explode_4"));
	tear_explode_sprite = CombineBitmapsX(vTearExplode);

	pD2DMgr->StoreCreateMap(tear_explode_sprite, L"tear_explode");
	////////////////////////���� �����°�///////////////////////////////
}

void CPlayerMgr::CreateWalkingAnimation()
{
	////////////////////////////////�⺻ ����////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"idle", walking_sprite
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, 0.f));
	////////////////////////////////�⺻ ����/////////////////////////////////////////

	////////////////////////////////�⺻ �ȱ� (���� , �ĸ� )����////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"idle_walking", walking_sprite
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 0.09f, 10, true, Vec2(0.f,0.f));
	////////////////////////////////�⺻ �ȱ� (���� , �ĸ� )����/////////////////////////////////////////

	////////////////////////////////�����ʰȱ�////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"right_walking", right_walking_sprite
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 0.09f, 10, true, Vec2(0.f, 0.f));
	////////////////////////////////�����ʰȱ�////////////////////////////////////////

	////////////////////////////////���ʰȱ�////////////////////////////////////////
	left_walking_sprite = FlipBitamp(right_walking_sprite, true, false);
	m_Player->GetAnimator()->CreateAnimation(L"left_walking", left_walking_sprite
		, Vec2(288.f, 0.f), Vec2(32.f, 32.f), Vec2(-32.f, 0.f), 0.09f, 10, true, Vec2(0.f, 0.f));
	////////////////////////////////���ʰȱ�////////////////////////////////////////

	////////////////////////////////�⺻���� �Ӹ�////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"front_idle", front_attack_sprite
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 1.f, 1, true, Vec2(0.f, -22.f));
	////////////////////////////////�⺻���� �Ӹ�////////////////////////////////////////

	////////////////////////////////������ �⺻���� �Ӹ�////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"right_idle", right_attack_sprite
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, -22.f));
	////////////////////////////////������ �⺻���� �Ӹ�////////////////////////////////////////

	////////////////////////////////���� �⺻���� �Ӹ�////////////////////////////////////////
	left_attack_sprite = FlipBitamp(right_attack_sprite, true, false);
	m_Player->GetAnimator()->CreateAnimation(L"left_idle", left_attack_sprite
		, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, -22.f));
	////////////////////////////////���� �⺻���� �Ӹ�////////////////////////////////////////

	////////////////////////////////�ĸ� �⺻���� �Ӹ�////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"back_idle", back_attack_sprite
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 1.f, 1, true, Vec2(0.f, -22.f));
	////////////////////////////////�ĸ� �⺻���� �Ӹ�////////////////////////////////////////



	////////////////////////////////�ĸ���� �Ӹ�////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(attack_animation_name[0], back_attack_sprite
		, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(-32.f, 0.f), m_Player->GetPlayerStat().m_fAttackSpd / 2.f, 2, true, Vec2(0.f, -22.f));
	////////////////////////////////�ĸ���� �Ӹ�////////////////////////////////////////

	////////////////////////////////�����ʰ��� �Ӹ�////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(attack_animation_name[1], right_attack_sprite
		, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(-32.f, 0.f), m_Player->GetPlayerStat().m_fAttackSpd / 2.f, 2, true, Vec2(0.f, -22.f));
	////////////////////////////////�����ʰ��� �Ӹ�////////////////////////////////////////

	////////////////////////////////������� �Ӹ�////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(attack_animation_name[2], front_attack_sprite
		, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(-32.f, 0.f), m_Player->GetPlayerStat().m_fAttackSpd / 2.f, 2, true, Vec2(0.f, -22.f));
	////////////////////////////////������� �Ӹ�////////////////////////////////////////

	////////////////////////////////���ʰ��� �Ӹ�////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(attack_animation_name[3], left_attack_sprite
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), m_Player->GetPlayerStat().m_fAttackSpd / 2.f, 2, true, Vec2(0.f, -22.f));
	////////////////////////////////���ʰ��� �Ӹ�////////////////////////////////////////



	////////////////////////////////������ ȹ�� �� �Ӹ� �⺻ ����////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"item_get_idle_head", pD2DMgr->GetStoredBitmap(L"player_item_get_head")
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, -22.f));
	////////////////////////////////������ ȹ�� �� �Ӹ� �⺻ ����/////////////////////////////////////////

	////////////////////////////////������ ȹ�� �� �� �⺻ ����////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"item_get_idle", pD2DMgr->GetStoredBitmap(L"player_item_get_idle")
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, 0.f));
	////////////////////////////////������ ȹ�� �� �� �⺻ ����/////////////////////////////////////////


	////////////////////������ ȹ�� �� ���� �ȱ�/////////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"item_get_front_walking", item_get_front_walking_sprite
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 0.1f, 10, true, Vec2(0.f, 0.f));
	////////////////////������ ȹ�� �� ���� �ȱ�/////////////////////////////////////////////

	////////////////////������ ȹ�� �� ������ �ȱ�/////////////////////////////////////////////
	m_Player->GetAnimator()->CreateAnimation(L"item_get_right_walking", item_get_right_walking_sprite
		, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 0.1f, 10, true, Vec2(0.f, 0.f));
	////////////////////������ ȹ�� �� ������ �ȱ�/////////////////////////////////////////////

	////////////////////������ ȹ�� �� ���� �ȱ�/////////////////////////////////////////////
	item_get_left_walking_sprite = FlipBitamp(item_get_right_walking_sprite, true, false);
	m_Player->GetAnimator()->CreateAnimation(L"item_get_left_walking", item_get_left_walking_sprite
		, Vec2(288.f, 0.f), Vec2(32.f, 32.f), Vec2(-32.f, 0.f), 0.1f, 10, true, Vec2(0.f, 0.f));
	////////////////////������ ȹ�� �� ���� �ȱ�/////////////////////////////////////////////
}

void CPlayerMgr::CreateAccessoriesAnimation(int _characterIdx)
{
	const vector<wstring> vecAccessoriesAnimName = {
		L"accessories_back_idle",
		L"accessories_right_idle",
		L"accessories_front_idle",
		L"accessories_left_idle"
	};

	const vector<wstring> vecAccessoriesAttackAnimName = {
		L"accessories_back_attack",
		L"accessories_right_attack",
		L"accessories_front_attack",
		L"accessories_left_attack"
	};

	/////////////������ ������/////////////////////
	m_Player->GetAnimator()->CreateAnimation(vecAccessoriesAnimName[0], Direct2DMgr::GetInstance()->GetStoredBitmap(L"accessories_back")
		, Vec2(0.f, 0.f), characterAccessoriesSliceSize[_characterIdx-1], Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, -22.f));
	m_Player->GetAnimator()->CreateAnimation(vecAccessoriesAnimName[1], Direct2DMgr::GetInstance()->GetStoredBitmap(L"accessories_right")
		, Vec2(0.f, 0.f), characterAccessoriesSliceSize[_characterIdx - 1], Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, -22.f));
	m_Player->GetAnimator()->CreateAnimation(vecAccessoriesAnimName[2], Direct2DMgr::GetInstance()->GetStoredBitmap(L"accessories_front")
		, Vec2(0.f, 0.f), characterAccessoriesSliceSize[_characterIdx - 1], Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, -22.f));
	m_Player->GetAnimator()->CreateAnimation(vecAccessoriesAnimName[3], Direct2DMgr::GetInstance()->GetStoredBitmap(L"accessories_left")
		, Vec2(0.f, 0.f), characterAccessoriesSliceSize[_characterIdx - 1], Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, -22.f));
	/////////////������ ������/////////////////////


	////////////���� ��///////////////////////////
	m_Player->GetAnimator()->CreateAnimation(vecAccessoriesAttackAnimName[0], Direct2DMgr::GetInstance()->GetStoredBitmap(L"accessories_back")
		, Vec2(characterAccessoriesSliceSize[_characterIdx - 1].x, 0.f), characterAccessoriesSliceSize[_characterIdx - 1], Vec2(-characterAccessoriesSliceSize[_characterIdx - 1].x, 0.f), m_Player->GetPlayerStat().m_fAttackSpd / 2.f, 2, true, Vec2(0.f, -22.f));
	m_Player->GetAnimator()->CreateAnimation(vecAccessoriesAttackAnimName[1], Direct2DMgr::GetInstance()->GetStoredBitmap(L"accessories_right")
		, Vec2(characterAccessoriesSliceSize[_characterIdx - 1].x, 0.f), characterAccessoriesSliceSize[_characterIdx - 1], Vec2(-characterAccessoriesSliceSize[_characterIdx - 1].x, 0.f), m_Player->GetPlayerStat().m_fAttackSpd / 2.f, 2, true, Vec2(0.f, -22.f));
	m_Player->GetAnimator()->CreateAnimation(vecAccessoriesAttackAnimName[2], Direct2DMgr::GetInstance()->GetStoredBitmap(L"accessories_front")
		, Vec2(characterAccessoriesSliceSize[_characterIdx - 1].x, 0.f), characterAccessoriesSliceSize[_characterIdx - 1], Vec2(-characterAccessoriesSliceSize[_characterIdx - 1].x, 0.f), m_Player->GetPlayerStat().m_fAttackSpd / 2.f, 2, true, Vec2(0.f, -22.f));
	m_Player->GetAnimator()->CreateAnimation(vecAccessoriesAttackAnimName[3], Direct2DMgr::GetInstance()->GetStoredBitmap(L"accessories_left")
		, Vec2(characterAccessoriesSliceSize[_characterIdx - 1].x, 0.f), characterAccessoriesSliceSize[_characterIdx - 1], Vec2(-characterAccessoriesSliceSize[_characterIdx - 1].x, 0.f), m_Player->GetPlayerStat().m_fAttackSpd / 2.f, 2, true, Vec2(0.f, -22.f));
	////////////���� ��///////////////////////////
}

void CPlayerMgr::init()
{
	m_Player = new CPlayer;
	m_Player->CreateAnimator();
	m_Player->CreateRigidBody();

	pD2DMgr = Direct2DMgr::GetInstance();

	CuttingTearImages();
}
