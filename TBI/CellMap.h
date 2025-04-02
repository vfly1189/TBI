#pragma once

class CSpriteUI;
class CObject;
class CDoor;
class CWall;

class CellMap
{
private:
	Vec2				m_vMapPos;					//실제 위치
	Vec2				m_vGridMapPos;
	ROOM_INFO			m_eRoomType;

	CSpriteUI*			cellMap;
	vector<CObject*>	m_vecObjects;
	vector<CDoor*>		m_vecDoors;
	vector<bool>		m_vecDoorExist;

	vector<CWall*>		m_vecWalls;

	bool				m_bIsClear;

public:
	void SetPos(Vec2 _vPos) { m_vMapPos = _vPos; }
	void SetRoomType(ROOM_INFO _eType) { m_eRoomType = _eType; }
	
public:
	Vec2 GetPos() { return m_vMapPos; }

	CSpriteUI* GetCellMap() { return cellMap; }
	vector<CObject*>& GetObjects() { return m_vecObjects; }
	vector<CDoor*>& GetCellMapDoors() { return m_vecDoors; }
	vector<CWall*>& GetWalls() { return m_vecWalls; }
	ROOM_INFO GetRoomType() { return m_eRoomType; }


public:
	void update();
	void finalupdate();
	void render(ID2D1HwndRenderTarget* _pRender);

public:
	CellMap(wstring _strMapBaseSprite, Vec2 _vRealPos, Vec2 _vGridPos, ROOM_INFO _eRoomType);
	~CellMap();
};

