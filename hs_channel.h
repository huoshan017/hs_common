#ifndef __HS_CHANNEL_H_20130820__
#define __HS_CHANNEL_H_20130820__

#include "hs_acceptor.h"
#include "hs_connector.h"


class HSChannel
{
public:
	HSChannel(boost::asio::io_service& ios)
		: ios_(ios), acceptor_(NULL), connector_(NULL), event_handler_(NULL), session_id_(0), state_(0)
	{
	}

	~HSChannel()
	{
		clear();
	}

	void clear()
	{
		if (acceptor_) {
			delete acceptor_;
			acceptor_ = NULL;
		}
		if (connector_) {
			delete connector_;
			connector_ = NULL;
		}
		event_handler_ = NULL;
		state_ = 0;
	}

	void init(const tcp::endpoint& ep, HSEventHandler* event_handler, unsigned int session_id)
	{
		end_point_ = ep;
		event_handler_ = event_handler;
		session_id_ = session_id;
	}

	void set_endpoint(const tcp::endpoint& ep)
	{
		end_point_ = ep;
	}

	void set_eventhandler(HSEventHandler* event_handler)
	{
		event_handler_ = event_handler;
	}

	void set_session_id(unsigned int session_id)
	{
		session_id_ = session_id;
	}

	void listen()
	{
		if (!session_id_ || !event_handler_)
			return;

		if (!acceptor_)
			acceptor_ = new HSAcceptor(ios_, end_point_, event_handler_, session_id_);

		acceptor_->start();
		state_ = 2;
	}

	void connect(size_t timeout)
	{
		if (!session_id_ || !event_handler_)
			return;

		if (!connector_)
			connector_ = new HSConnector(ios_);

		connector_->connect(end_point_, session_id_, event_handler_, timeout);
		state_ = 1;
	}

	void handle_connect_failed()
	{
		listen();
	}

	void poll()
	{
		if (state_ == 1 && connector_) {
			if (connector_->is_connected_failed()) {
				handle_connect_failed();
			} else
				connector_->poll();
		}
		else if (state_ == 2 && acceptor_) {
			acceptor_->poll();
		}
	}

private:
	boost::asio::io_service& ios_;
	tcp::endpoint end_point_;
	HSAcceptor* acceptor_;
	HSConnector* connector_;
	HSEventHandler* event_handler_;
	unsigned int session_id_;
	int state_; // 0-未开始 1-连接状态 2-等待连接状态 3-连接成功 -1-连接失败
};

#endif