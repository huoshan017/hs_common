#ifndef __HS_SESSION_H__
#define __HS_SESSION_H__

#include <cstdlib>
#include <iostream>
#include <boost/aligned_storage.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "hs_define.h"
#include "hs_packetworker.h"
#include "hs_buffer.h"

using namespace std;
using boost::asio::ip::tcp;

class HSEventHandler;

class handler_allocator
	: private boost::noncopyable
{

public:
	handler_allocator() : in_use_(false)
	{
	}

	void* allocate(std::size_t size)
	{
		if (!in_use_ && size<storage_.size) {
			in_use_ = true;
			return storage_.address();
		} else {
			return ::operator new(size);
		}
	}

	void deallocate(void* pointer)
	{
		if (pointer == storage_.address()) {
			in_use_ = false;
		} else {
			::operator delete(pointer);
		}
	}

private:
	boost::aligned_storage<1024> storage_;
	bool in_use_;
};

template <typename Handler>
class custom_alloc_handler
{
public:
	custom_alloc_handler(handler_allocator& a, Handler h)
		: allocator_(a),
		  handler_(h)
	{
	}

	template <typename Arg1>
	void operator()(Arg1 arg1)
	{
		handler_(arg1);
	}

	template <typename Arg1, typename Arg2>
	void operator()(Arg1 arg1, Arg2 arg2)
	{
		handler_(arg1, arg2);
	}

	friend void* asio_handler_allocate(std::size_t size, custom_alloc_handler<Handler>* this_handler)
	{
		return this_handler->allocator_.allocate(size);
	}

	friend void asio_handler_deallocate(void* pointer, std::size_t size, custom_alloc_handler<Handler>* this_handler)
	{
		this_handler->allocator_.deallocate(pointer);
	}

private:
	handler_allocator& allocator_;
	Handler handler_;
};

template <typename Handler>
inline custom_alloc_handler<Handler> make_custom_alloc_handler(handler_allocator& a, Handler h)
{
	return custom_alloc_handler<Handler>(a, h);
}

/**
 * 会话连接
 */

class HSSession : public boost::enable_shared_from_this<HSSession>
{
public:
	HSSession(boost::asio::io_service& io_service, /*int type, */unsigned int id, size_t recv_buff_size=SESSION_DATA_BUFFER_DEFAULT_SIZE, size_t send_buff_size=SESSION_DATA_BUFFER_DEFAULT_SIZE);

	~HSSession();

	//int get_type() const { return type_; }

	unsigned int get_id() const { return id_; }

	tcp::socket& socket() {	return socket_; }

	bool is_connected() const { return connected_; }

	// 清理
	void clear();

	// 事件处理器
	void attach_eventhandler(HSEventHandler* event_handler) { event_handler_ = event_handler; }

	void start();

	void connected();

	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

	void handle_write();

	void write_result(const boost::system::error_code& error, size_t bytes_transferred);

	// 发送数据
	bool send(const char* data, size_t length);

private:
	int handle_error(int error_value);

private:
	tcp::socket socket_;
	HSRecvBuffer* recv_buffer_;												// 接收缓冲区
	HSSendBuffer* send_buffer_;												// 发送缓冲区
	bool handle_sending_;													// 正在发送
	handler_allocator allocator_;											// 处理器分配器
	//int type_;																// 会话类型
	unsigned int id_;														// 会话ID
	HSEventHandler* event_handler_;											// 事件处理器
	bool connected_;															// 是否已开始
};

typedef boost::shared_ptr<HSSession> session_ptr;

#endif 