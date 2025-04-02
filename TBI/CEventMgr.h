#pragma once


struct tEvent {
	EVENT_TYPE		eEven;
	DWORD_PTR		lParam;
	DWORD_PTR		wParam;
};

//�浹�� ���������, ���� �浹�� Ȱ���Ϸ��� �ϸ�. ������ ������Ű�� �ʿ��ѵ�.
//EX) Scene���� -> �������� �̹� �� �����ƴµ�?

//� �̺�Ʈ�� �߻��ϸ�, �ش� �������� ��ΰ� ����Ǿ�� �Ѵ�. 
//���� ����ε��� �ٸ��� �޾Ƶ鿩������ ���� <- � ���, �̺�Ʈ���� ������ ������ ����ȭ ����� ���� ������??

//�ٷ� ������ ���� -> ����ó���� ���ؼ� ���� �����ӿ� ��� �����ų ��. 

class CEventMgr
{
	SINGLE(CEventMgr)

private:
	vector<tEvent> m_vecEvent;
	unordered_set<CObject*> m_setDeadScheduled;		//���� ������ ������Ʈ��. 
public:
	void update();


private:
	void Excute(const tEvent& _eve);


public:
	void AddEvent(const tEvent& _eve) {
		m_vecEvent.push_back(_eve);
	}

	//������ �ϸ�, ���� �����ӿ��� ���� ����. 
	//������ ������Ʈ�� ���� �˰��ִ� ������Ʈ�� �°� ���� �ƴ��� ��� �� ����� ����. 
	//�׷��� �� �༮�� ������ �Ŷ�� Ʈ���Ÿ� on��Ŵ. �� ������Ʈ�� Dead���°� ��. 
	//���� �����ӿ����� Dead���·� �����ؼ�, �����ϴ� ������Ʈ���� �� �� �װڱ��� �� �� ����. 

	//�� ���� �����ӿ� ���� ������ �ֵ��� ������Ŵ. 
};

