#pragma once

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


class MyLuaWrapper
{
public:
	MyLuaWrapper(void);
	~MyLuaWrapper(void);

	bool Init();
	void Close();

	bool LoadFile(const char* script);
	bool DoFile(const char* script);
	bool RequireFile(const char* script);
	bool DoString(const char* buffer);
	bool call_va(const char* func, const char* sig, ...);
	bool call_va(const char* script, const char* func, const char* sig, ...);
	bool call_va_with_userdata(const char* func, const char* sig, ...);

private:
	lua_State* m_pState;
};

