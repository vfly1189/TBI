#pragma once

class CUI;
class CUIMgr
{
	SINGLE(CUIMgr)

private:
	CUI* m_pFocusedUI;

public:
	void update();

	//�ش� ui�� Ȱ��ȭ �Ǹ鼭 �ڽ��� ����� ����. 
	void SetFocusedUI(CUI* _pUI);

private:
	CUI* GetFocusedUI();						//���� ��Ŀ�� �� ui
	CUI* GetTargetedUI(CUI* _pParentUI);		//�θ� UI������ ������ Ÿ���� �� ui�� ã�Ƽ� ��ȯ�Ѵ�. 
};

