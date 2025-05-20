#pragma once

// 필요한 헤더 포함
class CMonster;
class CAnimator;
#include "global.h" // Vec2 정의를 위해

class CMonFactory
{
public:
    static float m_fRecog;
    static float m_fSpeed;

public:
    // 기존 정적 메서드 - 매개변수 타입을 포인터에서 참조로 변경
    static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos, Vec2 _vGridPos, int option = 0);

private:
    // 새로 추가된 정적 헬퍼 메서드
    static CMonster* CreateFlyMonster(Vec2 _vPos, Vec2 _vGridPos, int option);
    static CMonster* CreateHorfMonster(Vec2 _vPos, Vec2 _vGridPos);
    static CMonster* CreateBabyPlumMonster(Vec2 _vPos, Vec2 _vGridPos);

    static void InitializeBaseMonster(CMonster* pMon, Vec2 _vPos, Vec2 _vGridPos,
        const wstring& name, Vec2 scale, float renderScaleMult);

    static void CreateAnimation(CAnimator* pAnimator, const wstring& animName, const wstring& bitmapName,
        Vec2 startPos, Vec2 frameSize, Vec2 step, float duration, int frameCount, bool loop,
        Vec2 offset = Vec2(0.f, 0.f));

private:
    // 생성자와 소멸자는 private 유지
    CMonFactory() {}
    ~CMonFactory() {}
};