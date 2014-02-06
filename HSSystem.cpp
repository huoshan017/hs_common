#include "HSSystem.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#endif


string HSSystem::m_strCurrentDir;

void HSSystem::Sleep(int secs)
{
#ifdef _WIN32
	::Sleep(secs*1000);
#else
	sleep(secs);
#endif
}

void HSSystem::MSleep(int msecs)
{
#ifdef _WIN32
	::Sleep(msecs);
#else
	usleep(msecs*1000);
#endif
}

void HSSystem::USleep(int usecs)
{
#ifdef _WIN32
	::Sleep(usecs/1000);
#else
	usleep(usecs);
#endif
}

const char* HSSystem::GetCurrentDir()
{
	if (m_strCurrentDir.length() == 0)
	{
		//wchar_t szBuffer[1024];
		char buffer[2048];

#ifdef _WIN32
		GetModuleFileName(NULL, /*szBuffer*/buffer, /*sizeof(szBuffer)*/sizeof(buffer)-1);
		//size_t s = 0;
		//wcstombs_s(&s, buffer, szBuffer, sizeof(buffer));
#else
#endif

		m_strCurrentDir = buffer;
		string::size_type offset = m_strCurrentDir.rfind('\\');
		if (offset != string::npos) {
			m_strCurrentDir = m_strCurrentDir.substr(0, offset-0+1);
		}
	}
	return m_strCurrentDir.c_str();
}