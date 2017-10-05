#ifndef _HS_CLIENT_CONNECTOR_H_
#define _HS_CLIENT_CONNECTOR_H_

#include "hs_std.h"
#include "hs_define.h"
#include "hs_buffer.h"
#include "hs_packetworker.h"

/**
 接收缓冲数据分离器
 分离缓冲中所有的数据包，去掉包头并把分离后的逻辑数据交给处理器，这里不会
 把数据拷贝出来，而是返回缓冲中的偏移和长度，待处理函数处理完后得到下一个
 原始数据的偏移，继续上一步直到处理完。
 */

class HSRecvDataUnpacker
{
public:
	HSRecvDataUnpacker(HSRecvBuffer* buffer = NULL);
	~HSRecvDataUnpacker();

	void SetBuffer(HSRecvBuffer* buffer) { buffer_ = buffer; }

	// 接收缓冲已经读进了数据，直接处理即可
	template <class Connection, class DataHandler>
	int Process(Connection* conn, DataHandler* handler, size_t bytes_transferred);

private:
	HSRecvBuffer* buffer_;
};

template <class Connection, class DataHandler>
inline int HSRecvDataUnpacker::Process(Connection* conn, DataHandler* handler, size_t bytes_transferred)
{
	// 没有收到数据
	if (!bytes_transferred) {
		cout << "HSRecvBufferSeperator::Process: bytes_transferred = 0" << endl;
		return 0;
	}

	// 已经收到的数据 
	buffer_->DataIn(bytes_transferred);

	// 先解包再交给处理函数
	size_t next_offset = 0;
	size_t out_size = 0;
	int res = -1;
	while (true) {
		res = HSPacketWorker::DecodePacket(buffer_->GetCanUseBuffer(), buffer_->GetCanUseSize(), next_offset, out_size);

		// 没有完整的包跳出
		if (res == 0) {
			cout << "HSRecvBufferSeperator::Process: 没有完整的数据包，等待下一次数据" << endl;
			break;
		}

		// 数据接收有问题，可能是客户端发来的数据本身有问题，或者网络出现数据接收错误问题
		if (res < 0) {
			// 断开连接
			conn->Close();
			return -1;
		}

		// 去掉包头准备处理
		buffer_->DataUse(next_offset);

		// 处理用户数据
		if (handler)
			handler->OnReceive(conn, buffer_->GetCanUseBuffer(), out_size);

		// 处理完往后挪out_size个字节
		buffer_->DataUse(out_size);

		// 数据处理完毕
		if (buffer_->GetInOffset() == buffer_->GetUsedSize()) {
			cout << "HSRecvBufferSeperator::Process: 处理完毕，重新设置了偏移量" << endl;
			break;
		}
	}

	// 有不完整的数据
	if (res == 0) {
		//static size_t max_read_length = buffer_->GetLength() - HS_NET_PACKET_MAX_SIZE;
		//if (buffer_->GetInOffset() >= max_read_length) {

		// 剩下的未写的缓冲长度小于某个值时，把不完整的数据挪到缓冲区最前面，保证下次能收到完整的数据
		if (buffer_->GetLeftInSize() <= HS_NET_PACKET_MAX_SIZE/8) {
			buffer_->MoveUnuseData2Begin();
			cout << "HSRecvBufferSeperator::Process: 剩下的" << endl;
		}
	}

	return 0;
}

/**
 发送缓冲打包器
 */

class HSSendDataPacker
{
public:
	HSSendDataPacker(HSSendBuffer* buffer = NULL);
	~HSSendDataPacker();

	void SetBuffer(HSSendBuffer* buffer) { buffer_ = buffer; }

	// 写入数据（先发送数据直到返回0，然后再写到缓冲）
	int Write(const char* data, size_t len);

	// 通过连接发送数据
	template <class Connection>
	int Process(Connection* conn);

private:
	HSSendBuffer* buffer_;
};

template <class Connection>
int HSSendDataPacker::Process(Connection* conn)
{
	if (!conn || !buffer_) {
		return -1;
	}

	size_t size = buffer_->GetCanSendSize();
	if (!size)
		return 0;

	const char* send_data = buffer_->GetCanSendBuffer();
	int sent = conn->SendDirectly(send_data, size);
	if (sent < 0) {
		cout << "HSSendDataPacker::Process: 发送数据失败" << endl;
		return -1;
	}

	return 1;
}

/**
 用于客户端连接服务器
 */

class HSClientConnector
{
	enum State { NotInitialized = -1, Unconnect, Connecting, Connected };
public:
	HSClientConnector();
	~HSClientConnector();
	bool Init(size_t recv_buff_len=16*HS_NET_PACKET_MAX_SIZE, size_t send_buff_len=16*HS_NET_PACKET_MAX_SIZE);
	template <class DataHandler>
	bool Connect(const string& ip, short port, DataHandler* handler);
	//bool Reconnect();
	void Disconnect();
	void Close();

	// 写到缓冲区
	int Send(const char* data, size_t len);
	// 直接往socket上发，仅仅是封装了send函数
	int SendDirectly(const char* data, size_t len);

	template <class DataHandler>
	int Tick(DataHandler* handler, long delta);

private:
	HS_SOCKET sock_;
	string ip_;
	short port_;
	HSSendBuffer send_buffer_;
	HSSendDataPacker packer_;
	HSRecvBuffer recv_buffer_;
	HSRecvDataUnpacker unpacker_;
	State state_;
};

template <class DataHandler>
bool HSClientConnector::Connect(const string& ip, short port, DataHandler* handler)
{
	if (!sock_)
		return false;

	sockaddr_in si;
	si.sin_family = AF_INET;
	si.sin_port = htons(port);
	si.sin_addr.s_addr = inet_addr(ip.c_str());
	memset(si.sin_zero, 0, sizeof(si.sin_zero));

	int res = connect(sock_, (sockaddr*)&si, sizeof(si));
	if (res == SOCKET_ERROR) {
#ifdef WIN32
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return false;
#else
		if (errno != EWOULDBLOCK)
			return false;
#endif
		state_ = Connecting;
	} else if (res == 0) {
		// 连接成功
		state_ = Connected;
		handler->OnConnect(this, res);
	}

	ip_ = ip;
	port_ = port;

	return true;
}

template <class DataHandler>
int HSClientConnector::Tick(DataHandler* handler, long delta)
{
	if (state_!=Connecting && state_!=Connected)
		return 0;

	if (!handler)
		return -1;

	fd_set fs;
	FD_ZERO(&fs);
	FD_SET(sock_, &fs);
	struct timeval tv = { 0, 0 };

	// 正在连接中
	if (state_ == Connecting) {
		// 等待写事件
		int res = select(sock_+1, NULL, &fs, NULL, &tv);
		if (res < 0) {
#ifdef WIN32
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK)
				return 0;
#else
			if (errno == EWOULDBLOCK)
				return 0;
#endif
			return -1;
		} else if (res > 0) {

			// 连接成功
			if (FD_ISSET(sock_, &fs)) {
				state_ = Connected;
				handler->OnConnect(this, res);
				return 1;
			}
		}
	}
	// 已连接
	else if (state_ == Connected) {
		// 等待读事件
		int res = select(sock_+1, &fs, NULL, NULL, &tv);
		if (res < 0) {
			Close();
			// 断开连接
			handler->OnDisconnect(this, res);
			return 0;
		}

		// 可读
		if (res > 0) {

			// 读数据到缓冲区
			res = recv(sock_, recv_buffer_.GetCanUseBuffer(), recv_buffer_.GetCanUseSize(), 0);
			if (res < 0) {
				Close();
				handler->OnDisconnect(this, res);
				return 0;
			}

			if (res > 0) {
				if (unpacker_.Process(this, handler, res) < 0) {
					Close();
					handler->OnDisconnect(this, res);
					return 0;
				}
			}
		}

		// 读事件处理完，执行写操作
		if (packer_.Process(this) < 0) {
			Close();
			handler->OnDisconnect(this, res);
		}
	}

	return 0;
}

#endif