#include "..\libnetasio\lib_net_asio.h"
#include <tchar.h>
#include <string>
#include "account.pb.h"

#pragma comment(lib,"libnetasio.lib")
#pragma comment(lib,"libprotobufd.lib")



void main()
{




	for(int i=0;i<1000;i++)
	{
		libnetasio::CClient client;
		client.connect("127.0.0.1", "7878");
		libnetasio::SMSG msg;
		CAccount account;
		account.set_strusername("xiaoqiang");
		std::string str = std::to_string(i);
		str += "abc@123.com";
		account.set_stremail(str);
		msg.head.iDataLength = account.ByteSizeLong();
		memcpy_s(msg.body, _countof(msg.body), account.SerializeAsString().c_str(), account.ByteSizeLong());
		client.send(msg);
	}

	while (1)
	{

	}

}