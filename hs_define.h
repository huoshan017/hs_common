#ifndef __HS_DEFINE_20130806_H__
#define __HS_DEFINE_20130806_H__

// ������
static const int HS_NET_PACKET_MAX_SIZE = 8192;

// ��ͷ��
static const unsigned char HS_NET_PACKET_HEDER_LENGTH = 4;

// ʶ����
static const unsigned long HS_NET_PACKET_IDENTIFY_NUMBER = 0xEDCBA98;

// ������Ĭ�ϴ�С
static const int SESSION_DATA_BUFFER_DEFAULT_SIZE = HS_NET_PACKET_MAX_SIZE * 4;

// �ͻ������ӻ�����Ĭ�ϴ�С
static const int CLIENT_SESSION_DATA_BUFFER_DEFAULT_SIZE = HS_NET_PACKET_MAX_SIZE * 4;

// �����������ӻ�����Ĭ�ϴ�С
static const int SERVER_SESSION_DATA_BUFFER_DEFAULT_SIZE = HS_NET_PACKET_MAX_SIZE * 4 * 16;

// channel���ӳ�ʱĬ��ʱ��
static const unsigned int CHANNEL_CONNECT_DEFAULT_TIMEOUT = 30;

// ���������Ͷ���
enum {
	LOGIN_SERVER_TYPE		= 1,			// ��¼������
	GATE_SERVER_TYPE		= 2,			// ���ط�����
	WORLD_SERVER_TYPE		= 3,			// ���������
	GAME_SERVER_TYPE		= 4,			// ��Ϸ�߼�������
	PUBLIC_SERVER_TYPE		= 5,			// ����������
	DB_SERVER_TYPE			= 6,			// ���ݻ��������
};

#ifdef WIN32
#include <winsock2.h>
typedef SOCKET HS_SOCKET;
#else
typedef int HS_SOCKET;
#endif

#endif