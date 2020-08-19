#ifndef __LIBNETASIO_CLIENT_H__
#define __LIBNETASIO_CLIENT_H__
#include <string>
#include "net_protocol.h"
#include <boost/asio.hpp>

namespace libnetasio
{
	class _declspec(dllexport) CClient
	{
	public:
		CClient();
		virtual ~CClient();
	public:
		bool connect(const std::string& strHost, const std::string& strPort, unsigned long iTimeOut = 3);
		bool send(SMSG& msg,unsigned long iTimeout=3);
		bool send(const char* pData,unsigned short iDataLen, unsigned long iTimeout = 3);
		int recv(SMSG& msg,short iTimeout = 3);
		int recv(char* pBuffer, unsigned short iBufferSize, unsigned short iTimeout = 3);
		void run(unsigned long iTimeout);
	private:
		boost::asio::io_context m_IoContext;
		boost::asio::ip::tcp::socket m_Socket;

	};
}




#endif
