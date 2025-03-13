#pragma once


class CObject;
class CAnimation;

class CAnimator
{
	//���� ������ Animation�� ��� �־����. 
	//�� �߿����� ���� ������� �ִϸ��̼��� �ְ���. 

private:
	unordered_map<wstring, CAnimation*> m_mapAnim;		//��� Anim
	CAnimation* m_pCurAnim;								//���� ������� Anim
	CObject* m_pOwner;									//Animator ���� ������Ʈ
	bool								m_bRepeat;		//�ݺ� ��� ����.

public:
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _animName, ID2D1Bitmap* _bitmap, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _Step
		, float _fDuration, UINT _iFrameCount);

public:
	CAnimation* FindAnimation(const wstring& _strName);

public:
	void Play(const wstring& _animName, bool _bRepeat);

public:
	void update();
	void finalupdate();
	void render(ID2D1HwndRenderTarget* _pRender);

public:
	CAnimator();
	~CAnimator();

	friend class CObject;
};

