#include "global.h"
#include "CRigidBody.h"

#include "CObject.h"

#include "CTimeMgr.h"


CRigidBody::CRigidBody()
	: m_pOwner(nullptr)
	, m_fMass(1.f)
	, m_fFricCoeff(3000.f)
	, m_vMaxVelocity(Vec2(500.f, 500.f))
{

}

CRigidBody::~CRigidBody()
{

}

void CRigidBody::finalupdate()
{
	//printf("�׽�Ʈ : %f %f\n", m_vMaxVelocity.x, m_vMaxVelocity.y);


	float fForce = m_vForce.Length();		//���� ũ��

	if (0.f != fForce) {
		m_vForce.Normalize();

		float m_fAccel = fForce / m_fMass;		//�������� ���ӵ�. 
		m_vAccel = m_vForce * m_fAccel;
	}

	m_vAccel += m_vAccelA;						//�߰� ���ӵ�
	m_vVelocity += m_vAccel * fDT;				//�ӵ�. 

	//�����¿� ���� �ݴ���������� ���ӵ� ����.

	if (!m_vVelocity.IsZero()) {
		Vec2 vFircDir = -m_vVelocity;
		vFircDir.Normalize();
		Vec2 vFriction = vFircDir * m_fFricCoeff * fDT;

		if (m_vVelocity.Length() <= vFriction.Length()) {
			//���� ���ӵ��� ���� �ӵ����� �� ū ���.
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else {
			m_vVelocity += vFriction;
		}
	}


	//�ӵ� ���� �˻�. 
	//�� ���� ���� �˻��ؾ���. 

	//X�� �ӵ� ���� �˻�. 
	if (m_vMaxVelocity.x < abs(m_vVelocity.x)) {
		m_vVelocity.x /= abs(m_vVelocity.x);
		m_vVelocity.x *= m_vMaxVelocity.x;
	}

	//Y�� �ӵ� ���� �˻�. 
	if (m_vMaxVelocity.y < abs(m_vVelocity.y)) {
		m_vVelocity.y /= abs(m_vVelocity.y);
		m_vVelocity.y *= m_vMaxVelocity.y;
	}
	//�ӵ��� ���� �̵�. 
	Move();

	//�� �ʱ�ȭ. 
	m_vForce = Vec2(0.f, 0.f);

	//���ӵ� �ʱ�ȭ
	m_vAccel = Vec2(0.f, 0.f);

	//�߰� ���ӵ� �ʱ�ȭ
	m_vAccelA = Vec2(0.f, 0.f);
}

void CRigidBody::Move()
{
	float fSpeed = m_vVelocity.Length();

	if (0.f != fSpeed) {
		Vec2 vPos = m_pOwner->GetPos();
		vPos += m_vVelocity * fDT;

		m_pOwner->SetPos(vPos);
	}
}