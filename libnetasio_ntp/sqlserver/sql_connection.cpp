#include "sql_connection.h"



CSqlConnection::CSqlConnection(const std::string& strHost,
	const std::string& strUserName,
	const std::string& strPassword,
	const std::string& strDataBase)
	:m_bConnected(false),
	m_pDriver(0)
{
	if (!m_pDriver)
	{
		m_pDriver = get_driver_instance();
	}
	if (m_pDriver)
	{
		m_pConn = m_pDriver->connect(strHost, strUserName, strPassword);

		m_pConn->setSchema(strDataBase);
		m_pStatement = m_pConn->createStatement();
	}

}

CSqlConnection::~CSqlConnection()
{
}

bool CSqlConnection::connected()
{
	return !m_pConn->isClosed();
}


sql::PreparedStatement* CSqlConnection::create_prepared_statement(const std::string& strSql)
{
	sql::PreparedStatement* pstmt = NULL;
	pstmt = m_pConn->prepareStatement(strSql.c_str());

	return pstmt;
}


sql::ResultSet* CSqlConnection::execute_query(const std::string& strSql)
{
	sql::ResultSet* res = NULL;

	res = m_pStatement->executeQuery(strSql);

	return res;
	
}

int CSqlConnection::execute(const std::string& strSql)
{
	int iRet = 0;

	iRet = m_pStatement->execute(strSql);
	return iRet;
}

int CSqlConnection::execute_update(const std::string& strSql)
{
	int iRet = 0;

	iRet = m_pStatement->executeUpdate(strSql);
	return iRet;
}
