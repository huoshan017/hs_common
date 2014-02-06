#ifndef __HS_EVENT_HANDLER_H_20130807__
#define __HS_EVENT_HANDLER_H_20130807__

#include <iostream>

using namespace std;

class HSEventHandler
{
public:
	virtual void onConnect(unsigned int session_id)
	{
		cout << "HSEventHandler::onConnect" << endl;
	}

	virtual void onDisconnect(unsigned int session_id)
	{
		cout << "HSEventHandler::onDisconnect" << endl;
	}

	virtual void onError(unsigned int session_id, int error)
	{
		cout << "HSEventHandler::onError" << endl;
	}
 
	virtual void onReceive(unsigned int session_id, const char* data, size_t data_len)
	{
		cout << "HSEventHandler::onReceive" << endl;
	}

	virtual void onPoll()
	{
		//cout << "HSEventHandler::onPoll" << endl;
	}
};

#endif