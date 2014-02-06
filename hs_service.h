#ifndef __HS_SERVICE_H_20130826__
#define __HS_SERVICE_H_20130826__

#include "boost/asio.hpp"
#include "boost/thread.hpp"
#include "singleton.hpp"
#include <vector>

using namespace std;

static const size_t MAX_SERVICE_NUM = 4;

class HSServiceMgr : public Singleton<HSServiceMgr>
{
public:
	HSServiceMgr()
	{
		memset(threads_, 0, sizeof(threads_));
	}

	~HSServiceMgr()
	{
	}

	size_t ios_num() const { return MAX_SERVICE_NUM; }
	boost::asio::io_service& get_ios(size_t index) { return ios_[index]; }

	void start_service(size_t index)
	{
		if (index >= MAX_SERVICE_NUM)
			return;

		if (threads_[index] == NULL) {
			threads_[index] = new boost::thread(boost::bind(&boost::asio::io_service::run, &ios_[index]));
			//threads_[index]->join();
		}
	}

private:
	boost::asio::io_service ios_[MAX_SERVICE_NUM];
	boost::thread* threads_[MAX_SERVICE_NUM];
};

#endif