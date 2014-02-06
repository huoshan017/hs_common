#ifndef __HS_SERVER_20130802_H__
#define __HS_SERVER_20130802_H__

#include "hs_acceptor.h"
#include "hs_test_eventhandler.h"
#include "hs_connector.h"

#include <map>

using namespace std;

class HSClientServer
{
public:
	HSClientServer(boost::asio::io_service& ios) : ios_(ios), event_handler_(NULL), acceptor_(NULL)
	{
	}

	~HSClientServer()
	{
		clear();
	}

	void set_eventhandler(HSEventHandler* event_handler)
	{
		event_handler_ = event_handler;
	}

	void clear()
	{
		if (acceptor_) {
			delete acceptor_;
			acceptor_ = NULL;
		}
	}

	bool start(const tcp::endpoint& ep)
	{
		acceptor_ = new HSAcceptor(ios_, ep, event_handler_);
		acceptor_->start();
		return true;
	}

	void run()
	{
		if (acceptor_)
			acceptor_->run();
	}

	void poll()
	{
		if (acceptor_)
			acceptor_->poll();
	}

private:
	boost::asio::io_service& ios_;
	HSEventHandler* event_handler_;
	HSAcceptor* acceptor_;
};

#endif