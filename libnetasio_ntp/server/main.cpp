#include "..\libnetasio\lib_net_asio.h"

#pragma comment(lib,"libnetasio.lib")



class CEchoServer :public libnetasio::CServer
{
public:
	virtual void read_event_handler(libnetasio::SCONNCONTEXT& connContext)
	{
		printf("msg=%s socket=%p\n", connContext.msg.body, connContext.pConn);
		connContext.pConn->send(connContext.msg);
	}
	virtual void connect_event_handler(libnetasio::SCONNCONTEXT& connContext)
	{
		printf("与客户端建立连接成功...\n");
	}
	virtual void close_event_handler(libnetasio::SCONNCONTEXT& connContext)
	{
		printf("客户端已经断开连接...\n");
	}
};



void main()
{
	CEchoServer Server;
	Server.initialize(7878, 1);
	Server.start();
}