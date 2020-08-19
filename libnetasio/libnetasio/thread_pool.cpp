#include "thread_pool.h"


using namespace libnetasio;


CThreadPool::CThreadPool(int iThreadNum)
	:m_iThreadNum(iThreadNum)
{

}

CThreadPool::~CThreadPool()
{
	stop();
}

void CThreadPool::start()
{
	m_Running = true;
	for (int i = 0; i < m_iThreadNum; i++)
	{
		auto t = std::make_shared<std::thread>(std::bind(&CThreadPool::thread_work, this));
		m_vctThread.emplace_back(t);
	}
}

void CThreadPool::stop()
{
	m_Running = false;
	m_EmptyCondition.notify_all();
	for (auto iter = m_vctThread.begin(); iter != m_vctThread.end();)
	{
		(*iter)->join();
		++iter;
	}


	m_vctThread.clear();

}


void CThreadPool::thread_work()
{
	task_t task = NULL;

	while (m_Running)
	{
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			if (m_lstTask.empty())
			{
				m_EmptyCondition.wait(m_Mutex);
				continue;
			}
			task = m_lstTask.front();
			m_lstTask.pop_front();
		}
		task();
	}
}

void CThreadPool::post_task(task_t task)
{
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_lstTask.emplace_back(task);
	}
	m_EmptyCondition.notify_one();
}