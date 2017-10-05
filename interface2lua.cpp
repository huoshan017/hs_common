/*
** Lua binding: interface2lua
** Generated automatically by tolua++-1.0.92 on 08/29/13 22:45:05.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_interface2lua_open (lua_State* tolua_S);

#include "HSSystem.h"
#include "config_file.h"
#include "hs_std.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"map");
 tolua_usertype(tolua_S,"UintKeyConfigFile");
 tolua_usertype(tolua_S,"IntKeyConfigFile");
 tolua_usertype(tolua_S,"HSValue");
 tolua_usertype(tolua_S,"deque");
 tolua_usertype(tolua_S,"list");
 tolua_usertype(tolua_S,"StringKeyConfigFile");
 tolua_usertype(tolua_S,"vector");
 tolua_usertype(tolua_S,"HSSystem");
}

/* method: Sleep of class  HSSystem */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSSystem_Sleep00
static int tolua_interface2lua_HSSystem_Sleep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"HSSystem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int secs = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   HSSystem::Sleep(secs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Sleep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MSleep of class  HSSystem */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSSystem_MSleep00
static int tolua_interface2lua_HSSystem_MSleep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"HSSystem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int msecs = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   HSSystem::MSleep(msecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MSleep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: USleep of class  HSSystem */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSSystem_USleep00
static int tolua_interface2lua_HSSystem_USleep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"HSSystem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int usecs = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   HSSystem::USleep(usecs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'USleep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCurrentDir of class  HSSystem */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSSystem_GetCurrentDir00
static int tolua_interface2lua_HSSystem_GetCurrentDir00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"HSSystem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  HSSystem::GetCurrentDir();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCurrentDir'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsBool of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsBool00
static int tolua_interface2lua_HSValue_AsBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsBool'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->AsBool();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsChar of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsChar00
static int tolua_interface2lua_HSValue_AsChar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsChar'", NULL);
#endif
  {
   char tolua_ret = (char)  self->AsChar();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsChar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsUchar of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsUchar00
static int tolua_interface2lua_HSValue_AsUchar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsUchar'", NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->AsUchar();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsUchar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsShort of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsShort00
static int tolua_interface2lua_HSValue_AsShort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsShort'", NULL);
#endif
  {
   short tolua_ret = (short)  self->AsShort();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsShort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsUshort of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsUshort00
static int tolua_interface2lua_HSValue_AsUshort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsUshort'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->AsUshort();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsUshort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsInt of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsInt00
static int tolua_interface2lua_HSValue_AsInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsInt'", NULL);
#endif
  {
   int tolua_ret = (int)  self->AsInt();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsUint of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsUint00
static int tolua_interface2lua_HSValue_AsUint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsUint'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->AsUint();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsUint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsFloat of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsFloat00
static int tolua_interface2lua_HSValue_AsFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsFloat'", NULL);
#endif
  {
   float tolua_ret = (float)  self->AsFloat();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsDouble of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsDouble00
static int tolua_interface2lua_HSValue_AsDouble00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsDouble'", NULL);
#endif
  {
   double tolua_ret = (double)  self->AsDouble();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsDouble'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsLong of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsLong00
static int tolua_interface2lua_HSValue_AsLong00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsLong'", NULL);
#endif
  {
   long tolua_ret = (long)  self->AsLong();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsLong'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsUlong of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsUlong00
static int tolua_interface2lua_HSValue_AsUlong00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsUlong'", NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->AsUlong();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsUlong'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsCString of class  HSValue */
#ifndef TOLUA_DISABLE_tolua_interface2lua_HSValue_AsCString00
static int tolua_interface2lua_HSValue_AsCString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"HSValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  HSValue* self = (HSValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsCString'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->AsCString();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsCString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetData of class  IntKeyConfigFile */
#ifndef TOLUA_DISABLE_tolua_interface2lua_IntKeyConfigFile_GetData00
static int tolua_interface2lua_IntKeyConfigFile_GetData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IntKeyConfigFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IntKeyConfigFile* self = (IntKeyConfigFile*)  tolua_tousertype(tolua_S,1,0);
  const int key = ((const int)  tolua_tonumber(tolua_S,2,0));
  const char* field = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetData'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetData(key,field);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
   tolua_pushnumber(tolua_S,(lua_Number)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHSValue of class  IntKeyConfigFile */
#ifndef TOLUA_DISABLE_tolua_interface2lua_IntKeyConfigFile_GetHSValue00
static int tolua_interface2lua_IntKeyConfigFile_GetHSValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"IntKeyConfigFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  IntKeyConfigFile* self = (IntKeyConfigFile*)  tolua_tousertype(tolua_S,1,0);
  const int key = ((const int)  tolua_tonumber(tolua_S,2,0));
  const char* field = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHSValue'", NULL);
#endif
  {
   HSValue* tolua_ret = (HSValue*)  self->GetHSValue(key,field);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"HSValue");
   tolua_pushnumber(tolua_S,(lua_Number)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHSValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetData of class  UintKeyConfigFile */
#ifndef TOLUA_DISABLE_tolua_interface2lua_UintKeyConfigFile_GetData00
static int tolua_interface2lua_UintKeyConfigFile_GetData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UintKeyConfigFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UintKeyConfigFile* self = (UintKeyConfigFile*)  tolua_tousertype(tolua_S,1,0);
  unsigned const int key = ((unsigned const int)  tolua_tonumber(tolua_S,2,0));
  const char* field = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetData'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetData(key,field);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
   tolua_pushnumber(tolua_S,(lua_Number)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHSValue of class  UintKeyConfigFile */
#ifndef TOLUA_DISABLE_tolua_interface2lua_UintKeyConfigFile_GetHSValue00
static int tolua_interface2lua_UintKeyConfigFile_GetHSValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"UintKeyConfigFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  UintKeyConfigFile* self = (UintKeyConfigFile*)  tolua_tousertype(tolua_S,1,0);
  unsigned const int key = ((unsigned const int)  tolua_tonumber(tolua_S,2,0));
  const char* field = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHSValue'", NULL);
#endif
  {
   HSValue* tolua_ret = (HSValue*)  self->GetHSValue(key,field);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"HSValue");
   tolua_pushnumber(tolua_S,(lua_Number)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHSValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetData of class  StringKeyConfigFile */
#ifndef TOLUA_DISABLE_tolua_interface2lua_StringKeyConfigFile_GetData00
static int tolua_interface2lua_StringKeyConfigFile_GetData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"StringKeyConfigFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  StringKeyConfigFile* self = (StringKeyConfigFile*)  tolua_tousertype(tolua_S,1,0);
  const string key = ((const string)  tolua_tocppstring(tolua_S,2,0));
  const char* field = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetData'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetData(key,field);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHSValue of class  StringKeyConfigFile */
#ifndef TOLUA_DISABLE_tolua_interface2lua_StringKeyConfigFile_GetHSValue00
static int tolua_interface2lua_StringKeyConfigFile_GetHSValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"StringKeyConfigFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  StringKeyConfigFile* self = (StringKeyConfigFile*)  tolua_tousertype(tolua_S,1,0);
  const string key = ((const string)  tolua_tocppstring(tolua_S,2,0));
  const char* field = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHSValue'", NULL);
#endif
  {
   HSValue* tolua_ret = (HSValue*)  self->GetHSValue(key,field);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"HSValue");
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHSValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_interface2lua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"vector","vector","",NULL);
  tolua_beginmodule(tolua_S,"vector");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"map","map","",NULL);
  tolua_beginmodule(tolua_S,"map");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"list","list","",NULL);
  tolua_beginmodule(tolua_S,"list");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"deque","deque","",NULL);
  tolua_beginmodule(tolua_S,"deque");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"HSSystem","HSSystem","",NULL);
  tolua_beginmodule(tolua_S,"HSSystem");
   tolua_function(tolua_S,"Sleep",tolua_interface2lua_HSSystem_Sleep00);
   tolua_function(tolua_S,"MSleep",tolua_interface2lua_HSSystem_MSleep00);
   tolua_function(tolua_S,"USleep",tolua_interface2lua_HSSystem_USleep00);
   tolua_function(tolua_S,"GetCurrentDir",tolua_interface2lua_HSSystem_GetCurrentDir00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"HSValue","HSValue","",NULL);
  tolua_beginmodule(tolua_S,"HSValue");
   tolua_function(tolua_S,"AsBool",tolua_interface2lua_HSValue_AsBool00);
   tolua_function(tolua_S,"AsChar",tolua_interface2lua_HSValue_AsChar00);
   tolua_function(tolua_S,"AsUchar",tolua_interface2lua_HSValue_AsUchar00);
   tolua_function(tolua_S,"AsShort",tolua_interface2lua_HSValue_AsShort00);
   tolua_function(tolua_S,"AsUshort",tolua_interface2lua_HSValue_AsUshort00);
   tolua_function(tolua_S,"AsInt",tolua_interface2lua_HSValue_AsInt00);
   tolua_function(tolua_S,"AsUint",tolua_interface2lua_HSValue_AsUint00);
   tolua_function(tolua_S,"AsFloat",tolua_interface2lua_HSValue_AsFloat00);
   tolua_function(tolua_S,"AsDouble",tolua_interface2lua_HSValue_AsDouble00);
   tolua_function(tolua_S,"AsLong",tolua_interface2lua_HSValue_AsLong00);
   tolua_function(tolua_S,"AsUlong",tolua_interface2lua_HSValue_AsUlong00);
   tolua_function(tolua_S,"AsCString",tolua_interface2lua_HSValue_AsCString00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"IntKeyConfigFile","IntKeyConfigFile","",NULL);
  tolua_beginmodule(tolua_S,"IntKeyConfigFile");
   tolua_function(tolua_S,"GetData",tolua_interface2lua_IntKeyConfigFile_GetData00);
   tolua_function(tolua_S,"GetHSValue",tolua_interface2lua_IntKeyConfigFile_GetHSValue00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"UintKeyConfigFile","UintKeyConfigFile","",NULL);
  tolua_beginmodule(tolua_S,"UintKeyConfigFile");
   tolua_function(tolua_S,"GetData",tolua_interface2lua_UintKeyConfigFile_GetData00);
   tolua_function(tolua_S,"GetHSValue",tolua_interface2lua_UintKeyConfigFile_GetHSValue00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"StringKeyConfigFile","StringKeyConfigFile","",NULL);
  tolua_beginmodule(tolua_S,"StringKeyConfigFile");
   tolua_function(tolua_S,"GetData",tolua_interface2lua_StringKeyConfigFile_GetData00);
   tolua_function(tolua_S,"GetHSValue",tolua_interface2lua_StringKeyConfigFile_GetHSValue00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_interface2lua (lua_State* tolua_S) {
 return tolua_interface2lua_open(tolua_S);
};
#endif

