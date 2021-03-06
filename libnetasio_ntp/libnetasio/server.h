#ifndef __LIBNETASIO_SERVER_H__
#define __LIBNETASIO_SERVER_H__
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "connection.h"
#include "net_event_handler.h"
#include "io_context_pool.h"

namespace libnetasio
{
	class _declspec(dllexport) CServer
		:private boost::noncopyable
	{
	public:
		CServer();
		virtual ~CServer(); 
	public:
		bool initialize(unsigned short iListenPort, unsigned short iIoContextNum);
		void start();
	private:
		void start_accept();
		void handle_accept(std::shared_ptr<CConnection> Conn, const boost::system::error_code& ec);
		void run();

	public:
		virtual void read_event_handler(SCONNCONTEXT& connContext) = 0;
		virtual void connect_event_handler(SCONNCONTEXT& connContext) = 0;
		virtual void close_event_handler(SCONNCONTEXT& connContext) = 0;
	private:
		boost::asio::io_context m_IoContext;
		boost::asio::ip::tcp::acceptor* m_pAcceptor;
		unsigned int  m_iListenPort;
		CIoContextPool* m_pIoContextPool;
	};
}


#endif
