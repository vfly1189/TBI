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

class MapMgr
{
	SINGLE(MapMgr);

private:
	vector<vector<UINT>> gridMap;
	vector<CellMap*> m_vecCellMaps;

	UINT m_iMapMaxWidth = 9;
	UINT m_iMapMaxHeight = 8;

	int startx;
	int starty;

private:
	int m_iCurLevel;

public:
	void GenerateBossRoom();
	void GenerateTreasureRoom();
	bool IsEndRoom(int x, int y);

public:
	void SetLevel(int _iLevel) { m_iCurLevel = _iLevel; }

public:
	int GetLevel() { return m_iCurLevel; }
	vector<vector<UINT>>& GetGridMap() { return gridMap; }
	vector<CellMap*>& GetCellMaps() { return m_vecCellMaps; }
	Vec2 GetStartPoint() { return { startx, starty }; }
	UINT GetMapMaxHeight() { return m_iMapMaxHeight; }
	UINT GetMapMaxWidth() { return m_iMapMaxWidth; }


public:
	void MapCutting();
	void DoorCutting();

public:
	void MapGenerate();
	int countNeighbors(int x, int y);
	void ShowMap();


public:
	void update();
	void finalupdate();
	void render(ID2D1HwndRenderTarget* _pRender);

public:
	void init();
	void clear();
};

