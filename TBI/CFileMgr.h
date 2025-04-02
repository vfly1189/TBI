#pragma once
class CFileMgr
{
	SINGLE(CFileMgr);

private:
	void SearchFolder(const wstring& folderPath);
	void ProcessFile(const wstring& filePath);
	wstring GetFileNameWithoutExtension(const wstring& fileName);
public:
	wstring GetLastFolderName(const std::wstring& path);
	wstring GetLastFileName(const std::wstring& path);
public:
	void init(const wstring& folderPath);
};


