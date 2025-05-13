#pragma once
class CMonster;

class CMonFactory
{
public:
	static float m_fRecog;
	static float m_fSpeed;

public:
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos, Vec2 _vGridPos, int option);
	

private:
	CMonFactory() {}
	~CMonFactory() {}
};