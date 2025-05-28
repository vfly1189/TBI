#include "global.h"
#include "CFileMgr.h"
#include "CPathMgr.h"
#include "CSoundMgr.h"
#include "Direct2DMgr.h"

CFileMgr::CFileMgr() = default;
CFileMgr::~CFileMgr() = default;

void CFileMgr::init(const wstring& folderPath)
{
    // �־��� ���� ��ο��� Ž�� ����
    SearchFolder(folderPath);
}

void CFileMgr::SearchFolder(const wstring& folderPath)
{
    WIN32_FIND_DATA findData;
    wstring searchPath = folderPath + L"\\*";

    // ù ��° ����/���͸� �˻�
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        wprintf(L"Failed to open directory: %s\n", folderPath.c_str());
        return;
    }

    // ���� ���� �̸� �̸� ��� (�ݺ��� ������ �ݺ� ��� ����)
    wstring curFolderName = GetLastFolderName(folderPath);
    bool isCollectiblesFolder = (curFolderName == L"collectibles");

    do
    {
        // �̸��� "." �Ǵ� ".."�� Ư�� ���丮 �ǳʶٱ�
        if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
            continue;

        wstring fullPath = folderPath + L"\\" + findData.cFileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // ���� ���͸��� ��������� Ž��
            SearchFolder(fullPath);
        }
        else
        {
            // collectibles ������ ������ �̸� ����
            if (isCollectiblesFolder)
            {
                wstring tag = GetFileNameWithoutExtension(findData.cFileName);
                wstring itemName = GetLastFileName(tag);
                items_tags.push_back(itemName);
            }

            // ���� ó��
            ProcessFile(fullPath);
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}

void CFileMgr::ProcessFile(const std::wstring& filePath)
{
    // ���� �̸��� Ȯ���� ����
    size_t lastSlashPos = filePath.find_last_of(L"\\/");
    wstring fileName = (lastSlashPos == wstring::npos) ? filePath : filePath.substr(lastSlashPos + 1);
    size_t dotPos = fileName.find_last_of(L'.');
    if (dotPos == wstring::npos) return; // Ȯ���ڰ� ���� ������ ó������ ����

    wstring extension = fileName.substr(dotPos + 1);
    wstring tag = fileName.substr(0, dotPos);
    wstring relativePath = CPathMgr::GetInstance()->GetRelativePath(filePath.c_str());

    // Ȯ���ڿ� ���� ó��
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