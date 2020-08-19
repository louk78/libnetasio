#ifndef __LIBNETASIO_NET_PROTOCOL_H__
#define __LIBNETASIO_NET_PROTOCOL_H__

namespace libnetasio
{
#define MSG_PACKET_VERSION 0X66
#define SIZE_SOCKET_BUFFER 8192
#define SIZE_MSG_BODY (SIZE_SOCKET_BUFFER-sizeof(SMSGHEAD)-2*sizeof(unsigned long))

#define EVENT_TCP_SOCKET_READ 0x0001
#define EVENT_TCP_SOCKET_CLOSE 0X0002
#define EVENT_TCP_SOCKET_CONNECT 0X0003

#define CLOSE_TYPE_SERVER_DISCONNECT 0x0001



	typedef struct s_msg_head
	{
		unsigned char cbVersion;  //协议版本号
		unsigned char cbCheckCode; //校验码
		unsigned short iDataLength;//数据大小
		unsigned short wModuleID;//C/S是模块与模块通信,接收模块ID
	}SMSGHEAD, *PSMSGHEAD;

	typedef struct S_MSG//消息框架为head + body
	{
		s_msg_head head;
		unsigned char body[SIZE_MSG_BODY];
	}SMSG, *PSMSG;



}

#endif
