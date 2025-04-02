#pragma once


class CObject;
class CAnimation;

class CAnimator
{

private:
	vector<CAnimation*> m_vecActiveAnims; // 활성 애니메이션 목록

	//여러 종류의 Animation을 들고 있어야함. 
	//그 중에서도 현재 재생중인 애니메이션이 있겠지. 
private:
	unordered_map<wstring, CAnimation*> m_mapAnim;				//모든 Anim
	CAnimation* m_pCurAnim;								//현재 재생중인 Anim
	CObject* m_pOwner;									//Animator 소유 오브젝트
	bool								m_bRepeat;		//반복 재생 여부.


public:
	CObject* GetObj() { return m_pOwner; }
	size_t GetActiveAnimsCount() { return m_vecActiveAnims.size(); }
	vector<CAnimation*>& GetActiveAnims() { return m_vecActiveAnims; }

public:
	void CreateAnimation(const wstring& _animName, ID2D1Bitmap* _bitmap, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _Step
		, float _fDuration, UINT _iFrameCount, bool _bRepeat, Vec2 _offSet);

public:
	CAnimation* FindAnimation(const wstring& _strName);

public:
	void Play(const wstring& _animName, bool _bRepeat, int _iPriority);

public:
	// ... 기존 메서드들 ...
	void PauseAnimation(const wstring& _animName);
	void ResumeAnimation(const wstring& _animName);
	void PauseAllAnimations();
	void ResumeAllAnimations();

public:
	void update();
	void finalupdate();
	void render(ID2D1HwndRenderTarget* _pRender);


public:
	CAnimator();
	~CAnimator();

	friend class CObject;
	friend class CAnimation;
};

