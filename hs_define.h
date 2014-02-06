#ifndef __HS_DEFINE_20130806_H__
#define __HS_DEFINE_20130806_H__

// 最大包长
static const int HS_NET_PACKET_MAX_SIZE = 8192;

// 包头长
static const unsigned char HS_NET_PACKET_HEDER_LENGTH = 4;

// 识别码
static const unsigned long HS_NET_PACKET_IDENTIFY_NUMBER = 0xEDCBA98;

// 缓冲区默认大小
static const int SESSION_DATA_BUFFER_DEFAULT_SIZE = HS_NET_PACKET_MAX_SIZE * 4;

// 客户端连接缓冲区默认大小
static const int CLIENT_SESSION_DATA_BUFFER_DEFAULT_SIZE = HS_NET_PACKET_MAX_SIZE * 4;

// 服务器间连接缓冲区默认大小
static const int SERVER_SESSION_DATA_BUFFER_DEFAULT_SIZE = HS_NET_PACKET_MAX_SIZE * 4 * 16;

// channel连接超时默认时间
static const unsigned int CHANNEL_CONNECT_DEFAULT_TIMEOUT = 30;

// 服务器类型定义
enum {
	LOGIN_SERVER_TYPE		= 1,			// 登录服务器
	GATE_SERVER_TYPE		= 2,			// 网关服务器
	WORLD_SERVER_TYPE		= 3,			// 世界服务器
	GAME_SERVER_TYPE		= 4,			// 游戏逻辑服务器
	PUBLIC_SERVER_TYPE		= 5,			// 公共服务器
	DB_SERVER_TYPE			= 6,			// 数据缓存服务器
};

#ifdef WIN32
#include <winsock2.h>
typedef SOCKET HS_SOCKET;
#else
typedef int HS_SOCKET;
#endif

#endif