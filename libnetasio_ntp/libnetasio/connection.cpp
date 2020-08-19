#include "connection.h"
#include "boost/bind.hpp"
#include "net_structs.h"


using namespace libnetasio;
bool g_bBlock = true;

CConnection::CConnection(boost::asio::io_context& IoContext, CServer* pServer)
	:
	m_IoContext(IoContext),
	m_Socket(IoContext),
	m_pServer(pServer)
{
	
}

CConnection::~CConnection()
{

}

boost::asio::ip::tcp::socket&  CConnection::socket()
{
	return m_Socket;
}

void CConnection::start()
{
	memset(&m_ConnContext, 0, sizeof(SCONNCONTEXT));
	memset(&m_Msg, 0, sizeof(SMSG));
	m_ConnContext.pConn = this;
	net_event_notify(EVENT_TCP_SOCKET_CONNECT,m_ConnContext);
	boost::asio::async_read(m_Socket,
		boost::asio::buffer(&m_Msg.head, sizeof(SMSGHEAD)),
		boost::bind(
			&CConnection::handler_read_header, shared_from_this(),
			boost::asio::placeholders::error));
}

void CConnection::handler_read_header(const boost::system::error_code& ec)
{
	if (!ec)
	{
/*
		if (g_bBlock)
		{
			Sleep(60000);
			g_bBlock = false;
		}*/
		unsigned short iDataLength = m_Msg.head.iDataLength;
		iDataLength = (iDataLength <= SIZE_MSG_BODY) ? iDataLength : SIZE_MSG_BODY;
		boost::asio::async_read(m_Socket,
			boost::asio::buffer(m_Msg.body, iDataLength),
			boost::bind(
				&CConnection::handler_read_body, shared_from_this(),
				boost::asio::placeholders::error));

	}
	else
	{
		net_event_notify(EVENT_TCP_SOCKET_CLOSE, m_ConnContext);
	}
}


void CConnection::handler_read_body(const boost::system::error_code& ec)
{
	if (!ec)
	{
		m_ConnContext.msg = m_Msg;
		
		net_event_notify(EVENT_TCP_SOCKET_READ, m_ConnContext);
		memset(&m_Msg, 0, sizeof(SMSG));
		boost::asio::async_read(m_Socket,
			boost::asio::buffer(&m_Msg.head, sizeof(SMSGHEAD)),
			boost::bind(
				&CConnection::handler_read_header, shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		net_event_notify(EVENT_TCP_SOCKET_CLOSE, m_ConnContext);
	}
}



void CConnection::net_event_notify(int iEventType, SCONNCONTEXT& connContext)
{
	switch (iEventType)
	{
	case EVENT_TCP_SOCKET_CONNECT:
	{
		m_pServer->connect_event_handler(connContext);
		break;
	}
	case EVENT_TCP_SOCKET_READ:
	{
		m_pServer->read_event_handler(connContext);
		break;
	}
	case EVENT_TCP_SOCKET_CLOSE:
	{
		m_pServer->close_event_handler(connContext);
		break;
	}
	default:
		break;
	}
}


void CConnection::close()
{
	m_Socket.close();
}


void CConnection::send(SMSG& msg)
{
	boost::asio::post(m_IoContext,
		boost::bind(&CConnection::do_write,
			shared_from_this(), msg));
}

void CConnection::do_write(SMSG& msg)
{
	BOOL bIsWriting;
	bIsWriting = m_WriteMsgs.size() > 0 ? true : false;
	m_WriteMsgs.push_back(msg);
	if (!bIsWriting)
	{
		boost::asio::async_write(
			m_Socket,
			boost::asio::buffer(&m_WriteMsgs.front(), sizeof(SMSGHEAD) + m_WriteMsgs.front().head.iDataLength),
			boost::bind(&CConnection::handle_write, shared_from_this(),
				boost::asio::placeholders::error)
		);
	}
}

void CConnection::handle_write(const boost::system::error_code& ec)
{
	if (!ec)
	{
		m_WriteMsgs.pop_front();
		if (!m_WriteMsgs.empty())
		{
			boost::asio::async_write(
				m_Socket,
				boost::asio::buffer(&m_WriteMsgs.front(),sizeof(SMSGHEAD)+m_WriteMsgs.front().head.iDataLength),
				boost::bind(&CConnection::handle_write,shared_from_this(),
					boost::asio::placeholders::error)
				);
		}
	}
	else
	{
		net_event_notify(EVENT_TCP_SOCKET_CLOSE, m_ConnContext);
	}
}

