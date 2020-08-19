#include "client.h"
#include "boost/lambda/lambda.hpp"



using namespace libnetasio;


CClient::CClient()
	:m_Socket(m_IoContext)
{

}

CClient::~CClient()
{

}

bool CClient::connect(const std::string& strHost, const std::string& strPort, unsigned long iTimeOut /* = 3 */)
{
	bool bResult = false;
	boost::system::error_code ec;

	boost::asio::ip::tcp::resolver resolver(m_IoContext);
	boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(strHost, strPort);

	boost::asio::async_connect(m_Socket, endpoints,

		boost::lambda::var(ec) = boost::lambda::_1
	);
	run(iTimeOut);
	if (!ec)
	{
		bResult = true;
	}

	return bResult;
}


bool CClient::send(SMSG& msg, unsigned long iTimeout/* =3 */)
{
	bool bResult = false;
	boost::system::error_code ec;

	boost::asio::async_write(m_Socket,
		boost::asio::buffer(&msg, sizeof(SMSGHEAD)+ msg.head.iDataLength),
		boost::lambda::var(ec) = boost::lambda::_1
	);

	run(iTimeout);
	if (!ec)
	{
		bResult = true;
	}


	return bResult;
	
}

bool CClient::send(const char* pData, unsigned short iDataLen, unsigned long iTimeout)
{
	bool bResult = false;
	boost::system::error_code ec;

	boost::asio::async_write(m_Socket,
		boost::asio::buffer(pData,iDataLen),
		boost::lambda::var(ec) = boost::lambda::_1
	);

	run(iTimeout);
	if (!ec)
	{
		bResult = true;
	}


	return bResult;
}


int CClient::recv(SMSG& msg, short iTimeout /* = 3 */)
{
	boost::system::error_code ec;
	size_t n;

	boost::asio::async_read(m_Socket,
		boost::asio::buffer(&msg.head,sizeof(SMSGHEAD)),
		(boost::lambda::var(ec)=boost::lambda::_1,boost::lambda::var(n)=boost::lambda::_2)
		);

	run(iTimeout);

	if (!ec)
	{
		unsigned short iDataLength = msg.head.iDataLength;
		iDataLength = iDataLength <= SIZE_MSG_BODY ? iDataLength : SIZE_MSG_BODY;
		boost::asio::async_read(m_Socket,
			boost::asio::buffer(msg.body, iDataLength),
			(boost::lambda::var(ec) = boost::lambda::_1, boost::lambda::var(n) = boost::lambda::_2)
		);
		run(iTimeout);
		return n;

	}

	return 0;
}

int CClient::recv(char* pBuffer, unsigned short iBufferSize, unsigned short iTimeout)
{
	boost::system::error_code ec;
	size_t n;

	boost::asio::async_read(m_Socket,
		boost::asio::buffer(pBuffer,iBufferSize),
		(boost::lambda::var(ec) = boost::lambda::_1, boost::lambda::var(n) = boost::lambda::_2)
	);

	run(iTimeout);

	return n;
}

void CClient::run(unsigned long iTimeout)
{
	std::chrono::steady_clock::duration timeout = std::chrono::seconds(iTimeout);
	m_IoContext.restart();
	m_IoContext.run_for(timeout);
	if (!m_IoContext.stopped())
	{
		m_Socket.close();
		m_IoContext.run();
	}
}