#include "..\libnetasio\lib_net_asio.h"
#include "mysql_pool.h"
#include "account.pb.h"
#include <windows.h>
#include <chrono>
#include <atomic>
#pragma comment(lib,"libnetasio.lib")
#pragma comment(lib,"libprotobufd.lib")
#pragma comment(lib,"mysqlcppconn.lib")



class CSqlServer :public libnetasio::CServer
{
public:
	CSqlServer()
	{
		//m_pSqlConn = new CSqlConnection("127.0.0.1", "root", "123789qq", "test");
		m_pMysqlPool = new CMysqlPool("127.0.0.1", "root", "123789qq", "test", 72, 100);
	}

public:
	virtual void read_event_handler(libnetasio::SCONNCONTEXT& connContext)
	{
		static std::atomic_int iCount = 0;
		if (iCount == 0)
		{
			m_tBegin = std::chrono::steady_clock::now();
		}
		printf("msg=%s socket=%p\n [tid==%d]", connContext.msg.body, connContext.pConn,GetCurrentThreadId());

		//connContext.pConn->send(connContext.msg);
		CAccount account;
		account.ParseFromArray(connContext.msg.body, connContext.msg.head.iDataLength);
		char sql[512] = { 0 };
		sprintf_s(sql,_countof(sql), "insert into accountinfo values('0','%s','%s');",account.strusername().c_str(),account.stremail().c_str());
			
		std::shared_ptr<CSqlConnection> sqlConn;
		if (m_pMysqlPool->get_sql_conn(sqlConn))
		{
			sqlConn->execute_update(sql);

			m_pMysqlPool->ret_sql_conn(sqlConn);
		}
		
		if (iCount == 999)
		{
			auto tEnd = std::chrono::steady_clock::now();
			auto tSpan = std::chrono::duration_cast<std::chrono::duration<double>>(tEnd - m_tBegin);
			printf("insert time = %f\n", tSpan);
		}

		iCount++;
	}
	virtual void connect_event_handler(libnetasio::SCONNCONTEXT& connContext)
	{
		printf("与客户端建立连接成功...\n");
	}
	virtual void close_event_handler(libnetasio::SCONNCONTEXT& connContext)
	{
		printf("客户端已经断开连接...\n");
	}
private:
	//CSqlConnection* m_pSqlConn;
	CMysqlPool* m_pMysqlPool;
	std::chrono::time_point<std::chrono::steady_clock> m_tBegin;


};



void main()
{
	CSqlServer Server;
	Server.initialize(7878, 72);
	Server.start();
}