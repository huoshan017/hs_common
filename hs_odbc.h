#ifndef __HS_ODBC_H_20130815__
#define __HS_ODBC_H_20130815__

#ifdef WIN32
#include <windows.h>
#endif

#include "hs_list.hpp"
#include <string>
#include "singleton.hpp"

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

using namespace std;

#ifdef WIN32
#pragma comment(lib, "odbc32.lib")
#else
#endif

typedef void (*query_callback)(bool, void* , unsigned int*, size_t, const char*);

struct HSSqlCallback
{
	unsigned int param[4];
	string sparam;
	query_callback callback;
};

class HSODBC : public Singleton<HSODBC>
{
public:
	class DataHandle
	{
	public:
		DataHandle() : hstmt_(NULL), doing_(false) {}
		SQLHSTMT& GetStatementHandle() { return hstmt_; }
		bool GetDoing() const { return doing_; }
		void SetDoing(bool doing=true) { doing_ = doing; }
		HSSqlCallback& GetCallback() { return callback_; }
		void SetCallback(const HSSqlCallback& callback) { callback_ = callback; }
		bool GetRowCount(size_t& count);
		bool Fetch();
		bool GetDataString(unsigned short column, char* string, size_t len);
		bool GetDataShort(unsigned short column, short& value);
		bool GetDataDouble(unsigned short column, double& value);
		bool GetDataFloat(unsigned short column, float& value);
		bool GetDataLong(unsigned short column, long& value);

	private:
		SQLHSTMT hstmt_;
		bool doing_;
		HSSqlCallback callback_;
	};

	HSODBC();
	~HSODBC();

	bool Connect(const char* server_name, const char* user_name, const char* password);
	void Disconnect();

	bool ExecuteQuery(const char* sql_text, size_t sql_len, struct HSSqlCallback& callback, size_t use_index = 0xffffffff);

	void Doing(); // 检测是否有结果返回，然后处理

private:
	size_t GetCanUseHandleIndex();

private:
	SQLHENV   henv_;		// xsj:环境句柄
	SQLHDBC   hdbc_;		// xsj:数据库连接句柄
	DataHandle  hstmt_[2];	// xsj:语句句柄，多个语句同时执行

	struct QueryAndCallback {
		HSSqlCallback callback_;
		string statement_;
	};
	HSList<QueryAndCallback> callback_list_; // 请求列表
};



#endif