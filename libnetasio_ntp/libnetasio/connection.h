#ifndef __LIBNETASIO_CONNECTION_H__
#define __LIBNETASIO_CONNECTION_H__
#include "boost/asio.hpp"
#include <memory>
#include "net_protocol.h"
#include "net_event_handler.h"
#include "net_structs.h"
#include "server.h"
#include <deque>




namespace libnetasio
{

	class CServer;
	class _declspec(dllexport) CConnection
		:public std::enable_shared_from_this<CConnection>
	{
	public:
		CConnection(boost::asio::io_context& IoContext,CServer* pServer);
		virtual ~CConnection();
	public:
		boost::asio::ip::tcp::socket& socket();
		void start();
		void close();
		void send(SMSG& msg);
	private:
		void handler_read_header(const boost::system::error_code& ec);
		void handler_read_body(const boost::system::error_code& ec);
		void net_event_notify(int iEventType,SCONNCONTEXT& connContext);
		void do_write(SMSG& msg);
		void handle_write(const boost::system::error_code& ec);
	private:
		boost::asio::io_context& m_IoContext;
		boost::asio::ip::tcp::socket m_Socket;
		SMSG m_Msg;
		SCONNCONTEXT m_ConnContext;
		CServer* m_pServer;
		std::deque<SMSG> m_WriteMsgs;

	};
}



#endif
