#ifndef __HS_SERVER_CHANNEL_H_20130831__
#define __HS_SERVER_CHANNEL_H_20130831__

/**
 * 服务器之间的连接通道，因为连接很少，所以用select实现
 * 基本原理为先进行连接操作，超时后重置socket改为监听状态
 * 服务器之间相互连接不用考虑先后顺序，同时提供断线重连功能
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
 * hs_sock_base 仅用于基类
 * 没有独立的逻辑，不能单独使用
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
	 * 先发送缓存中的数据，再发送参数中的数据，发不完则存到缓存，等到下次write时发或poll推送
	 * @ buf 缓存数据
	 * @ len 缓存长度
	 */
	int write(const char* buf, size_t len);

	/**
	 * 推送数据加接受数据
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

	// 非阻塞连接(默认超时时间为0)
	bool connect(unsigned int timeout = 0);

	// 断开
	void disconnect();

	// 主循环调用
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

	bool connect();		// 非阻塞连接
	bool accept();		// 非阻塞监听
	void disconnect();	// 主动断开

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
		Idle,			// 初始状态
		Connecting,		// 正在连接
		Connected,		// 连接完成
		Accepting,		// 监听状态
		Accepted,		// 被连接
		Closed,			// 关闭状态
	};

	int state_;

	// 每帧轮询时需要的数据
	timeval tm_;
	fd_set fs_;
};

#endif