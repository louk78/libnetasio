#include "server.h"
#include "boost/bind.hpp"


using namespace libnetasio;


CServer::CServer()
	:m_pAcceptor(NULL),
	m_pIoContextPool(NULL)
{

}
CServer::~CServer()
{
	if (m_pIoContextPool)
	{
		m_pIoContextPool->stop();
		delete m_pIoContextPool;
		m_pIoContextPool = NULL;
	}
}

bool CServer::initialize(unsigned short iListenPort,unsigned short iIoContextNum)
{
	bool bResult = false;

	m_iListenPort = iListenPort;
	if (m_pIoContextPool==NULL)
	{
		m_pIoContextPool = new CIoContextPool(iIoContextNum);
		if (m_pIoContextPool)
		{
			m_pAcceptor = new boost::asio::ip::tcp::acceptor(m_pIoContextPool->get_io_context());
		}
	}
	return m_pIoContextPool&&m_pAcceptor;
}

void CServer::start()
{
	boost::asio::ip::tcp::endpoint endpoint(
		boost::asio::ip::tcp::v4(),
		m_iListenPort
	);


	m_pAcceptor->open(endpoint.protocol());
	m_pAcceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_pAcceptor->bind(endpoint);
	m_pAcceptor->listen();


	start_accept();
	run();
}

void CServer::start_accept()
{
	auto conn = std::make_shared<CConnection>(m_pIoContextPool->get_io_context(),this);
	m_pAcceptor->async_accept(conn->socket(),
		boost::bind(&CServer::handle_accept, this,
			conn,
			boost::asio::placeholders::error
		));
}

void CServer::handle_accept(std::shared_ptr<CConnection> Conn,const boost::system::error_code& ec)
{
	if (!ec)
	{
		Conn->start();
		start_accept();
	}
}


void CServer::run()
{
	m_pIoContextPool->run();
}