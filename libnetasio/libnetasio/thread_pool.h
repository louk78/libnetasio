#ifndef __LIBNETASIO_THREAD_POOL_H__
#define __LIBNETASIO_THREAD_POOL_H__
#include <condition_variable>
#include <list>
#include <vector>
#include <thread>
#include <atomic>


namespace libnetasio
{
	class CThreadPool
	{
	public:
		typedef std::function<void(void)> task_t;
	public:
		CThreadPool(int iThreadNum=3);
		virtual ~CThreadPool();
	public:
		void  post_task(task_t task);
		void start();
		void stop();
		void thread_work();

	private:
		std::vector < std::shared_ptr<std::thread> > m_vctThread;
		std::condition_variable_any m_EmptyCondition;
		std::list<task_t> m_lstTask;
		std::mutex m_Mutex;
		std::atomic_bool m_Running;
		int m_iThreadNum;

	};
}





#endif
