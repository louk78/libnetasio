#include "io_context_pool.h"
#include <boost\bind.hpp>


CIoContextPool::CIoContextPool(unsigned short iPoolSize)
	:m_iPoolSize(iPoolSize),
	m_iNextPos(0)
{
	for (unsigned short i = 0; i < m_iPoolSize; ++i)
	{
		auto IoContextPtr = std::make_shared<boost::asio::io_context>();
		m_vctIoContextPtr.push_back(IoContextPtr);
		m_lstWork.push_back(boost::asio::make_work_guard(*IoContextPtr));
		
	}
}


CIoContextPool::~CIoContextPool()
{

}


void CIoContextPool::run()
{
	std::vector<std::shared_ptr<std::thread>> vctThreads;
	for (int i = 0; i < m_vctIoContextPtr.size(); ++i)
	{
		auto thread = std::make_shared<std::thread>(
			boost::bind(&boost::asio::io_context::run, m_vctIoContextPtr[i])
			);
		vctThreads.push_back(thread);
	}
	for (int i = 0; i < vctThreads.size(); ++i)
	{
		vctThreads[i]->join();
	}
}


void CIoContextPool::stop()
{
	for (unsigned i = 0; i < m_vctIoContextPtr.size(); ++i)
	{
		m_vctIoContextPtr[i]->stop();
	}
}

boost::asio::io_context& CIoContextPool::get_io_context()
{
	boost::asio::io_context& IoContext = *m_vctIoContextPtr[m_iNextPos];
	++m_iNextPos;
	if (m_iNextPos == m_vctIoContextPtr.size())
	{
		m_iNextPos = 0;
	}

	return IoContext;
}