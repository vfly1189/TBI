#pragma once
/*
enum class ROOM_INFO
{
	EMPTY,
	START,
	NORMAL,
	SHOP,
	TREASURE,
	BOSS,
};*/

class CellMap;
class CBaby_Plum;

class MapMgr
{
	SINGLE(MapMgr);

private:
	vector<vector<UINT>> gridMap;
	//vector<CellMap*> m_vecCellMaps;
	//unordered_map<pair<int,int>, CellMap*> m_vecCellMaps;
	vector<vector<CellMap*>> m_vecCellMaps;
	vector<vector<vector<UINT>>> m_vecCellMapLayOuts;

	UINT m_iMapMaxWidth = 9;
	UINT m_iMapMaxHeight = 8;

	int startx;
	int starty;
	
	Vec2 m_vCurPos;
	Vec2 m_vBossPos; //gridpos

	CBaby_Plum* m_curFloorBoss;

private:
	int m_iCurLevel;


private:
	std::string LoadJsonFileInternal(const std::wstring& filePath);
	json ParseJsonStringInternal(const std::string& jsonString);
	void LoadMapLayoutsFromJSON(const json& jsonData);

public:
	void GenerateBossRoom();
	void GenerateTreasureRoom();
	bool IsEndRoom(int x, int y);

public:
	void SetLevel(int _iLevel) { m_iCurLevel = _iLevel; }
	void SetCurPos(Vec2 _vPos) { m_vCurPos = _vPos; }
	void SetBoss(CBaby_Plum* _boss) { m_curFloorBoss = _boss; }

public:
	int GetLevel() { return m_iCurLevel; }
	vector<vector<UINT>>& GetGridMap() { return gridMap; }
	//vector<CellMap*>& GetCellMaps() { return m_vecCellMaps; }
	CellMap* GetCellMap(Vec2 _vGridPos) { return m_vecCellMaps[(UINT)_vGridPos.y][(UINT)_vGridPos.x]; }
	vector<vector<CellMap*>>& GetAllCellMaps() { return m_vecCellMaps; }
	Vec2 GetStartPoint() { return { startx, starty }; }
	UINT GetMapMaxHeight() { return m_iMapMaxHeight; }
	UINT GetMapMaxWidth() { return m_iMapMaxWidth; }

	Vec2 GetCurPos() { return m_vCurPos; }

	Vec2 GetBossRoomGridPos() { return m_vBossPos; }
	CBaby_Plum* GetBoss() { return m_curFloorBoss; }
public:
	void MapCutting();
	void DoorCutting();

public:
	void MapGenerate();
	int  countNeighbors(int x, int y);
	void ShowMap();
	void SetEntities(vector<vector<UINT>>& _cellmap);
	void MakeMapLayOut();

public:
	void update();
	void finalupdate();
	void render(ID2D1HwndRenderTarget* _pRender);

public:
	void reset();

public:
	void init();
	void clear();
};

