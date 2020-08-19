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
		printf("��ͻ��˽������ӳɹ�...\n");
	}
	virtual void close_event_handler(libnetasio::SCONNCONTEXT& ConnContext)
	{
		printf("�ͻ����Ѿ��Ͽ�����...\n");
	}
};



void main()
{
	CEchoServer Server;
	Server.initialize(7878, 1, 1);
	Server.start();
}