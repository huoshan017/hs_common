#include "hs_server_channel.h"
#include <iostream>

#ifndef WIN32
#include "fcntl.h"
#endif

hs_connector::hs_connector() : port_(0), state_(CLOSED)
{
	memset(&timeout_, 0, sizeof(timeout_));
}

hs_connector::~hs_connector()
{
}

bool hs_connector::init(const char* ip, short port)
{
	ip_ = ip;
	port_ = port;

	//设置非阻塞模式
	u_long iMode = 1;
	::ioctlsocket(sock_, FIONBIO, &iMode);

	return true;
}

void hs_connector::close()
{
	::closesocket(sock_);
	state_ = CLOSED;
}

bool hs_connector::connect(unsigned int ms)
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(ip_.c_str());
	addr.sin_port = port_;
	int res = ::connect(sock_, (SOCKADDR*)&addr, sizeof(SOCKADDR));
#ifdef WIN32
	if (res == 0)
	{
		state_ = CONNECTED;
		cout << "connect success!" << endl;
		return true;
	}
	else if (res == SOCKET_ERROR)
	{
		if (::WSAGetLastError() == WSAEWOULDBLOCK)
		{
			state_ = CONNECTING;
			return true;
		}

		close();
		return false;
	}
	else
	{
		cout << "not happened!" << endl;
	}
#else
#endif

	timeout_.tv_sec = ms/1000;
	timeout_.tv_usec = ms - (timeout_.tv_sec)*1000;

	return true;
}

void hs_connector::disconnect()
{
#ifdef WIN32
	::shutdown(sock_, 2);
	::closesocket(sock_);
#else
#endif
}

int hs_connector::poll(unsigned int delta)
{
	if (state_ != CONNECTING)
		return 0;

	fd_set wf;
	FD_ZERO(&wf);
	int res = ::select(sock_+1, NULL, &wf, NULL, &timeout_);
#ifdef WIN32
	if (res < 0)
	{
		// 错误
		::closesocket(sock_);
		cout << "connect time out!" << endl;
		return -1;
	}
	else if (res == 0)
	{
		// 超时继续poll
		return 0;
	}
	else
	{
		// 不是可写事件
		if (!FD_ISSET(sock_, &wf))
		{
			::closesocket(sock_);
			cout << "no events on socket found!" << endl;
			return -1;
		}
	}
#else
#endif

	state_= CONNECTED;

	return 1;
}

/* hs_listener */


/* hs_server_channel */

hs_server_channel::hs_server_channel() : sock_(0), port_(0), /*timeout_(0), u_timeout_(0),*/ state_(Idle)
{
}

hs_server_channel::~hs_server_channel()
{
}

void hs_server_channel::init(const char* ip, short port)
{
	ip_ = ip;
	port_ = port;

#ifdef WIN32
	WSADATA wsaData;
	::WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	sock_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#ifdef WIN32
	if (sock_ == INVALID_SOCKET) {
		return;
	}
#endif

#ifdef WIN32
	u_long value = 1;
	::ioctlsocket(sock_, FIONBIO, &value);
#else
	int flags = ::fcntl(sock_, F_GETFL, 0);
	::fcntl(sock_, F_SETFL, flags|O_NONBOLCK);
#endif
}

void hs_server_channel::close()
{
	if (state_ == Idle || state_ == Closed)
		return;

#ifdef WIN32
	::WSACleanup();
	::closesocket(sock_);
#else
	::close(sock_);
#endif

	state_ = Closed;
}

bool hs_server_channel::connect()
{
	struct sockaddr_in si;
	si.sin_family = AF_INET;
	si.sin_addr.S_un.S_addr = inet_addr(ip_.c_str());
	si.sin_port = htons(port_);
	memset(si.sin_zero, 0, sizeof(si.sin_zero));
	int res = ::connect(sock_, (sockaddr*)&si, sizeof(si));
	if (res < 0) {
		state_ = Connecting;
		return true;
	} else {
#ifndef WIN32
#else
		int err = ::WSAGetLastError();
		if (err == 10061) {
		} else {
		}
#endif
	}
	return true;
}

bool hs_server_channel::accept()
{
	if (state_!=Idle || state_!=Closed)
		return false;

	state_ = Accepting;
	return true;
}

void hs_server_channel::disconnect()
{
}

bool hs_server_channel::send(const char* data, size_t data_len)
{
#ifdef WIN32
	::send(sock_, data, data_len, 0);
#else
	::send(sock_, data, data_len, MSG_NOSIGNAL);
#endif

	return true;
}

bool hs_server_channel::poll()
{
	if (state_ == Idle || state_ == Closed)
		return true;
	
	tm_.tv_sec = 0;
	tm_.tv_usec = 0;
	FD_ZERO(&fs_);
	FD_SET(sock_, &fs_);

	// windows下第一个参数可以设为任意值
	int res = ::select(sock_+1, &fs_, NULL, NULL, &tm_);
	// 超时
	if (res == 0) {
		res = handle_timeout();
	}
	// 可读或可写
	else if (res > 0) {
		res = handle_write();
	}
	// 错误
	else {
		
	}

	// 连接状态
	if (state_ == Connecting) {
#ifndef WIN32
#else
		cout << res << endl;
#endif
	}
	// 监听状态
	else if (state_ == Accepting)
	{
	}

	return true;
}

int hs_server_channel::get_error()
{
	if (state_ != Closed)
		return 0;

#ifdef WIN32
	return ::WSAGetLastError();
#else
	return error;
#endif
}

const char* hs_server_channel::get_error_string()
{
	return "";
}

int hs_server_channel::handle_timeout()
{
	return 0;
}

int hs_server_channel::handle_write()
{
	return 0;
}

int hs_server_channel::handle_read()
{
	return 0;
}

int hs_server_channel::handle_error()
{
	return 0;
}