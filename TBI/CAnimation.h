#pragma once

class CAnimator;

//vLT : �»��. vSlice : �ڸ� ����. fDuration : �� �����ӿ��� �ӹ��� �ð�. 
//�� �����ӿ��� �ӹ��� �ð�. 
struct tAnimFrm {
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vOffset;
	float	fDuration;
};

class CAnimation
{
private:
	wstring					m_pAnimName;


	Vec2				m_vOffSet;
	Vec2				m_vFinalPos;

	CAnimator*				m_pAnimator;
	ID2D1Bitmap*			m_pBitmap;		//Animation���� ����ϴ� Direct2D �ؽ���.
	vector<tAnimFrm>		m_vecFrm;		//��� ������ ����

	int						m_iCurFrm;		//���� ������. 
	float					m_fAccTime;		//�ð� ����
	int						m_iFrameCount;


	bool					m_bFinish;		//�ִϸ��̼��� �������� �˸�. 

public:
	void SetName(const wstring& _name) { m_pAnimName = _name; }
	void SetOffset(Vec2 _vOffset) { m_vOffSet = _vOffset; }

public:
	Vec2 GetFinalPos() { return m_vFinalPos; }

public:
	void Create(ID2D1Bitmap* _bitmap, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep
		, float _fDuration, UINT _iFrameCount);

public:
	void update();
	void finalupdate();
	void render(ID2D1HwndRenderTarget* _pRender);

public:
	bool IsFinish() { return m_bFinish; }
	void SetFrame(int _iframeIDX) {
		m_bFinish = false;
		m_iCurFrm = _iframeIDX;
		m_fAccTime = 0.f;
	}

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

