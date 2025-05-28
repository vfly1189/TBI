#include "global.h"
#include "CFileMgr.h"
#include "CPathMgr.h"
#include "CSoundMgr.h"
#include "Direct2DMgr.h"

CFileMgr::CFileMgr() = default;
CFileMgr::~CFileMgr() = default;

void CFileMgr::init(const wstring& folderPath)
{
    // 주어진 폴더 경로에서 탐색 시작
    SearchFolder(folderPath);
}

void CFileMgr::SearchFolder(const wstring& folderPath)
{
    WIN32_FIND_DATA findData;
    wstring searchPath = folderPath + L"\\*";

    // 첫 번째 파일/디렉터리 검색
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        wprintf(L"Failed to open directory: %s\n", folderPath.c_str());
        return;
    }

    // 현재 폴더 이름 미리 계산 (반복문 내에서 반복 계산 방지)
    wstring curFolderName = GetLastFolderName(folderPath);
    bool isCollectiblesFolder = (curFolderName == L"collectibles");

    do
    {
        // 이름이 "." 또는 ".."인 특수 디렉토리 건너뛰기
        if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
            continue;

        wstring fullPath = folderPath + L"\\" + findData.cFileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // 하위 디렉터리를 재귀적으로 탐색
            SearchFolder(fullPath);
        }
        else
        {
            // collectibles 폴더의 아이템 이름 추출
            if (isCollectiblesFolder)
            {
                wstring tag = GetFileNameWithoutExtension(findData.cFileName);
                wstring itemName = GetLastFileName(tag);
                items_tags.push_back(itemName);
            }

            // 파일 처리
            ProcessFile(fullPath);
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}

void CFileMgr::ProcessFile(const std::wstring& filePath)
{
    // 파일 이름과 확장자 추출
    size_t lastSlashPos = filePath.find_last_of(L"\\/");
    wstring fileName = (lastSlashPos == wstring::npos) ? filePath : filePath.substr(lastSlashPos + 1);
    size_t dotPos = fileName.find_last_of(L'.');
    if (dotPos == wstring::npos) return; // 확장자가 없는 파일은 처리하지 않음

    wstring extension = fileName.substr(dotPos + 1);
    wstring tag = fileName.substr(0, dotPos);
    wstring relativePath = CPathMgr::GetInstance()->GetRelativePath(filePath.c_str());

    // 확장자에 따라 처리
    if (extension == L"png")
    {
        Direct2DMgr::GetInstance()->LoadAndStoreBitmap(relativePath.c_str(), tag.c_str(), false);
        wprintf(L"Loaded PNG: %s\n", tag.c_str());
    }
    else if (extension == L"mp3" || extension == L"wav" || extension == L"ogg")
    {
        bool isLoop = (tag == L"genesis_retake_light_loop");
        bool isBackgroundSound = isLoop || (tag == L"insect swarm");

        CSoundMgr::GetInstance()->AddSound(tag, relativePath.c_str(), isLoop, isBackgroundSound);

        wprintf(L"Loaded sound: %s (%s)\n", tag.c_str(),
            isBackgroundSound ? (isLoop ? L"BGM" : L"SFX") : L"Sound");
    }
}

inline wstring CFileMgr::GetFileNameWithoutExtension(const wstring& fileName)
{
    size_t dotPos = fileName.find_last_of(L'.');
    return (dotPos != wstring::npos) ? fileName.substr(0, dotPos) : fileName;
}

inline wstring CFileMgr::GetLastFolderName(const std::wstring& path)
{
    size_t lastBackslash = path.find_last_of(L"\\");
    return (lastBackslash != std::wstring::npos) ? path.substr(lastBackslash + 1) : path;
}

inline wstring CFileMgr::GetLastFileName(const std::wstring& path)
{
    size_t lastUnderscore = path.find_last_of(L"_");
    return (lastUnderscore != std::wstring::npos) ? path.substr(lastUnderscore + 1) : path;
}