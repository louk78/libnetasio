#ifndef __SQL_CONNECTION_H__
#define __SQL_CONNECTION_H__
#include <string>
#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"



class CSqlConnection
{
public:
	explicit CSqlConnection(const std::string& strHost,
		const std::string& strUserName,
		const std::string& strPassword,
		const std::string& strDataBase
	);
	~CSqlConnection();
	bool connected();
	sql::PreparedStatement* create_prepared_statement(const std::string& strSql);
	sql::ResultSet* execute_query(const std::string& strSql);
	int execute(const std::string& strSql);
	int execute_update(const std::string& strSql);

private:
	sql::Connection* m_pConn;
	sql::Statement* m_pStatement;
	bool m_bConnected;
	sql::Driver* m_pDriver;
};


#endif
