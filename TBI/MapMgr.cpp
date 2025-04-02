#include "global.h"
#include "MapMgr.h"

#include "CellMap.h"

#include "Direct2DMgr.h"

MapMgr::MapMgr()
	: m_iCurLevel(1)
{

}

MapMgr::~MapMgr()
{

}

void MapMgr::MapGenerate()
{
	m_vecCellMaps.clear();

	int max_room = 8 + int(m_iCurLevel * 2.3);
	int min_room = 10;

	startx = std::rand() % m_iMapMaxWidth;
	starty = std::rand() % m_iMapMaxHeight;
	int room_count = 1;

	do
	{
		clear();
		room_count = 1;
		queue<std::pair<int, int>> cellQueue;

		gridMap[starty][startx] = (UINT)ROOM_INFO::START;
		cellQueue.push({ starty, startx });

		while (!cellQueue.empty() && room_count < max_room)
		{
			std::pair<int, int> cur = cellQueue.front();
			cellQueue.pop();

			// 상하좌우 방향
			vector<std::pair<int, int>> directions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
			std::shuffle(directions.begin(), directions.end(), std::mt19937{ std::random_device{}() });

			for (int i = 0; i < 4; i++)
			{
				int nx = cur.second + directions[i].second;
				int ny = cur.first + directions[i].first;

				if (nx < 0 || ny < 0 || nx >= (int)m_iMapMaxWidth || ny >= (int)m_iMapMaxHeight) continue;
				if (gridMap[ny][nx] != (UINT)ROOM_INFO::EMPTY) continue;

				// 인접 방 개수 제한
				int neighborCount = countNeighbors(nx, ny);
				if (neighborCount > 1) continue;

				// 확률적으로 방 생성
				if (std::rand() % 2 == 0 && room_count > min_room) continue;

				// 새로운 방 생성
				gridMap[ny][nx] = (UINT)ROOM_INFO::NORMAL;
				room_count++;
				cellQueue.push({ ny, nx });
			}
		}
	} while (room_count < min_room);

	GenerateBossRoom();
	GenerateTreasureRoom();

	int map_base_number = 0;
	if (m_iCurLevel == 1 || m_iCurLevel == 2) map_base_number = rand() % 2;
	else if (m_iCurLevel == 3 || m_iCurLevel == 4) map_base_number = rand() % 2 + 2;
	else if (m_iCurLevel == 5 || m_iCurLevel == 6) map_base_number = rand() % 2 + 4;

	vector<vector<bool>> m_vecCheck(m_iMapMaxHeight, vector<bool>(m_iMapMaxWidth, false));
	queue<pii> checkQueue;
	checkQueue.push({ starty, startx });
	m_vecCheck[starty][startx] = true;
	
	while (!checkQueue.empty())
	{
		pii curPos = checkQueue.front(); checkQueue.pop();

		CellMap* room = new CellMap(cellmap_base_sprite_tag[map_base_number]
			, Vec2((curPos.second - startx) * 960.f, (curPos.first - starty) * 540.f)
			, Vec2(curPos.second, curPos.first)
			, (ROOM_INFO)gridMap[curPos.first][curPos.second]);
		m_vecCellMaps.push_back(room);

		for (int i = 0; i < 4; i++)
		{
			int ny = curPos.first + dy4[i];
			int nx = curPos.second + dx4[i];

			if (ny < 0 || nx < 0 || ny >= (int)m_iMapMaxHeight || nx >= (int)m_iMapMaxWidth) continue;
			if (gridMap[ny][nx] == (UINT)ROOM_INFO::EMPTY) continue;
			if (m_vecCheck[ny][nx]) continue;

			m_vecCheck[ny][nx] = true;
			checkQueue.push({ ny,nx });
		}
	}
}

int MapMgr::countNeighbors(int x, int y)
{
	int neighbors_count = 0;
	
	// 상하좌우 방향
	vector<std::pair<int, int>> directions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	for (int i = 0; i < 4; i++)
	{
		int nx = x + directions[i].first;
		int ny = y + directions[i].second;

		if (nx < 0 || ny < 0 || nx >= (int)m_iMapMaxWidth || ny >= (int)m_iMapMaxHeight) continue;
		
		if (gridMap[ny][nx] != (UINT)ROOM_INFO::EMPTY) neighbors_count++;
	}

	return neighbors_count;
}

void MapMgr::MapCutting()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	ID2D1Bitmap* originalBitmap;
	ID2D1Bitmap* xFlip;
	ID2D1Bitmap* yFlip;
	ID2D1Bitmap* xyFlip;
	ID2D1Bitmap* combinedBitmap;
	vector<ID2D1Bitmap*> bitmaps;
	for (size_t i = 0; i < cellmap_base_sprite.size(); ++i)
	{
		bitmaps.clear();
		pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(cellmap_base_sprite[i]), cellmap_base_sprite_tag[i],
			D2D1::Point2F(0.f, 0.f), D2D1::Point2F(234.f, 156.f));

		originalBitmap = pD2DMgr->GetStoredBitmap(cellmap_base_sprite_tag[i]);
		xFlip = FlipBitamp(originalBitmap, true, false);
		yFlip = FlipBitamp(originalBitmap, false, true);
		xyFlip = FlipBitamp(originalBitmap, true, true);

		bitmaps.push_back(originalBitmap);
		bitmaps.push_back(xFlip);
		bitmaps.push_back(yFlip);
		bitmaps.push_back(xyFlip);

		combinedBitmap = CombineBitmaps2X2(bitmaps);
		pD2DMgr->StoreCreateMap(combinedBitmap, cellmap_base_sprite_tag[i]);
	}
}

void MapMgr::DoorCutting()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"door_01_normaldoor"), L"normal_door",
		D2D1::Point2F(9.f, 10.f), D2D1::Point2F(9.f + 49.f, 10.f + 33.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"door_01_normaldoor"), L"normal_door_open",
		D2D1::Point2F(65.f, 0.f), D2D1::Point2F(130.f, 51.f));



	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"door_10_bossroomdoor"), L"bossroom_door",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(0.f + 65.f, 00.f + 49.f));



	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"door_02_treasureroomdoor"), L"treasureroom_door",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(0.f + 65.f, 00.f + 49.f));

}

void MapMgr::ShowMap()
{
	for (size_t i = 0; i < m_iMapMaxHeight; i++)
	{
		for (size_t j = 0; j < m_iMapMaxWidth; j++)
		{
			if (gridMap[i][j] == -1) printf("0 ");
			else printf("%d ", gridMap[i][j]);
		}
		printf("\n");
	}
}

void MapMgr::GenerateBossRoom()
{
	std::pair<int, int> bossRoom;
	int maxDistance = -1;

	for (int y = 0; y < (int)m_iMapMaxHeight; ++y) 
	{
		for (int x = 0; x < (int)m_iMapMaxWidth; ++x) 
		{
			if (gridMap[y][x] != (UINT)ROOM_INFO::EMPTY && IsEndRoom(x, y)) {
				int distance = std::abs(x - startx) + std::abs(y - starty);
				if (distance > maxDistance) {
					maxDistance = distance;
					bossRoom = { x, y };
				}
			}
		}
	}
	gridMap[bossRoom.second][bossRoom.first] = (UINT)ROOM_INFO::BOSS; // 보스방 표시
}

void MapMgr::GenerateTreasureRoom()
{	
	std::pair<int, int> treasureRoom;
	int minDistance = 9999999;

	for (int y = 0; y < (int)m_iMapMaxHeight; ++y) 
	{
		for (int x = 0; x < (int)m_iMapMaxWidth; ++x) 
		{
			if (gridMap[y][x] != (UINT)ROOM_INFO::EMPTY && IsEndRoom(x, y) && gridMap[y][x] == (UINT)ROOM_INFO::NORMAL)
			{
				int distance = std::abs(x - startx) + std::abs(y - starty);
				if (distance < minDistance) {
					minDistance = distance;
					treasureRoom = { x, y };
				}
			}
		}
	}
	gridMap[treasureRoom.second][treasureRoom.first] = (UINT)ROOM_INFO::TREASURE; // 보스방 표시
}

void MapMgr::init()
{
	gridMap.resize(m_iMapMaxHeight);
	for (size_t i = 0; i < m_iMapMaxHeight; i++)
		gridMap[i].resize(m_iMapMaxWidth);

	clear();

	MapCutting();
	DoorCutting();
}

bool MapMgr::IsEndRoom(int x, int y)
{
	return countNeighbors(x, y) == 1;
}

void MapMgr::clear()
{
	for (size_t i = 0; i < m_iMapMaxHeight; i++)
	{
		for (size_t j = 0; j < m_iMapMaxWidth; j++)
		{
			gridMap[i][j] = 0;
		}
	}
}

void MapMgr::render(ID2D1HwndRenderTarget* _pRender)
{
	
	for (size_t i = 0; i < m_vecCellMaps.size(); ++i)
	{
		//printf("i : %d\n",i);
		m_vecCellMaps[i]->render(_pRender);
	}
}

void MapMgr::update()
{
	for (size_t i = 0; i < m_vecCellMaps.size(); ++i)
	{
		//printf("i : %d\n",i);
		m_vecCellMaps[i]->update();
	}
}

void MapMgr::finalupdate()
{
	for (size_t i = 0; i < m_vecCellMaps.size(); ++i)
	{
		//printf("i : %d\n",i);
		m_vecCellMaps[i]->finalupdate();
	}
}
