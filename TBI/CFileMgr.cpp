#include "global.h"
#include "CFileMgr.h"
#include "CPathMgr.h"
#include "CSoundMgr.h"
#include "Direct2DMgr.h"

CFileMgr::CFileMgr()
{

}

CFileMgr::~CFileMgr()
{

}

void CFileMgr::init(const wstring& folderPath)
{
    // �־��� ���� ��ο��� Ž�� ����
    SearchFolder(folderPath);
}

void CFileMgr::SearchFolder(const wstring& folderPath)
{
    WIN32_FIND_DATA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // ���� �� ��� ���� �� ���͸��� �˻��ϱ� ���� ���
    wstring searchPath = folderPath + L"\\*";

    // ù ��° ����/���͸� �˻�
    hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        wprintf(L"Failed to open directory: %s\n", folderPath.c_str());
        return;
    }

    do
    {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0)
            {
                // ���� ���͸��� ��������� Ž��
                wstring subFolderPath = folderPath + L"\\" + findData.cFileName;
                SearchFolder(subFolderPath);
            }
        }
        else
        {
            // ���� ��� ����
            std::wstring fileName = findData.cFileName;
            std::wstring filePath = folderPath + L"\\" + fileName;

            // ���� ó��
            ProcessFile(filePath);
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}

void CFileMgr::ProcessFile(const std::wstring& filePath)
{
    // ���� Ȯ���� Ȯ��
    size_t lastSlashPos = filePath.find_last_of(L"\\/");
    wstring fileName = (lastSlashPos == wstring::npos) ? filePath : filePath.substr(lastSlashPos + 1);
    wstring extension = fileName.substr(fileName.find_last_of(L'.') + 1);

    // PNG ���� ó��
    const wchar_t* contentPath = CPathMgr::GetInstance()->GetContentPath();
    wstring relativePath = CPathMgr::GetInstance()->GetRelativePath(filePath.c_str());
    // Ȯ���ڿ� ���� ó��
    if (extension == L"png")
    {
        wstring tag = GetFileNameWithoutExtension(fileName);

        Direct2DMgr::GetInstance()->LoadAndStoreBitmap(relativePath.c_str(), tag.c_str(), false);

        wprintf(L"Loaded PNG: %s\n", tag.c_str());
    }
    else if (extension == L"mp3" || extension == L"wav")
    {
        // MP3 �Ǵ� WAV ���� ó��
        wstring key = GetFileNameWithoutExtension(fileName);

        if (L"main_title_bgm" == key) {
            CSoundMgr::GetInstance()->AddSound(key, relativePath.c_str(), true, false);
            wprintf(L"BGM: %s\n", key.c_str());
        }
        else {
            CSoundMgr::GetInstance()->AddSound(key, relativePath.c_str(), false, false);
        }

        //CSoundMgr::GetInstance()->AddSound(key, relativePath.c_str(), false, false);

        wprintf(L"Loaded sound: %s\n", key.c_str());
    }
}

wstring CFileMgr::GetFileNameWithoutExtension(const wstring& fileName)
{
    size_t dotPos = fileName.find_last_of(L'.');
    if (dotPos != wstring::npos)
    {
        return fileName.substr(0, dotPos); // Ȯ���ڸ� ������ ���� �̸� ��ȯ
    }
    return fileName; // Ȯ���ڰ� ���� ��� �״�� ��ȯ
}