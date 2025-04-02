#pragma once

class CObject;

class CRigidBody
{
private:
	CObject*	m_pOwner;		//collider를 소유하고 있는 오브젝트. 

	Vec2		m_vForce;		//크기, 방향. 
	Vec2		m_vAccel;		//가속도. 
	Vec2		m_vAccelA;		//가속도 추가. 
	Vec2		m_vVelocity;	//속도. (크기 : 속력, 방향)
	Vec2		m_vMaxVelocity;	//최대 속력. 

	float		m_fMass;		//강체의 질량. 
	float		m_fFricCoeff;	//마찰 계수. 

public:
	void finalupdate();

public:
	void AddForce(Vec2 _vF) { m_vForce += _vF; }
	void SetMass(float _fMass) { m_fMass = _fMass; }
	float GetMass() { return m_fMass; }
	float GetSpeed() { return m_vVelocity.Length(); }
	Vec2 GetVelocity() { return m_vVelocity; }
	Vec2 GetMaxVelocity() { return m_vMaxVelocity; }

	void SetVelocity(Vec2 _v) { m_vVelocity = _v; }
	void AddVelocity(Vec2 _v) { m_vVelocity += _v; }
	void SetMaxVelocity(Vec2 _vVelocity) { m_vMaxVelocity = _vVelocity; }
	void SetAccelAlpha(Vec2 _vAccel) { m_vAccelA = _vAccel; }

private:
	void Move();


public:
	CRigidBody();
	~CRigidBody();

public:
	friend class CObject;
};

