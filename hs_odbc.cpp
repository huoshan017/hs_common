#include "hs_odbc.h"
#include <iostream>
#include <algorithm>
#include <list>

#define LOGIN_TIMEOUT 30

using namespace std;

HSODBC::HSODBC() : henv_(NULL), hdbc_(NULL)
{
}

HSODBC::~HSODBC()
{
}

bool HSODBC::Connect(const char* server_name, const char* user_name, const char* password)
{
	// xsj:分配环境句柄
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv_);
	// xsj:设置环境属性
	SQLSetEnvAttr(henv_, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	// xsj:分配连接句柄
	SQLAllocHandle(SQL_HANDLE_DBC, henv_, &hdbc_);
	// xsj:设置连接属性
	SQLSetConnectAttr(hdbc_, SQL_LOGIN_TIMEOUT, (void*)LOGIN_TIMEOUT, 0);

	SQLRETURN retcode = SQLConnect(hdbc_,
                         (SQLCHAR *)server_name, SQL_NTS,
                         (SQLCHAR *)user_name, SQL_NTS,
                         (SQLCHAR *)password, SQL_NTS);
	if (retcode == SQL_SUCCESS)
	{
		cout << "SQLConnect - SQL_SUCCESS" << endl;
	}
	else if (retcode == SQL_SUCCESS_WITH_INFO)
	{
		cout << "SQLConnect - SQL_SUCCESS_WITH_INFO";

		SQLCHAR Sqlstate[10];
		SQLCHAR MessageText[100];

		SQLGetDiagRec(SQL_HANDLE_DBC,
                      hdbc_,
                      1,
                      Sqlstate,
                      NULL,
                      MessageText,
                      100,
                      NULL);
		cout << " [" << Sqlstate << " - " << MessageText << "]" << endl;
	}
	else
	{
		cout << "SQLConnect - error" << endl;

		SQLCHAR Sqlstate[10];
		SQLCHAR MessageText[100];

		SQLGetDiagRec(SQL_HANDLE_DBC,
                      hdbc_,
                      1,
                      Sqlstate,
                      NULL,
                      MessageText,
                      100,
                      NULL);
		cout << " [" << Sqlstate << " - " << MessageText << "]" << endl;

		return false;
	}

	size_t c = sizeof(hstmt_)/sizeof(DataHandle);
	for (size_t i=0; i<c; ++i) {
		SQLAllocHandle(SQL_HANDLE_STMT, hdbc_, &hstmt_[i].GetStatementHandle());

		// 设置异步模式
		//retcode = SQLSetStmtOption(hstmt_[i].GetStatementHandle(), SQL_ASYNC_ENABLE, SQL_ASYNC_ENABLE_ON);
		retcode = SQLSetStmtAttr(hstmt_[i].GetStatementHandle(), SQL_ATTR_ASYNC_ENABLE, (SQLPOINTER)SQL_ASYNC_ENABLE_ON, 0);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
			cout << "ODBC not support async mode" << endl;
			return false;
		}
	}

	return true;
}

void HSODBC::Disconnect()
{
	size_t c = sizeof(hstmt_)/sizeof(DataHandle);
	// xsj:释放语句句柄
	for (size_t i=0; i<c; ++i) {
		SQLFreeStmt(hstmt_[i].GetStatementHandle(), SQL_CLOSE);
	}
	// xsj:释放数据库连接句柄
	SQLFreeHandle(SQL_HANDLE_DBC,hdbc_);
	// xsj:释放环境句柄
	SQLFreeHandle(SQL_HANDLE_ENV,henv_);
	// xsj:断开连接
	SQLDisconnect(hdbc_);
}

size_t HSODBC::GetCanUseHandleIndex()
{
	//static SQLHSTMT null_handle = NULL;
	static size_t c = sizeof(hstmt_)/sizeof(DataHandle);
	size_t i = 0;
	for (; i<c; ++i) {
		if (!hstmt_[i].GetDoing()) {
			return i;
		}
	}
	return c+100;
}

bool HSODBC::ExecuteQuery(const char* sql_text, size_t sql_len, struct HSSqlCallback& callback, size_t use_index)
{
	static size_t c = sizeof(hstmt_)/sizeof(DataHandle);
	size_t index = 0;
	if (use_index > c)
		index = GetCanUseHandleIndex();
	else
		index = use_index;
	if (index >= c) {
		QueryAndCallback q;
		q.callback_ = callback;
		q.statement_ = sql_text;
		callback_list_.PushValue(q);
		return false;
	}
	// xsj:执行SQL语句
	SQLRETURN retcode = SQLExecDirect(hstmt_[index].GetStatementHandle(), (SQLCHAR*)sql_text, SQL_NTS);
	if (retcode == SQL_STILL_EXECUTING) {
		hstmt_[index].SetDoing();
		hstmt_[index].SetCallback(callback);
		return false;
	}

	callback.callback(retcode!=SQL_SUCCESS, (void*)&hstmt_[index], callback.param, sizeof(callback.param)/sizeof(unsigned int), callback.sparam.c_str());

	if (retcode == SQL_SUCCESS_WITH_INFO) {
		static SQLCHAR Sqlstate[10];
		static SQLCHAR MessageText[100];
		cout << "SQLExecute - SQL_SUCCESS_WITH_INFO" << endl;
		SQLGetDiagRec(SQL_HANDLE_STMT,
						hstmt_[index].GetStatementHandle(),
						1,
						Sqlstate,
						NULL,
						MessageText,
						sizeof(MessageText),
						NULL);
		cout << " [" << Sqlstate << " - " << MessageText << "]" << endl;
	}

	if (retcode != SQL_SUCCESS)
		return false;

	return true;
}

void HSODBC::Doing()
{
	static size_t c = sizeof(hstmt_)/sizeof(DataHandle);
	
	static SQLCHAR Sqlstate[10];
	static SQLCHAR MessageText[100];

	bool doing = false;
	SQLRETURN retcode = -1;
	for (size_t i=0; i<c; ++i) {
		doing = hstmt_[i].GetDoing();
		if (doing) {
			SQLRETURN retcode = SQLExecDirect(hstmt_[i].GetStatementHandle(), NULL, 0);
			if (retcode == SQL_STILL_EXECUTING)
				continue;

			hstmt_[i].SetDoing(false);

			if (retcode == SQL_ERROR)
				continue;

			if (retcode == SQL_SUCCESS_WITH_INFO) {
				cout << "SQLExecute - SQL_SUCCESS_WITH_INFO" << endl;
				SQLGetDiagRec(SQL_HANDLE_STMT,
								hstmt_[i].GetStatementHandle(),
								1,
								Sqlstate,
								NULL,
								MessageText,
								sizeof(MessageText),
								NULL);
				cout << " [" << Sqlstate << " - " << MessageText << "]" << endl;
			}

			HSSqlCallback& cb = hstmt_[i].GetCallback();
			cb.callback(retcode!=SQL_SUCCESS, (void*)&hstmt_[i], cb.param, sizeof(cb.param)/sizeof(unsigned int), cb.sparam.c_str());
		}
	}

	static QueryAndCallback* qc = NULL;
	
	while (callback_list_.GetCount()) {
		if (callback_list_.GetHeadValue(qc)) {
			size_t i = GetCanUseHandleIndex();
			if (i >= c)
				return;
			ExecuteQuery(qc->statement_.c_str(), qc->statement_.length(), qc->callback_, i);
			callback_list_.RemoveHead();
		}
	}
}

bool HSODBC::DataHandle::GetRowCount(size_t& count)
{
	SQLLEN c = 0;
	if (SQLRowCount(hstmt_, &c) == SQL_SUCCESS) {
		count = c;
		return true;
	}
	return false;
}

bool HSODBC::DataHandle::Fetch()
{
	return SQLFetch(hstmt_) == SQL_SUCCESS;
}

bool HSODBC::DataHandle::GetDataString(unsigned short column, char* string, size_t len)
{
	return SQLGetData(hstmt_, column, SQL_C_CHAR, string, len, NULL) == SQL_SUCCESS; 
}

bool HSODBC::DataHandle::GetDataShort(unsigned short column, short& value)
{
	return SQLGetData(hstmt_, column, SQL_C_SHORT, &value, 0, NULL) == SQL_SUCCESS;
}

bool HSODBC::DataHandle::GetDataDouble(unsigned short column, double& value)
{
	return SQLGetData(hstmt_, column, SQL_C_DOUBLE, &value, 0, NULL) == SQL_SUCCESS;
}

bool HSODBC::DataHandle::GetDataFloat(unsigned short column, float& value)
{
	return SQLGetData(hstmt_, column, SQL_C_FLOAT, &value, 0, NULL) == SQL_SUCCESS;
}

bool HSODBC::DataHandle::GetDataLong(unsigned short column, long& value)
{
	return SQLGetData(hstmt_, column, SQL_C_LONG, &value, 0, NULL) == SQL_SUCCESS;
}
