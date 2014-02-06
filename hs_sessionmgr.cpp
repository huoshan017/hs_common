#include "hs_sessionmgr.h"
#include <stdlib.h>

HSSessionMgr::HSSessionMgr() : curr_id_(0)
{
}

session_ptr HSSessionMgr::new_session(boost::asio::io_service& io_service, /*int type, */size_t recv_buff_size, size_t send_buff_size)
{
	unsigned int id = ++curr_id_;
	session_ptr new_session(new HSSession(io_service, /*type, */id, recv_buff_size, send_buff_size));
	sessions_.insert(make_pair(id, new_session));
	return new_session;
}

session_ptr HSSessionMgr::get_session(unsigned int id)
{
	map<unsigned int, session_ptr>::iterator it = sessions_.find(id);
	if (it == sessions_.end())
		return session_ptr();
	return it->second;
}

bool HSSessionMgr::remove_session(unsigned int id)
{
	if (sessions_.find(id) == sessions_.end())
		return false;

	sessions_.erase(id);
	return true;
}

session_ptr HSSessionMgr::new_local_session(boost::asio::io_service& io_service, unsigned int sid, size_t recv_buff_size, size_t send_buff_size)
{
	map<unsigned int, session_ptr>::iterator it = local_sessions_.find(sid);
	if (it != local_sessions_.end()) {
		if (!it->second->is_connected()) {
			return it->second;
		} else {
			return session_ptr();
		}
	}

	session_ptr new_session(new HSSession(io_service, sid, recv_buff_size, send_buff_size));
	local_sessions_.insert(make_pair(sid, new_session));
	return new_session;
}

session_ptr HSSessionMgr::get_local_session(unsigned int id)
{
	map<unsigned int, session_ptr>::iterator it = local_sessions_.find(id);
	if (it == local_sessions_.end())
		return session_ptr();
	return it->second;
}