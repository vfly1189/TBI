#include "global.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr()
	: m_szContentPath{}
{

}

CPathMgr::~CPathMgr() {

}

void CPathMgr::init()
{
	//Ctrl + F5����, Ȥ�� �Ϲ������� �ص� ������Ʈ ������ ��.
	//�� ���־� ��Ʃ������� �����ϴ°Ŷ�, ���α׷� ��ü �����̶�
	//��ΰ��� �޶���. 
	GetCurrentDirectory(255, m_szContentPath);
	//���������� ������(\\�� ���������� ã�� �ű⿡ null���� �ֱ�.) 
	int dirLen = static_cast<int>(wcslen(m_szContentPath));
	for (int dirIDX = dirLen - 1; dirIDX >= 0; dirIDX--) {
		if ('\\' == m_szContentPath[dirIDX]) {
			m_szContentPath[dirIDX] = '\0';
			break;
		}
	}
	//  + bin\\content\\ ��� �ٿ��ֱ�
	wcscat_s(m_szContentPath, 255, L"\\bin\\content");
	//��� ��� ���� �Ϸ�.
	//SetWindowText(CCore::GetInstance()->GetMainHwnd(), m_szContentPath);
}

wstring CPathMgr::GetRelativePath(const wchar_t* _filepath)
{
	wstring strFilePath = _filepath;
	size_t iAbsLen = wcslen(m_szContentPath);
	size_t iFullLen = strFilePath.length();
	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);

	return strRelativePath;
}
