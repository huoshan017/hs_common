#include "hs_client_connector.h"

HSRecvDataUnpacker::HSRecvDataUnpacker(HSRecvBuffer* buffer) : buffer_(buffer)
{
}

HSRecvDataUnpacker::~HSRecvDataUnpacker()
{
}

HSSendDataPacker::HSSendDataPacker(HSSendBuffer* buffer) : buffer_(buffer)
{
}

HSSendDataPacker::~HSSendDataPacker()
{
}

int HSSendDataPacker::Write(const char* data, size_t len)
{
	if (!buffer_)
		return -1;

	size_t out_size = 0;

	// 封包拷贝数据到缓冲
	int res = HSPacketWorker::EncodePacket(data, len, buffer_->GetInOffsetData(), buffer_->GetLeftInSize(), out_size);

	if (res < 0) {
		cout << "HSSendDataPacker::Write: 数据过大超出最大包长，无法封包" << endl;
		return -1;
	}

	// 没有足够的缓冲区
	if (!res) {
		cout << "HSSendDataPacker::Write: 缓冲区不够，稍后再发" << endl;
		return 0;
	}

	// 偏移往后挪out_size
	if (!buffer_->DataIn(out_size))
		return -1;

	return 1;
}

/**
 HSClientConnector
 */

#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <Windows.h>
#else
#endif

HSClientConnector::HSClientConnector() : sock_(0), port_(0), state_(NotInitialized)
{
}

HSClientConnector::~HSClientConnector()
{
	Close();
}

bool HSClientConnector::Init(size_t recv_buff_len, size_t send_buff_len)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_ == SOCKET_ERROR)
		return false;

	u_long flag = 1;
	// 设为非阻塞
	if (ioctlsocket(sock_, FIONBIO, &flag) != 0) 
		return false;

	send_buffer_.SetLength(send_buff_len);
	packer_.SetBuffer(&send_buffer_);
	recv_buffer_.SetLength(recv_buff_len);
	unpacker_.SetBuffer(&recv_buffer_);

	state_ = Unconnect;

	return true;
}

void HSClientConnector::Close()
{
	if (sock_ <= 0)
		return;

	send_buffer_.Clear();
	packer_.SetBuffer(NULL);
	recv_buffer_.Clear();
	unpacker_.SetBuffer(NULL);

#ifdef WIN32
	shutdown(sock_, SD_BOTH);
	closesocket(sock_);
	WSACleanup();
#else
	shutdown(sock_, SHUT_RDWR);
	close(sock_);
#endif

	state_ = NotInitialized;
}

/*bool HSClientConnector::Reconnect()
{
	return Connect(ip_, port_);
}*/

void HSClientConnector::Disconnect()
{
	if (sock_ <= 0)
		return;

#ifdef WIN32
	shutdown(sock_, SD_BOTH);
#else
	shutdown(sock_, SHUT_RDWR);
#endif
}

int HSClientConnector::Send(const char* data, size_t len)
{
	if (packer_.Write(data, len) <= 0)
		return -1;

	// 马上执行Process
	if (packer_.Process(this) < 0)
		return -1;

	return 1;
}

int HSClientConnector::SendDirectly(const char* data, size_t len)
{
	return send(sock_, data, len, 0);
}
