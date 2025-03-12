#pragma once

enum class CAM_EFFECT {
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct tCamEffect {
	CAM_EFFECT	eEffect;			//ī�޶� ȿ��
	float		fDuration;		    //ȿ�� �ִ� ���� �ð�.
	float		fCurTime;			// ī�޶� ȿ�� ���� ����� �ð�. 
};

class CCamera
{
	SINGLE(CCamera)

private:
	//�׸����� ���� ī�޶��� ��ǥ�� ���� ����ؼ� ������.
	//�浹 ��ü�� �� ��ǥ�迡 �״��. 

	Vec2				m_vLookAt;			//ī�޶� ���� ��ġ
	Vec2				m_vCurLookAt;		//���� ��ġ�� ���� ��ġ ����. 
	Vec2				m_prevLookAt;		//ī�޶� ���� ���� ������ ��ġ. 

	Vec2				m_vMinBounds;		//�ּҹ���
	Vec2				m_vMaxBounds;		//�ִ����

	Vec2				m_vDiff;			//�ػ� �߽���ġ��, ī�޶� LootAt���� ���� ��. 
	float				m_fTime;			// Ÿ���� ���󰡴µ� �ɸ��� �ð�. 
	float				m_fSpeed;			//Ÿ���� ���󰡴� �ӵ�. 
	float				m_fAccTime;			//���� �ð�. 

	list<tCamEffect>	m_listCamEffect;

public:
	void SetLookAt(Vec2 _vLook) {
		m_vLookAt = _vLook;
		float fmoveDistance = (m_vLookAt - m_prevLookAt).Length();
		m_fSpeed = fmoveDistance / m_fTime;
		m_fAccTime = 0.f;
	}

	Vec2 GetLookAt() { return m_vCurLookAt; }
	Vec2 GetRenderPos(Vec2 _vObjPos) {
		return _vObjPos - m_vDiff;
	}

	Vec2 GetRealPos(Vec2 _vRenderPos) { { return _vRenderPos + m_vDiff; } }


public:
	void init();
	void update();

private:
	void CalDiff();
};

