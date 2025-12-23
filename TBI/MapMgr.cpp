#include "global.h"
#include "MapMgr.h"

#include "CellMap.h"

#include "Direct2DMgr.h"
#include "CCamera.h"
#include "CPathMgr.h"

#include <stack>

#include <limits>


MapMgr::MapMgr()
	: m_iCurLevel(1)
	, m_vCurPos{4,3}
{

}

MapMgr::~MapMgr()
{
	reset();
}

void MapMgr::MapGenerate()
{
	//m_vecCellMaps.clear();

	int max_room = 8 + int(m_iCurLevel * 2.3);
	int min_room = 10;

	// 시작지점 임의 생성
	std::uniform_int_distribution<int> dist_x(0, m_iMapMaxWidth - 1);
	std::uniform_int_distribution<int> dist_y(0, m_iMapMaxHeight - 1);
	startx = dist_x(rng);
	starty = dist_y(rng);
	m_vCurPos = Vec2(startx, starty);

	// 맵 생성 루프
	int room_count;
	do {
		clear();
		room_count = 1;

		queue<std::pair<int, int>> cellQueue;
		gridMap[starty][startx] = static_cast<UINT>(ROOM_INFO::START);
		cellQueue.push({ starty, startx });

		// 방향성
		const std::pair<int, int> directions[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
		std::vector<int> dir_indices = { 0, 1, 2, 3 };

		while (!cellQueue.empty() && room_count < max_room) {
			std::pair<int, int> cur = cellQueue.front();
			cellQueue.pop();

			// 방향 정보 랜덤하게 섞음
			std::shuffle(dir_indices.begin(), dir_indices.end(), rng);

			for (int i = 0; i < 4; i++) {
				const auto& dir = directions[dir_indices[i]];
				int nx = cur.second + dir.second;
				int ny = cur.first + dir.first;

				// 유효성 검사
				if (nx < 0 || ny < 0 || nx >= static_cast<int>(m_iMapMaxWidth) ||
					ny >= static_cast<int>(m_iMapMaxHeight) ||
					gridMap[ny][nx] != static_cast<UINT>(ROOM_INFO::EMPTY)) {
					continue;
				}

				// 강제로 이웃방의 갯수가 1개이하로 만듬
				if (countNeighbors(nx, ny) > 1) continue;

				
				gridMap[ny][nx] = static_cast<UINT>(ROOM_INFO::NORMAL);
				room_count++;
				cellQueue.push({ ny, nx });
			}
		}
	} while (room_count < min_room);

	//특수방 생성
	GenerateBossRoom();
	GenerateTreasureRoom();

	// 레벨에 따라 맵 배경 결정
	int map_base_number = 0;
	if (m_iCurLevel <= 2) map_base_number = rng() % 2;
	else if (m_iCurLevel <= 4) map_base_number = rng() % 2 + 2;
	else if (m_iCurLevel <= 6) map_base_number = rng() % 2 + 4;

	// BFS로 만든걸 이제 객체 생성
	std::vector<std::vector<bool>> visited(m_iMapMaxHeight, std::vector<bool>(m_iMapMaxWidth, false));
	queue<pii> checkQueue;
	checkQueue.push({ starty, startx });
	visited[starty][startx] = true;

	static const int dy4[4] = { -1, 1, 0, 0 };
	static const int dx4[4] = { 0, 0, -1, 1 };

	while (!checkQueue.empty()) {
		pii curPos = checkQueue.front();
		checkQueue.pop();

		ROOM_INFO roomType = static_cast<ROOM_INFO>(gridMap[curPos.first][curPos.second]);
		Vec2 roomPos = Vec2((curPos.second - startx) * 960.f, (curPos.first - starty) * 540.f);
		Vec2 gridPos = Vec2(curPos.second, curPos.first);

		m_vecCellMaps[curPos.first][curPos.second] =
			new CellMap(cellmap_base_sprite_tag[map_base_number], roomPos, gridPos, roomType);

		for (int i = 0; i < 4; i++) {
			int ny = curPos.first + dy4[i];
			int nx = curPos.second + dx4[i];

			if (ny < 0 || nx < 0 || ny >= static_cast<int>(m_iMapMaxHeight) ||
				nx >= static_cast<int>(m_iMapMaxWidth) ||
				gridMap[ny][nx] == static_cast<UINT>(ROOM_INFO::EMPTY) ||
				visited[ny][nx]) {
				continue;
			}

			visited[ny][nx] = true;
			checkQueue.push({ ny, nx });
		}
	}
}
int MapMgr::countNeighbors(int x, int y)
{
	static const std::pair<int, int> directions[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	int neighbors_count = 0;
	for (int i = 0; i < 4; i++) {
		int nx = x + directions[i].first;
		int ny = y + directions[i].second;

		if (nx < 0 || ny < 0 || nx >= static_cast<int>(m_iMapMaxWidth) ||
			ny >= static_cast<int>(m_iMapMaxHeight)) {
			continue;
		}

		if (gridMap[ny][nx] != static_cast<UINT>(ROOM_INFO::EMPTY)) {
			neighbors_count++;
		}
	}

	return neighbors_count;
}

void MapMgr::MapCutting()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	for (size_t i = 0; i < cellmap_base_sprite.size(); ++i) {
	
		pD2DMgr->SplitBitmap(
			pD2DMgr->GetStoredBitmap(cellmap_base_sprite[i]),
			cellmap_base_sprite_tag[i],
			D2D1::Point2F(0.f, 0.f),
			D2D1::Point2F(234.f, 156.f)
		);

		ID2D1Bitmap* originalBitmap = pD2DMgr->GetStoredBitmap(cellmap_base_sprite_tag[i]);

		
		std::vector<ID2D1Bitmap*> bitmaps;
		bitmaps.reserve(4); // Pre-allocate for performance

		bitmaps.push_back(originalBitmap);
		bitmaps.push_back(FlipBitmap(originalBitmap, true, false));  // x-flip
		bitmaps.push_back(FlipBitmap(originalBitmap, false, true));  // y-flip
		bitmaps.push_back(FlipBitmap(originalBitmap, true, true));   // xy-flip

		ID2D1Bitmap* combinedBitmap = CombineBitmaps2X2(bitmaps);
		pD2DMgr->DeleteBitmap(cellmap_base_sprite_tag[i]);
		pD2DMgr->StoreCreateMap(combinedBitmap, cellmap_base_sprite_tag[i]);
	}
}

void MapMgr::DoorCutting()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	
	struct DoorCutInfo {
		std::wstring sourceName;
		std::wstring targetPrefix;
	};


	const DoorCutInfo doorTypes[] = {
		{L"door_01_normaldoor", L"normal_door"},
		{L"door_10_bossroomdoor", L"bossroom_door"},
		{L"door_02_treasureroomdoor", L"treasureroom_door"}
	};

	
	const float frameWidth = 64.0f;
	const float frameHeight = 48.0f;


	for (const auto& doorInfo : doorTypes) {
		ID2D1Bitmap* doorBitmap = pD2DMgr->GetStoredBitmap(doorInfo.sourceName);


		pD2DMgr->SplitBitmap(doorBitmap, doorInfo.targetPrefix,
			D2D1::Point2F(0.0f, 0.0f),
			D2D1::Point2F(frameWidth, frameHeight));

		pD2DMgr->SplitBitmap(doorBitmap, doorInfo.targetPrefix + L"_open",
			D2D1::Point2F(frameWidth, 0.0f),
			D2D1::Point2F(frameWidth * 2.0f, frameHeight));

		pD2DMgr->SplitBitmap(doorBitmap, doorInfo.targetPrefix + L"_close_left",
			D2D1::Point2F(0.0f, frameHeight),
			D2D1::Point2F(frameWidth, frameHeight * 2.0f));

		pD2DMgr->SplitBitmap(doorBitmap, doorInfo.targetPrefix + L"_close_right",
			D2D1::Point2F(frameWidth, frameHeight),
			D2D1::Point2F(frameWidth * 2.0f, frameHeight * 2.0f));
	}
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

void MapMgr::SetEntities(vector<vector<UINT>>& _cellmap)
{
	
	int randRoom = rand() % m_vecCellMapLayOuts.size();
	_cellmap = m_vecCellMapLayOuts[randRoom];
}

void MapMgr::GenerateBossRoom()
{

	std::pair<int, int> bossRoom;
	int maxDistance = -1;

	for (int y = 0; y < static_cast<int>(m_iMapMaxHeight); ++y) {
		for (int x = 0; x < static_cast<int>(m_iMapMaxWidth); ++x) {
			
			if (gridMap[y][x] != static_cast<UINT>(ROOM_INFO::EMPTY) && IsEndRoom(x, y)) {
	
				int distance = std::abs(x - startx) + std::abs(y - starty);
				if (distance > maxDistance) {
					maxDistance = distance;
					bossRoom = { x, y };
				}
			}
		}
	}

	
	gridMap[bossRoom.second][bossRoom.first] = static_cast<UINT>(ROOM_INFO::BOSS);
	m_vBossPos = Vec2(bossRoom.first, bossRoom.second);
}

void MapMgr::GenerateTreasureRoom()
{	

	std::pair<int, int> treasureRoom;
	int minDistance = INT_MAX; // 수정: std::numeric_limits<int>::max() 대신 INT_MAX 사용

	for (int y = 0; y < static_cast<int>(m_iMapMaxHeight); ++y) {
		for (int x = 0; x < static_cast<int>(m_iMapMaxWidth); ++x) {
			
			if (gridMap[y][x] == static_cast<UINT>(ROOM_INFO::NORMAL) && IsEndRoom(x, y)) {
		
				int distance = std::abs(x - startx) + std::abs(y - starty);
				if (distance < minDistance) {
					minDistance = distance;
					treasureRoom = { x, y };
				}
			}
		}
	}

	
	gridMap[treasureRoom.second][treasureRoom.first] = static_cast<UINT>(ROOM_INFO::TREASURE);
}

void MapMgr::init()
{

	gridMap.resize(m_iMapMaxHeight, std::vector<UINT>(m_iMapMaxWidth, 0));
	m_vecCellMaps.resize(m_iMapMaxHeight, std::vector<CellMap*>(m_iMapMaxWidth, nullptr));

	
	wstring contentPath = CPathMgr::GetInstance()->GetContentPath();
	wstring jsonFilePath = contentPath + L"\\json\\map_layout.json";

	std::string jsonString = LoadJsonFileInternal(jsonFilePath);
	if (!jsonString.empty()) {
		json jsonData = ParseJsonStringInternal(jsonString);
		if (!jsonData.is_null()) {
			LoadMapLayoutsFromJSON(jsonData);
		}
	}

	MapCutting();
	DoorCutting();
}

bool MapMgr::IsEndRoom(int x, int y)
{
	return countNeighbors(x, y) == 1;
}

void MapMgr::clear()
{

	for (size_t i = 0; i < m_iMapMaxHeight; i++) {
		for (size_t j = 0; j < m_iMapMaxWidth; j++) {
			gridMap[i][j] = static_cast<UINT>(ROOM_INFO::EMPTY);

			
			if (m_vecCellMaps[i][j] != nullptr) {
				delete m_vecCellMaps[i][j];
				m_vecCellMaps[i][j] = nullptr;
			}
		}
	}
}

void MapMgr::render(ID2D1HwndRenderTarget* _pRender)
{
	/*
	for (size_t i = 0; i < m_vecCellMaps.size(); ++i)
	{
		m_vecCellMaps[i]->render(_pRender);
	}
	*/
}

void MapMgr::reset()
{
	
	for (size_t i = 0; i < m_iMapMaxHeight; i++) {
		for (size_t j = 0; j < m_iMapMaxWidth; j++) {
			gridMap[i][j] = static_cast<UINT>(ROOM_INFO::EMPTY);


			delete m_vecCellMaps[i][j];
			m_vecCellMaps[i][j] = nullptr;
		}
	}

	
	m_vCurPos = Vec2(4, 3);
}

void MapMgr::update()
{
	/*
	for (size_t i = 0; i < m_vecCellMaps.size(); ++i)
	{
		m_vecCellMaps[i]->update();
	}*/
}

void MapMgr::finalupdate()
{
	/*
	for (size_t i = 0; i < m_vecCellMaps.size(); ++i)
	{
		//printf("i : %d\n",i);
		m_vecCellMaps[i]->finalupdate();
	}
	*/
}


std::string MapMgr::LoadJsonFileInternal(const std::wstring& filePath) {
	std::ifstream file(filePath);
	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
	else {
		MessageBox(nullptr, L"JSON 파일 로드 실패!", L"오류", MB_OK | MB_ICONERROR);
		return "";
	}
}

json MapMgr::ParseJsonStringInternal(const std::string& jsonString) {
	try {
		return json::parse(jsonString);
	}
	catch (const json::parse_error& e) {
		std::wstring errorMessage = L"JSON 파싱 오류: ";
		errorMessage += std::wstring(e.what(), e.what() + strlen(e.what()));
		MessageBox(nullptr, errorMessage.c_str(), L"오류", MB_OK | MB_ICONERROR);
		return json();
	}
}

void MapMgr::LoadMapLayoutsFromJSON(const json& jsonData)
{
	if (jsonData.is_array()) {
		m_vecCellMapLayOuts.resize(jsonData.size());
		for (size_t i = 0; i < jsonData.size(); ++i) {
			if (jsonData[i].is_object()) {
				int width = jsonData[i].value("width", 0);
				int height = jsonData[i].value("height", 0);
				m_vecCellMapLayOuts[i].resize(height);
				printf("width : %d, height : %d\n", width, height);
				for (size_t y = 0; y < height; ++y) {
					m_vecCellMapLayOuts[i][y].resize(width, 0);
				}

				if (jsonData[i].contains("entities") && jsonData[i]["entities"].is_array()) {
					for (const auto& entity : jsonData[i]["entities"]) {
						if (entity.is_object()) {
							int x = entity.value("x", 0);
							int y = entity.value("y", 0);
							std::string typeStr = entity.value("type", "");
							ENTITY_TYPE type = ENTITY_TYPE::EMPTY;
							if (typeStr == "ROCK") type = ENTITY_TYPE::ROCK;
							else if (typeStr == "FLY") type = ENTITY_TYPE::FLY;
							else if (typeStr == "HORF") type = ENTITY_TYPE::HORF;
							// 필요에 따라 다른 엔티티 타입 처리 추가

							if (y < height && x < width) {
								m_vecCellMapLayOuts[i][y][x] = static_cast<UINT>(type);
							}
						}
					}
				}
			}
		}
	}
}