#ifndef __HS_SERVER_CHANNEL_H_20130831__
#define __HS_SERVER_CHANNEL_H_20130831__

/**
 * ������֮�������ͨ������Ϊ���Ӻ��٣�������selectʵ��
 * ����ԭ��Ϊ�Ƚ������Ӳ�������ʱ������socket��Ϊ����״̬
 * ������֮���໥���Ӳ��ÿ����Ⱥ�˳��ͬʱ�ṩ������������
 */

#include "hs_buffer.h"
#include "hs_define.h"
#include <string>

using namespace std;

const size_t DEFAULT_RECV_BUFF_LEN = 4096;
const size_t MAX_RECV_BUFF_LEN = 4096 * 16;
const size_t DEFAULT_SEND_BUFF_LEN = 4096;
const size_t MAX_SEND_BUFF_LEN = 4096 * 16;

/**
 * hs_sock_base �����ڻ���
 * û�ж������߼������ܵ���ʹ��
 */

class hs_sock_base
{
public:
	hs_sock_base(size_t send_buff_size=DEFAULT_SEND_BUFF_LEN, size_t recv_buff_size=DEFAULT_RECV_BUFF_LEN) :
			sock_(0),
			send_buff_(NULL),
			recv_buff_(NULL),
			send_buff_len_(send_buff_size),
			recv_buff_len_(recv_buff_size)
	{
	}
	virtual ~hs_sock_base()
	{
		if (send_buff_)
		{
			delete send_buff_;
			send_buff_ = NULL;
		}
		if (recv_buff_)
		{
			delete recv_buff_;
			recv_buff_ = NULL;
		}
		send_buff_len_ = 0;
		recv_buff_len_ = 0;
	}

	/**
	 * �ȷ��ͻ����е����ݣ��ٷ��Ͳ����е����ݣ���������浽���棬�ȵ��´�writeʱ����poll����
	 * @ buf ��������
	 * @ len ���泤��
	 */
	int write(const char* buf, size_t len);

	/**
	 * �������ݼӽ�������
	 */
	void poll();

protected:
	HS_SOCKET sock_;
	char* send_buff_;
	char* recv_buff_;
	size_t send_buff_len_;
	size_t recv_buff_len_;
};

class hs_connector : public hs_sock_base
{
public:
	hs_connector();
	~hs_connector();

	bool init(const char* ip, short port);
	void close();

	// ����������(Ĭ�ϳ�ʱʱ��Ϊ0)
	bool connect(unsigned int timeout = 0);

	// �Ͽ�
	void disconnect();

	// ��ѭ������
	int poll(unsigned int delta);

private:
	//HS_SOCKET sock_;
	string ip_; 
	short port_;
	timeval timeout_;
	enum State {
		CLOSED = 0,
		CONNECTING = 1,
		CONNECTED = 2,
	};
	State state_;
};

class hs_listener
{
public:
	hs_listener();
	~hs_listener();

private:
	HS_SOCKET sock_;
	string ip_;
	short port_;
	enum State {
		CLOSED = 0,
		LISTENING = 1,
		ACCEPTED = 2,
	};
	State state_;
};

class hs_server_channel
{
public:
	hs_server_channel();
	~hs_server_channel();

	void init(const char* ip, short port);
	//void set_timeout(size_t secs, size_t usecs) { timeout_ = secs; }
	void close();

	bool connect();		// ����������
	bool accept();		// ����������
	void disconnect();	// �����Ͽ�

	bool send(const char* data, size_t data_len);

	bool poll();

	int get_error();
	const char* get_error_string();

private:
	int handle_timeout();
	int handle_write();
	int handle_read();
	int handle_error();

private:
	HS_SOCKET sock_;
	HSRecvBuffer recv_buffer_;
	HSSendBuffer send_buffer_;
	string ip_;
	short port_;
	//size_t timeout_;
	//size_t u_timeout_;
	int error_;

	enum
	{
		Idle,			// ��ʼ״̬
		Connecting,		// ��������
		Connected,		// �������
		Accepting,		// ����״̬
		Accepted,		// ������
		Closed,			// �ر�״̬
	};

	int state_;

	// ÿ֡��ѯʱ��Ҫ������
	timeval tm_;
	fd_set fs_;
};

#endif