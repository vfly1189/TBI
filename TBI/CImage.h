#pragma once

class CObject;
struct ID2D1Bitmap;

//�� ������Ʈ�� Direct2D�θ� �۵��մϴ�. 
class CImage
{

private:
	CObject*			m_pOwner;		//image�� �����ϰ� �ִ� ������Ʈ. 
	ID2D1Bitmap*		m_pBitmap;

	Vec2				m_vOffSet;
	Vec2				m_vFinalPos;

	//������ Ŀ���͸���¡��
	bool				m_bScaleCustom;
	Vec2				m_vScale;//0~1

	float				m_fRatio;

public:
	void ScaleCustomizing(Vec2 _vScale, bool _bScaleCustom)
	{
		m_vScale = _vScale;
		m_bScaleCustom = _bScaleCustom;
	}

	CObject* GetObj() { return m_pOwner; }
	ID2D1Bitmap* GetBitmap() { return m_pBitmap; }
	Vec2 GetFinalPos() { return m_vFinalPos; }
	void SetBitmap(ID2D1Bitmap* _bitmap) { m_pBitmap = _bitmap; }

	void SetRatio(float _ratio) { m_fRatio = _ratio; }
	void SetOffset(Vec2 _vOffset) { m_vOffSet = _vOffset; }

public:
	void finalupdate();
	virtual void render(ID2D1HwndRenderTarget* _renderTarget);

public:
	CImage();
	~CImage();

public:
	friend class CObject;
};

