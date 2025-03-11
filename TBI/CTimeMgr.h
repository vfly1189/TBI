#pragma once


class CTimeMgr
{
	SINGLE(CTimeMgr);


private:
	//�̵����� ���������� �ɰ��� �̵��� ��Ű��, �̹� �����ӿ� �����̴� ���� ����.
	//�̵��� * 1�����ӿ� �ɸ��� �ð�(delta Time).
	LARGE_INTEGER m_llCurCount;
	LARGE_INTEGER m_llPrevCount;
	LARGE_INTEGER m_llFrequency;


	double			m_dDT;			//������ ���� �ð���. 
	double			m_dAcc;			//1�� üũ�� ���� ���� �ð�. 
	UINT			m_iCallCount;	//1�ʴ� ȣ�� Ƚ��. 
	UINT			m_iFPS;			//1�ʴ� ������. 

	float			m_ftimeScale;
public:
	//GetTick�� 1�ʴ� 1000�̶� ��Ȯ���� ����...
	//�Դٰ� ������ �־ �ξ� �� ������ �Լ���
	//QueryPerformanceCounter(); -> 1000�� ����.
	void init();
	void update();
	void render();

public:
	void SetTimeScale(float _fTimeScale) { m_ftimeScale = _fTimeScale; }

public:
	double GetDTN() { return m_dDT; }
	double GetDT() { return m_dDT * m_ftimeScale; }
	float GetfDTN() { return (float)m_dDT; }
	float GetfDT() { return (float)m_dDT * m_ftimeScale; }
};

