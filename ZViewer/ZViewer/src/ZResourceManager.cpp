/*
2004. 12. 20 Kim Jinwook

Resource Manage for Language pack

*/

#include "stdafx.h"
#include "ZResourceManager.h"

ZResourceManager::ZResourceManager()
: m_hInstance(NULL)
{
}

ZResourceManager::~ZResourceManager()
{
}

ZResourceManager & ZResourceManager::GetInstance()
{
	static ZResourceManager instance;
	return instance;
}

const std::string ZResourceManager::GetString(UINT iStringID)
{
	if ( m_stringMap.find(iStringID ) == m_stringMap.end())
	{
		// 맵에서 찾을 수 없으면 LoadString 한 후 맵에 넣는다.
		char szString[256] = { 0 };
		if ( 0 == LoadString(m_hInstance, iStringID, szString, 256) )
		{
			_ASSERTE(!"Can't get string");
			strcpy(szString, "Can't GET STRING");
		}

		std::string strString = szString;
		m_stringMap[iStringID] = strString;

		return strString;
	}

	// 맵에 있으면 맵에 있는 스트링을 반환한다.
	return m_stringMap[iStringID];
}