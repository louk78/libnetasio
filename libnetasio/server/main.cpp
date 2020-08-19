#include "..\libnetasio\lib_net_asio.h"

#pragma comment(lib,"libnetasio.lib")



class CEchoServer :public libnetasio::CServer
{
public:
	virtual void read_event_handler(libnetasio::SCONNCONTEXT& ConnContext)
	{
		printf("msg=%s socket=%p\n", ConnContext.msg.body, ConnContext.pConn);
		//ConnContext.pConn->send(ConnContext.Msg);
	}
	virtual void connect_event_handler(libnetasio::SCONNCONTEXT& ConnContext)
	{
		printf("与客户端建立连接成功...\n");
	}
	virtual void close_event_handler(libnetasio::SCONNCONTEXT& ConnContext)
	{
		printf("客户端已经断开连接...\n");
	}
};



void main()
{
	CEchoServer Server;
	Server.initialize(7878, 1, 1);
	Server.start();
}