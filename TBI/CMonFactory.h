#pragma once

// �ʿ��� ��� ����
class CMonster;
class CAnimator;
#include "global.h" // Vec2 ���Ǹ� ����

class CMonFactory
{
public:
    static float m_fRecog;
    static float m_fSpeed;

public:
    // ���� ���� �޼��� - �Ű����� Ÿ���� �����Ϳ��� ������ ����
    static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos, Vec2 _vGridPos, int option = 0);

private:
    // ���� �߰��� ���� ���� �޼���
    static CMonster* CreateFlyMonster(Vec2 _vPos, Vec2 _vGridPos, int option);
    static CMonster* CreateHorfMonster(Vec2 _vPos, Vec2 _vGridPos);
    static CMonster* CreateBabyPlumMonster(Vec2 _vPos, Vec2 _vGridPos);

    static void InitializeBaseMonster(CMonster* pMon, Vec2 _vPos, Vec2 _vGridPos,
        const wstring& name, Vec2 scale, float renderScaleMult);

    static void CreateAnimation(CAnimator* pAnimator, const wstring& animName, const wstring& bitmapName,
        Vec2 startPos, Vec2 frameSize, Vec2 step, float duration, int frameCount, bool loop,
        Vec2 offset = Vec2(0.f, 0.f));

private:
    // �����ڿ� �Ҹ��ڴ� private ����
    CMonFactory() {}
    ~CMonFactory() {}
};