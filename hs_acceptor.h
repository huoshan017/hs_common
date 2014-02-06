#ifndef __HS_ACCEPTOR_H__
#define __HS_ACCEPTOR_H__

#include <iostream>
#include "hs_sessionmgr.h"
#include "hs_eventhandler.h"

using namespace std;
using boost::asio::ip::tcp;

class HSAcceptor
{
public:
	HSAcceptor(boost::asio::io_service& io_service, const tcp::endpoint& ep, HSEventHandler* event_handler, unsigned int session_id = 0)
		: io_service_(io_service), acceptor_(io_service, ep), event_handler_(event_handler), session_id_(session_id)
	{
	}

	/**
	 * 开始监听
	 * param
	 * @session_id 默认0不指定连接管理器会自动分配一个，非零值则手动指定
	 */
	void start()
	{
		session_ptr new_session;
		if (session_id_ == 0)
			new_session = HSSessionMgr::Instance()->new_session(io_service_, CLIENT_SESSION_DATA_BUFFER_DEFAULT_SIZE, CLIENT_SESSION_DATA_BUFFER_DEFAULT_SIZE);
		else
			new_session = HSSessionMgr::Instance()->new_local_session(io_service_, session_id_, SERVER_SESSION_DATA_BUFFER_DEFAULT_SIZE, SERVER_SESSION_DATA_BUFFER_DEFAULT_SIZE);
		if (new_session.get() == NULL)
			return;

		new_session->clear();
		acceptor_.async_accept(new_session->socket(), boost::bind(&HSAcceptor::handle_accept, this, new_session, boost::asio::placeholders::error));
		cout << "start listen port " << acceptor_.local_endpoint().port() << " to waiting client connection ..." << endl;
	}

	void handle_accept(session_ptr new_session, const boost::system::error_code& error)
	{
		if (!error) {
			// 时间处理器
			new_session->attach_eventhandler(event_handler_);
			// 已连接
			new_session->connected();
			// 开始接受数据
			new_session->start();
			cout << "client " << new_session->socket().remote_endpoint().address().to_string() << ":" << new_session->socket().remote_endpoint().port() << " connected" << endl;
			cout << "new_session id: " << new_session->get_id() << endl;
		}

		//new_session.reset(new HSSession(io_service_));
		//session_ptr new_session2 = HSSessionMgr::Instance()->new_session(io_service_, SESSION_TYPE_PASSIVE, CLIENT_SESSION_DATA_BUFFER_DEFAULT_SIZE, CLIENT_SESSION_DATA_BUFFER_DEFAULT_SIZE);
		//acceptor_.async_accept(new_session2->socket(), boost::bind(&HSAcceptor::handle_accept, this, new_session2, boost::asio::placeholders::error));

		start();
	}

	void run()
	{
		io_service_.run();
	}

	void poll()
	{
		io_service_.poll();
		if (event_handler_)
			event_handler_->onPoll();
	}

private:
	boost::asio::io_service& io_service_;
	tcp::acceptor acceptor_;
	HSEventHandler* event_handler_;
	unsigned int session_id_;
};

#endif