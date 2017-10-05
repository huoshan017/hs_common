#ifndef _HS_CLIENT_CONNECTOR_H_
#define _HS_CLIENT_CONNECTOR_H_

#include "hs_std.h"
#include "hs_define.h"
#include "hs_buffer.h"
#include "hs_packetworker.h"

/**
 ���ջ������ݷ�����
 ���뻺�������е����ݰ���ȥ����ͷ���ѷ������߼����ݽ��������������ﲻ��
 �����ݿ������������Ƿ��ػ����е�ƫ�ƺͳ��ȣ����������������õ���һ��
 ԭʼ���ݵ�ƫ�ƣ�������һ��ֱ�������ꡣ
 */

class HSRecvDataUnpacker
{
public:
	HSRecvDataUnpacker(HSRecvBuffer* buffer = NULL);
	~HSRecvDataUnpacker();

	void SetBuffer(HSRecvBuffer* buffer) { buffer_ = buffer; }

	// ���ջ����Ѿ����������ݣ�ֱ�Ӵ�����
	template <class Connection, class DataHandler>
	int Process(Connection* conn, DataHandler* handler, size_t bytes_transferred);

private:
	HSRecvBuffer* buffer_;
};

template <class Connection, class DataHandler>
inline int HSRecvDataUnpacker::Process(Connection* conn, DataHandler* handler, size_t bytes_transferred)
{
	// û���յ�����
	if (!bytes_transferred) {
		cout << "HSRecvBufferSeperator::Process: bytes_transferred = 0" << endl;
		return 0;
	}

	// �Ѿ��յ������� 
	buffer_->DataIn(bytes_transferred);

	// �Ƚ���ٽ���������
	size_t next_offset = 0;
	size_t out_size = 0;
	int res = -1;
	while (true) {
		res = HSPacketWorker::DecodePacket(buffer_->GetCanUseBuffer(), buffer_->GetCanUseSize(), next_offset, out_size);

		// û�������İ�����
		if (res == 0) {
			cout << "HSRecvBufferSeperator::Process: û�����������ݰ����ȴ���һ������" << endl;
			break;
		}

		// ���ݽ��������⣬�����ǿͻ��˷��������ݱ��������⣬��������������ݽ��մ�������
		if (res < 0) {
			// �Ͽ�����
			conn->Close();
			return -1;
		}

		// ȥ����ͷ׼������
		buffer_->DataUse(next_offset);

		// �����û�����
		if (handler)
			handler->OnReceive(conn, buffer_->GetCanUseBuffer(), out_size);

		// ����������Ųout_size���ֽ�
		buffer_->DataUse(out_size);

		// ���ݴ������
		if (buffer_->GetInOffset() == buffer_->GetUsedSize()) {
			cout << "HSRecvBufferSeperator::Process: ������ϣ�����������ƫ����" << endl;
			break;
		}
	}

	// �в�����������
	if (res == 0) {
		//static size_t max_read_length = buffer_->GetLength() - HS_NET_PACKET_MAX_SIZE;
		//if (buffer_->GetInOffset() >= max_read_length) {

		// ʣ�µ�δд�Ļ��峤��С��ĳ��ֵʱ���Ѳ�����������Ų����������ǰ�棬��֤�´����յ�����������
		if (buffer_->GetLeftInSize() <= HS_NET_PACKET_MAX_SIZE/8) {
			buffer_->MoveUnuseData2Begin();
			cout << "HSRecvBufferSeperator::Process: ʣ�µ�" << endl;
		}
	}

	return 0;
}

/**
 ���ͻ�������
 */

class HSSendDataPacker
{
public:
	HSSendDataPacker(HSSendBuffer* buffer = NULL);
	~HSSendDataPacker();

	void SetBuffer(HSSendBuffer* buffer) { buffer_ = buffer; }

	// д�����ݣ��ȷ�������ֱ������0��Ȼ����д�����壩
	int Write(const char* data, size_t len);

	// ͨ�����ӷ�������
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
		cout << "HSSendDataPacker::Process: ��������ʧ��" << endl;
		return -1;
	}

	return 1;
}

/**
 ���ڿͻ������ӷ�����
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

	// д��������
	int Send(const char* data, size_t len);
	// ֱ����socket�Ϸ��������Ƿ�װ��send����
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
		// ���ӳɹ�
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

	// ����������
	if (state_ == Connecting) {
		// �ȴ�д�¼�
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

			// ���ӳɹ�
			if (FD_ISSET(sock_, &fs)) {
				state_ = Connected;
				handler->OnConnect(this, res);
				return 1;
			}
		}
	}
	// ������
	else if (state_ == Connected) {
		// �ȴ����¼�
		int res = select(sock_+1, &fs, NULL, NULL, &tv);
		if (res < 0) {
			Close();
			// �Ͽ�����
			handler->OnDisconnect(this, res);
			return 0;
		}

		// �ɶ�
		if (res > 0) {

			// �����ݵ�������
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

		// ���¼������ִ꣬��д����
		if (packer_.Process(this) < 0) {
			Close();
			handler->OnDisconnect(this, res);
		}
	}

	return 0;
}

#endif