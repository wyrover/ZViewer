#include "stdafx.h"

#include "ZFileExtReg.H"

bool ZFileExtReg::SetRegistries()
{
	RegSetExtension();
	RegSetDocumentType();

	return true;
}

bool ZFileExtReg::RegSetExtension()
{
	if( m_strExtension.empty() )
	{
		_ASSERTE(!"Extension string is empty!");
		return false;
	}

	std::string strKey = ".";
	strKey += m_strExtension;

	SetRegistryValue(HKEY_CLASSES_ROOT, strKey.c_str(), "", m_strDocumentClassName.c_str());

	if( !m_strShellOpenCommand.empty() )
	{
		strKey += "\\shell\\open\\command";
		SetRegistryValue(HKEY_CLASSES_ROOT, strKey.c_str(), "", m_strShellOpenCommand.c_str());
	}

	// 확장자에 따른 기본 프로그램을 정한다.
	strKey = "SoftWare\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.";
	strKey += m_strExtension;
	SetRegistryValue(HKEY_CURRENT_USER, strKey.c_str(), "ProgID", m_strDocumentClassName.c_str());

	return TRUE;
}

bool  ZFileExtReg::RegSetDocumentType()
{
	if( m_strDocumentClassName.empty())
	{
		_ASSERTE(!"DocumentClassName string is empty!");
		return false;
	}

	std::string strKey = m_strDocumentClassName;

	SetRegistryValue(HKEY_CLASSES_ROOT, strKey.c_str(), "", m_strDocumentDescription.c_str());

	// 기본 아이콘 등록
	if( !m_strDocumentDefaultIcon.empty() )
	{
		strKey  = m_strDocumentClassName;
		strKey += "\\DefaultIcon";
		SetRegistryValue(HKEY_CLASSES_ROOT, strKey.c_str(), "", m_strDocumentDefaultIcon.c_str());
	}

	// 더블 클릭했을 때 실행 시킬 명령
	if( !m_strShellOpenCommand.empty() )
	{
		strKey  = m_strDocumentClassName;
		strKey += "\\shell\\open\\command";
		SetRegistryValue(HKEY_CLASSES_ROOT, strKey.c_str(), "", m_strShellOpenCommand.c_str());
	}

	return true;
}

void ZFileExtReg::Clear()
{
	m_strExtension.clear();
	m_strContentType.clear();
	m_strShellOpenCommand.clear();
	m_strShellNewCommand.clear();
	m_strShellNewFileName.clear();

	m_strDocumentClassName.clear();
	m_strDocumentDescription.clear();
	m_strDocumentCLSID.clear();
	m_strDocumentCurrentVersion.clear();
	m_strDocumentDefaultIcon.clear();
	m_strDocumentShellOpenCommand.clear();
}

bool ZFileExtReg::SetRegistryValue(HKEY hOpenKey, const std::string & strKey,LPCTSTR szValue, const std::string & strData)
{
	if( !hOpenKey || strKey.empty() || !szValue || strData.empty() )
	{
		_ASSERTE(!"SetRegistryValue invalid arg");
		return false;
	}

	bool bRetVal = false;
	DWORD dwDisposition;
	HKEY hTempKey = NULL;

	if( ERROR_SUCCESS == ::RegCreateKeyEx(hOpenKey, strKey.c_str(), NULL,
		NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hTempKey, &dwDisposition) )
	{
		// 마지막 \0 까지 포함해야한다던데;;
		DWORD	dwBufferLength = (DWORD)strData.size() + 1;
		
		if( ERROR_SUCCESS == ::RegSetValueEx(hTempKey, (LPTSTR)szValue,
			NULL, REG_SZ, (const BYTE *)strData.c_str(), dwBufferLength) )
		{
			bRetVal = true;
		}
	}

	if( hTempKey )
	{
		::RegCloseKey(hTempKey);
	}

	return bRetVal;
}
