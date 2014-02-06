#include "hs_session.h"
#include "hs_eventhandler.h"

HSSession::HSSession(boost::asio::io_service& io_service, /*int type, */unsigned int id, size_t recv_buff_size, size_t send_buff_size)
	: socket_(io_service), handle_sending_(false), /*type_(type), */id_(id), event_handler_(NULL), connected_(false)
{
	recv_buffer_ = new HSRecvBuffer(recv_buff_size);
	send_buffer_ = new HSSendBuffer(send_buff_size);
}

HSSession::~HSSession()
{
	if (recv_buffer_) {
		delete recv_buffer_;
		recv_buffer_ = NULL;
	}
	if (send_buffer_) {
		delete send_buffer_;
		send_buffer_ = NULL;
	}
}

void HSSession::clear()
{
	socket_.close();
	recv_buffer_->Clear(false);
	send_buffer_->Clear(false);
	handle_sending_ = false;
	connected_ = false;
}

void HSSession::start()
{
	static size_t max_read_length = recv_buffer_->GetLength();
	// 最多读取max_read_length-recv_offset_个字节
	socket_.async_read_some(boost::asio::buffer(recv_buffer_->GetInOffsetData(), recv_buffer_->GetLeftInSize()),
		make_custom_alloc_handler(allocator_, 
		boost::bind(&HSSession::handle_read,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)));
}

void HSSession::connected()
{
	if (event_handler_) {
		event_handler_->onConnect(id_);
	}
	connected_ = true;
}

void HSSession::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	cout << "handle_read begin" << endl;
	if (!error) {

		// 没有收到数据
		if (!bytes_transferred)
			return;

		// 已经收到的数据 
		recv_buffer_->DataIn(bytes_transferred);

		// 先解包再交给处理函数
		size_t next_offset = 0;
		size_t out_size = 0;
		int res = -1;
		while (true) {
			res = HSPacketWorker::DecodePacket(recv_buffer_->GetCanUseBuffer(), recv_buffer_->GetCanUseSize(), next_offset, out_size);

			// 没有完整的包，继续接收数据
			if (res == 0)
				break;

			// 数据接收有问题，可能是客户端发来的数据本身有问题，或者网络出现数据接收错误问题
			if (res < 0) {
				// 断开连接
				clear();
				return;
			}

			recv_buffer_->DataUse(next_offset);

			// 处理用户数据
			if (event_handler_)
				event_handler_->onReceive(id_, recv_buffer_->GetCanUseBuffer(), out_size);

			// 往后挪out_size个字节
			recv_buffer_->DataUse(out_size);

			// 数据处理完毕
			if (recv_buffer_->GetInOffset() == recv_buffer_->GetUsedSize()) {
				cout << "处理完毕，重新设置了偏移量" << endl;
				break;
			}
		}

		if (res == 0) {
			static size_t max_read_length = recv_buffer_->GetLength() - HS_NET_PACKET_MAX_SIZE;
			if (recv_buffer_->GetInOffset() >= max_read_length) {
				recv_buffer_->MoveUnuseData2Begin();
			}
		}

		// 继续读数据
		start();
	} else {
		// 错误码
		cout << "hadle_read error: " << error << endl;

		int res = handle_error(error.value());
		if (res == 0) {
			this->start();
		} else if (res < 0) {
			if (event_handler_) {
				event_handler_->onDisconnect(id_);
			}
			clear();
		}
	}
	cout << "handle_read end" << endl << endl;
}

int HSSession::handle_error(int error_value)
{
	int result = -1;
	if (error_value == 10053) {
		
	} else if (error_value == 10054) {
		
	}
	// 缓存不够
	else if (error_value == 10035) {
		result = 0;
	}
	return result;
}

void HSSession::handle_write()
{
	if (send_buffer_->GetCanSendSize() == 0)
		return;

	if (handle_sending_)
		return;

	static size_t max_send_length = send_buffer_->GetLength(); 

	cout << "handle write sent buffer: ";
	size_t i = 0;
	for (; i<send_buffer_->GetCanSendSize(); ++i) {
		cout << *(send_buffer_->GetCanSendBuffer()+i);
	}
	cout << ", length: " << send_buffer_->GetCanSendSize() << endl;
	socket_.async_write_some(boost::asio::buffer(send_buffer_->GetCanSendBuffer(), send_buffer_->GetCanSendSize()),
		make_custom_alloc_handler(allocator_,
		boost::bind(&HSSession::write_result,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)));

	handle_sending_ = true;
}

void HSSession::write_result(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error) {
		if (!bytes_transferred)
			return;

		cout << "bytes sent: " << bytes_transferred << ", current sent_offset_: " << send_buffer_->GetSentSize() << endl;

		// 已发送的总偏移
		send_buffer_->Sent(bytes_transferred);

		if (send_buffer_->GetSentSize() > send_buffer_->GetInOffset()) {
			cout << "sent size " << send_buffer_->GetSentSize() << " > in offset " << send_buffer_->GetInOffset() << endl;
		}

		if (send_buffer_->GetCanSendSize() > 0) {
			handle_write();
		}

	} else {
		cout << "write_result error: " << error << endl;
		int result = handle_error(error.value());
		// 很可能是socket缓冲区已满
		if (result == 0) {
			handle_write();
		} else if (result < 0) {
			if (event_handler_) {
				event_handler_->onDisconnect(id_);
			}
			clear();
		}
	}
	handle_sending_ = false;
}

// 发送数据
bool HSSession::send(const char* data, size_t length)
{
	static size_t s_send_buff_size = send_buffer_->GetLength();
	size_t out_size = 0;

	// 封包拷贝数据到缓冲
	int res = HSPacketWorker::EncodePacket(data, length, send_buffer_->GetInOffsetData(), send_buffer_->GetLeftInSize(), out_size);

	// 包过大无法处理
	if (res == -1)
		return false;

	// 封包失败，缓冲区不够，可能是发送过快，也可能是对方处理过慢，或者是网络的问题
	// 推迟发送，最多3次，分别延迟1，3，10秒，如果还失败，则认为网络连接有问题，断开
	if (res == 0) {
		handle_write();
		return true;
	}

	send_buffer_->DataIn(out_size);
	handle_write();

	return true;
}