#ifndef __HS_SESSION_MGR_20130812_H__
#define __HS_SESSION_MGR_20130812_H__

#include "singleton.hpp"
#include "hs_define.h"
#include "hs_session.h"
#include <map>

using namespace std;

class HSSessionMgr : public Singleton<HSSessionMgr>
{
public:
	HSSessionMgr();
	session_ptr new_session(boost::asio::io_service& io_service, /*int type, */size_t recv_buff_size, size_t send_buff_size);
	session_ptr get_session(unsigned int id);
	bool remove_session(unsigned int id);
	session_ptr new_local_session(boost::asio::io_service& io_service, unsigned int sid, size_t recv_buff_size, size_t send_buff_size);
	session_ptr get_local_session(unsigned int id);

private:
	map<unsigned int, session_ptr> sessions_;
	map<unsigned int, session_ptr> local_sessions_;
	unsigned int curr_id_;
};

#endif