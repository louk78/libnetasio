#ifndef __IO_CONTEXT_POOL_H__
#define __IO_CONTEXT_POOL_H__
#include "boost/asio.hpp"
#include <list>
#include <vector>

class CIoContextPool
{
public:
	explicit CIoContextPool(unsigned short iPoolSize);
	~CIoContextPool();
	void run();
	void stop();
	boost::asio::io_context& get_io_context();

private:
	typedef boost::asio::executor_work_guard<
		boost::asio::io_context::executor_type> io_context_work_t;
	std::list<io_context_work_t> m_lstWork;
	std::vector<std::shared_ptr<boost::asio::io_context>> m_vctIoContextPtr;
	unsigned short m_iNextPos;
	unsigned short m_iPoolSize;
};

#endif
