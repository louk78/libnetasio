#include "..\libnetasio\lib_net_asio.h"
#include <tchar.h>
#include <string>

#pragma comment(lib,"libnetasio.lib")



void main()
{
	libnetasio::CClient client;
	client.connect("127.0.0.1", "7878");

	libnetasio::SMSG msg;
	
	
	
	

	for(int i=0;i<1000;i++)
	{
		ZeroMemory(&msg, sizeof(msg));
		std::string str = "helloworld!!!!made in china.";
		str += std::to_string(i);
		msg.Head.iDataLength = str.size();
		memcpy_s(msg.body, _countof(msg.body), str.c_str(), str.size());
		client.send(msg);
		//libnetasio::SMSG recvMsg;
		//ZeroMemory(&recvMsg, sizeof(recvMsg));
		//client.recv(recvMsg);
		//printf("%s\n", recvMsg.Body);
		//Sleep();
	}

	while (1)
	{

	}
	
}