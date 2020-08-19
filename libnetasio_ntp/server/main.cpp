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
		printf("��ͻ��˽������ӳɹ�...\n");
	}
	virtual void close_event_handler(libnetasio::SCONNCONTEXT& connContext)
	{
		printf("�ͻ����Ѿ��Ͽ�����...\n");
	}
};



void main()
{
	CEchoServer Server;
	Server.initialize(7878, 1);
	Server.start();
}