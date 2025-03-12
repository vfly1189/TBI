#include "global.h"

#include "CObject.h"
#include "CScene.h"

#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CCamera.h"
#include "CCore.h"
#include "Direct2DMgr.h"



bool CScene::isPause = false;
const float tile_Devide = 0.15625;

CScene::CScene()

	: m_iTileX(0)
	, m_iTileY(0)
	, m_pPlayer(nullptr)
{
}


CScene::~CScene()
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {
			//m_arrObj[그룹][물체] 삭제. 
			delete m_arrObj[typeIDX][objIDX];
		}
		//씬이 사라지면, 그 씬의 벡터들도 다 사라짐. 
		//STL의 RAII가 알아서 삭제하기 때문. 
	}
}


void CScene::start()
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {
			m_arrObj[typeIDX][objIDX]->start();
		}
	}
}

void CScene::update()
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {
			if (!m_arrObj[typeIDX][objIDX]->IsDead()) {
				m_arrObj[typeIDX][objIDX]->update();
			}
		}
	}
}
//움직이고 했던 걸, 마지막으로 업데이트 함. 
//충돌체가 플레이어 따라가게 함, 충돌 처리. 
void CScene::finalupdate()
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {

			//Final Update는 돌려줌. 내부적으로 Component들의 마무리 단계 업데이트(충돌처리나, 참조관계등)
			m_arrObj[typeIDX][objIDX]->finalupdate();
		}
	}
}

void CScene::render(ID2D1HwndRenderTarget* _pRender)
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		auto ObjVecIter = m_arrObj[typeIDX].begin();

		for (; ObjVecIter != m_arrObj[typeIDX].end();) {
			if (!(*ObjVecIter)->IsDead()) {
				(*ObjVecIter)->render(_pRender);
				ObjVecIter++;
			}
			else {
				//Dead상태일 경우엔 렌더링에서 삭제하기. 
				ObjVecIter = m_arrObj[typeIDX].erase(ObjVecIter);
			}

		}
	}
}


void CScene::DeleteGroup(GROUP_TYPE _eGroup)
{
	Safe_Delete_Vec<CObject*>(m_arrObj[(UINT)_eGroup]);
}

void CScene::DeleteAll()
{
	for (UINT GroupIdx = 0; GroupIdx < (UINT)GROUP_TYPE::END; GroupIdx++) {
		if (GroupIdx == (UINT)GROUP_TYPE::PLAYER)
		{
			m_arrObj[(UINT)GROUP_TYPE::PLAYER].clear();
			continue;
		}
		else if (GroupIdx == (UINT)GROUP_TYPE::WEAPON) {
			m_arrObj[(UINT)GROUP_TYPE::WEAPON].clear();
		}
		DeleteGroup((GROUP_TYPE)GroupIdx);
	}
}
