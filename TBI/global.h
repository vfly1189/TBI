#pragma once

#include <Windows.h>
#include <ctime>
#include <iostream>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite_3.h>
#include <dwrite.h>
#pragma comment(lib, "Dwrite.lib")
#include <wincodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#include <wrl.h>
using namespace Microsoft::WRL;
using namespace std;

#include <array>
using std::array;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <set>
#include <unordered_set>
using std::set;
using std::unordered_set;

#include <map>
using std::map;
using std::make_pair;

#include <unordered_map>
using std::unordered_map;

#include <queue>
using std::queue;

#include <string>
#include <string_view>
using std::string;
using std::wstring;

#include <array>

#include <random>
std::random_device random_generator;
std::uniform_int_distribution<int> random_distribution(0, 100);

#include <math.h>
#include <assert.h>

#pragma comment(lib, "Msimg32.lib")

#include <codecvt>
#include <locale>
 
#include <sstream>
#include <iomanip>

#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib, "fmod_vc.lib")

#include <gdiplus.h>
#include <gdiplusheaders.h>
#include <gdiplusflat.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#include <utility>
#include <algorithm>
#include <functional>

#include "define.h"
#include "struct.h"
#include "func.h"
#include "header_pos.h"
#include "names_string.h"
#include "character_stats.h"
#include "item_info.h"

#include "json.hpp"
using json = nlohmann::json;

#include <fstream>
#include <sstream>

std::random_device rd;
std::mt19937 rng(rd());

