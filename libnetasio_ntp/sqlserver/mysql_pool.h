#ifndef __MYSQL_POOL_H__
#define __MYSQL_POOL_H__
#include <boost/noncopyable.hpp>
#include <queue>
#include <string>
#include <memory>
#include <condition_variable>
#include "sql_connection.h"
#include <mutex>

class CMysqlPool
{
public:
	CMysqlPool(const std::string& strHost,const std::string& strUserName,
		const std::string& strPassword,const std::string& strDatabase,int iPoolSize,int iMaxConnNum);
	~CMysqlPool();
	bool init();
	bool create_sql_conn(std::shared_ptr<CSqlConnection>& conn);
	bool get_sql_conn(std::shared_ptr<CSqlConnection>& conn);
	void ret_sql_conn(std::shared_ptr<CSqlConnection> conn);

private:
	std::queue<std::shared_ptr<CSqlConnection>> m_FreeList;
	int m_iUsedConnNum;
	int m_iMaxConnNum;
	int m_iPoolSize;
	std::mutex m_Mutex;
	std::condition_variable_any m_MaxCondition;
private:
	std::string m_strHost;
	std::string m_strUserName;
	std::string m_strPassword;
	std::string m_strDatabase;
};


#endif
