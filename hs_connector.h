#ifndef __HS_CONNECTOR_H_20130815__
#define __HS_CONNECTOR_H_20130815__


#include <iostream>
#include <boost/bind.hpp>
#include "hs_sessionmgr.h"
#include "hs_eventhandler.h"

using namespace std;
using boost::asio::ip::tcp;

class HSConnector
{
public:
	HSConnector(boost::asio::io_service& ios)
		: io_service_(ios), connect_timer_(ios), connected_failed_(false), event_handler_(NULL)
	{
	}

	/**
	 * param:
	 * @ep 连接的端点
	 * @session_id 指定的连接会话ID
	 */
	void connect(const tcp::endpoint& ep, unsigned int session_id, HSEventHandler* event_handler, size_t timeout)
	{
		session_ptr new_session = HSSessionMgr::Instance()->new_local_session(io_service_, session_id, SERVER_SESSION_DATA_BUFFER_DEFAULT_SIZE, SERVER_SESSION_DATA_BUFFER_DEFAULT_SIZE);
		if (new_session->is_connected())
			return;

		new_session->clear();

		connect_timer_.expires_from_now(boost::posix_time::seconds(timeout));
		connect_timer_.async_wait(boost::bind(&HSConnector::handle_timeout, this));
		new_session->socket().async_connect(ep, boost::bind(&HSConnector::handle_connect, this, new_session, boost::asio::placeholders::error));
		new_session->attach_eventhandler(event_handler);
		event_handler_ = event_handler;
		connected_failed_ = false;
		cout << "to connect " << ep.address().to_string() << "(" << ep.port() << ")" << endl;
	}

	void handle_connect(session_ptr session, const boost::system::error_code& error)
	{
		if (!error) {
			//session->attach_eventhandler(event_handler_);
			session->connected();
		} else {
			// 
			cout << "handle connect get error: " << error << endl;
			handle_timeout();
		}
	}

	void handle_timeout()
	{
		connected_failed_ = true;
		connect_timer_.cancel();
	}

	bool is_connected_failed() const { return connected_failed_; }

	void poll()
	{
		io_service_.poll();
		if (event_handler_)
			event_handler_->onPoll();
	}

private:
	boost::asio::io_service& io_service_;
	boost::asio::deadline_timer connect_timer_;
	bool connected_failed_;
	HSEventHandler* event_handler_;
};

#endif