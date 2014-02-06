#ifndef __HS_SYSTEM_H_201306302248__
#define __HS_SYSTEM_H_201306302248__

#include <string>
using namespace std;

class HSSystem
{
public:
	static void Sleep(int secs);
	static void MSleep(int msecs);
	static void USleep(int usecs);
	static const char* GetCurrentDir();

private:
	static string m_strCurrentDir;
};

#ifdef WIN32
#define Snprintf sprintf_s
#else
#define Snprintf snprintf
#endif

#endif