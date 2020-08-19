#include "mysql_pool.h"



CMysqlPool::CMysqlPool(const std::string& strHost, const std::string& strUserName,
	const std::string& strPassword, const std::string& strDatabase,int iPoolSize,int iMaxConnNum)
	:m_strHost(strHost),
	m_strUserName(strUserName),
	m_strPassword(strPassword),
	m_strDatabase(strDatabase),
	m_iPoolSize(iPoolSize),
	m_iMaxConnNum(iMaxConnNum),
	m_iUsedConnNum(0)
{

}

CMysqlPool::~CMysqlPool()
{

}


bool CMysqlPool::init()
{
	for (int i = 0; i < m_iPoolSize; i++)
	{
		std::shared_ptr<CSqlConnection> conn;
		if (create_sql_conn(conn))
		{
			m_FreeList.push(conn);
		}
	}


	return m_FreeList.size() ? true:false;
}

bool CMysqlPool::create_sql_conn(std::shared_ptr<CSqlConnection>& conn)
{
	 conn = std::make_shared<CSqlConnection>(m_strHost, m_strUserName, m_strPassword, m_strDatabase);

	return conn->connected() ? true : false;
}

bool CMysqlPool::get_sql_conn(std::shared_ptr<CSqlConnection>& conn)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	while (m_FreeList.empty())
	{
		if (m_iUsedConnNum > m_iMaxConnNum)
		{
			m_MaxCondition.wait(m_Mutex);
			continue;
		}
		else
		{
			std::shared_ptr<CSqlConnection> temp;
			if (create_sql_conn(temp))
			{
				m_FreeList.push(temp);
			}
			else
			{
				return false;
			}
		}
	}

	conn = m_FreeList.front();
	m_FreeList.pop();
	m_iUsedConnNum++;
	return true;
}


void CMysqlPool::ret_sql_conn(std::shared_ptr<CSqlConnection> conn)
{
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_FreeList.push(conn);
		m_iUsedConnNum--;
	}
	m_MaxCondition.notify_one();
}