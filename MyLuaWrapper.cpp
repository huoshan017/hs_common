#include "MyLuaWrapper.h"
#include <stdarg.h>
#include <string.h>

int  tolua_interface2lua_open (lua_State* tolua_S);

MyLuaWrapper::MyLuaWrapper(void) : m_pState(NULL)
{
}


MyLuaWrapper::~MyLuaWrapper(void)
{
}


bool MyLuaWrapper::Init()
{
	if (m_pState == NULL) {
		m_pState = lua_open(); //初始化lua
		luaL_openlibs(m_pState);    //载入所有lua标准库
		tolua_interface2lua_open(m_pState);
	}

	return m_pState != NULL;
}

void MyLuaWrapper::Close()
{
	if (m_pState) {
		lua_close(m_pState);
		m_pState = NULL;
	}
}

bool MyLuaWrapper::LoadFile(const char* script)
{
	if (!m_pState)
		return false;

	if (!luaL_loadfile(m_pState, script) != 0) {
		printf("MyLuaWrapper::LoadFile(\"%s\") Failed!", script);
		return false;
	}

	return true;
}

bool MyLuaWrapper::DoFile(const char* script)
{
	if (!m_pState)
		return false;

	if (luaL_dofile(m_pState, script) != 0) {
		printf("MyLuaWrapper::DoFile(\"%s\") Failed!", script);
		return false;
	}

	return true;
}

bool MyLuaWrapper::DoString(const char* buffer)
{
	if (!m_pState)
		return false;

	if (!luaL_dostring(m_pState, buffer) != 0) {
		printf("MyLuaWrapper::DoString(\"%s\") Failed!", buffer);
		return false;
	}	

	return true;
}

bool MyLuaWrapper::call_va(const char* func, const char* sig, ...)
{
	if (func == NULL)
		return false;

	if (m_pState == NULL)
		return false;

	va_list vl;
	int narg, nres;

	va_start(vl, sig);
	lua_getglobal(m_pState, func);

	/* push arguments */
	narg = 0;
	while (*sig) {
		switch (*sig++) {
		case 'd': /* double argument */
			lua_pushnumber(m_pState, va_arg(vl, double));
			break;
		case 'i': /* int argument */
			lua_pushinteger(m_pState, va_arg(vl, int));
			break;
		case 'u': /* unsigned int argument */
			lua_pushinteger(m_pState, va_arg(vl, unsigned int));
			break;
		case 's':
			lua_pushstring(m_pState, va_arg(vl, char*));
			break;
		case '>':
			goto endwhile;
		default:
			lua_error(m_pState);
			return false;
		}
		++narg;
		luaL_checkstack(m_pState, 1, "too many arguments");
	}
endwhile:

	nres = strlen(sig);

	/* do the call */
	if (lua_pcall(m_pState, narg, nres, 0) != 0) {
		printf("error running function '%s': %s", func, lua_tostring(m_pState, -1));
		lua_pop(m_pState, 1);
		return false;
	}

	int n = nres;
	nres = -nres;

	while (*sig) {
		switch (*sig++)
		{
		case 'd':
			if (!lua_isnumber(m_pState, nres)) {
				lua_pop(m_pState, n);
				printf("wrong result type");
				return false;
			}
			*va_arg(vl, double*) = lua_tonumber(m_pState, nres);
			break;
		case 'i':
			if (!lua_isnumber(m_pState, nres)) {
				lua_pop(m_pState, n);
				printf("wrong result type");
				return false;
			}
			*va_arg(vl, int*) = lua_tointeger(m_pState, nres);
			break;
		case 's':
			if (!lua_isstring(m_pState, nres)) {
				lua_pop(m_pState, n);
				printf("wrong result type");
				return false;
			}
			*va_arg(vl, const char**) = lua_tostring(m_pState, nres);
			break;
		default:
			lua_pop(m_pState, n);
			printf("Invalid option (%c)", *(sig-1));
			return false;
		}
		nres++;
	}
	va_end(vl);

	lua_pop(m_pState, n);

	return true;
}

bool MyLuaWrapper::call_va(const char* script, const char* func, const char* sig, ...)
{
	return true;
}

bool MyLuaWrapper::call_va_with_userdata(const char* func, const char* sig, ...)
{
	return true;
}