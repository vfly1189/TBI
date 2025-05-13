#pragma once

class CAnimator;
class CAnimation;

//vLT : 좌상단. vSlice : 자를 영역. fDuration : 각 프레임에서 머무는 시간. 
//각 프레임에서 머무는 시간. 
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
	ID2D1Bitmap*			m_pBitmap;		//Animation에서 사용하는 Direct2D 텍스쳐.
	vector<tAnimFrm>		m_vecFrm;		//모든 프레임 정보

	int						m_iCurFrm;		//현재 프레임. 
	float					m_fAccTime;		//시간 누적
	int						m_iFrameCount;

	int m_iRenderPriority = 0;// 값이 낮을수록 먼저 렌더링

	bool					m_bFinish;		//애니메이션이 끝났음을 알림. 
	bool m_bRepeat;
	bool m_bPaused;
private:
	float m_fAlpha;

private:
	bool m_bFlicker = false;      // 깜빡임 활성화 플래그
	float m_fFlickerFreq = 0.1f;  // 깜빡임 주기(초)
	float m_fFlickerTimer = 0;    // 깜빡임 타이머
	float m_fFlickerSpeed = 5.0f; // 깜빡임 속도 조절

public:
	void SetName(const wstring& _name) { m_pAnimName = _name; }
	void SetOffset(Vec2 _vOffset) { m_vOffSet = _vOffset; }
	void SetAlpha(float _alpha) { m_fAlpha = _alpha; }
	void SetRenderPriority(int _iPriority) {
		m_iRenderPriority = _iPriority;
	}
	void SetDuration(float _duration)
	{
		for (size_t i = 0; i < m_vecFrm.size(); i++)
			m_vecFrm[i].fDuration = _duration;
	}

public:
	void EnableFlicker(bool enable, float freq = 0.1f, float speed = 5.0f) {
		m_bFlicker = enable;
		m_fFlickerFreq = freq;
		m_fFlickerSpeed = speed;
		m_fAlpha = 1.0f; // 초기화
	}
	

public:
	Vec2 GetFinalPos() { return m_vFinalPos; }
	ID2D1Bitmap* GetBitmap() { return m_pBitmap; }
	int GetRenderPriority() const {
		return m_iRenderPriority;
	}
	wstring GetName() { return m_pAnimName; }
	vector< tAnimFrm>& GetAllFrame() { return m_vecFrm; }

public:
	void Create(ID2D1Bitmap* _bitmap, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep
		, float _fDuration, UINT _iFrameCount, Vec2 _offSet);

public:
	void Pause() { m_bPaused = true; }
	void Resume() { m_bPaused = false; }
	bool IsPaused() const { return m_bPaused; }

public:
	CAnimation* Clone();

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

