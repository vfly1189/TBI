#pragma once


typedef struct _PlayerStat
{
	float		m_fCurHp;				//���� hp
	float		m_fMaxHp;				//�ִ� hp

	float		m_fAttackDmg;			//���ݷ�
	float		m_fAttackDmgCoef;		//���ݷ� ���
	float		m_fAttackSpd;			//���ݼӵ�

	float		m_fMoveSpd;				//�̵��ӵ�

	float		m_fAttackRange;			//���ݻ�Ÿ�

}PlayerStat;

typedef struct _CharacterInfo
{
	PlayerStat		m_stat;
	wstring			m_characterName;
}CharacterInfo;

PlayerStat isaac_stat = {
	5.5, 8,
	5.f, 1.f, 0.5f,
	500.f,
	300.f
};

PlayerStat magdalene_stat = {
	4, 4,
	3.f, 1.f, 0.5f,
	500.f,
	300.f
};

PlayerStat cain_stat = {
	4, 4,
	3.f, 1.f, 0.5f,
	500.f,
	300.f
};

PlayerStat judas_stat = {
	4, 4,
	3.f, 1.f, 0.5f,
	500.f,
	300.f
};

const vector<CharacterInfo> vecCharacterInfo =
{
	{isaac_stat, character_names[0]},
	{magdalene_stat, character_names[1]},
	{cain_stat, character_names[2]},
	{judas_stat, character_names[3]},
};