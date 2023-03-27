#ifndef LZS_PR_TOOLS_TASK_QUEUE_H
#define LZS_PR_TOOLS_TASK_QUEUE_H 

#include <queue>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

typedef boost::function<void(void)> task_func_t;

class TaskQueue : boost::noncopyable {
public:
    void push_task(const task_func_t& task_func) {
        boost::unique_lock<boost::mutex> lock(m_task_mutex);
        m_task_queue.push(task_func);
    }
    
    bool get_task(task_func_t& task) {
        boost::unique_lock<boost::mutex> lock(m_task_mutex);
        if (m_task_queue.size() == 0) {
            // lock_guard方式,这里无需使用条件变量
            // m_cond.wait(lock);
            return false;
        }
        task = m_task_queue.front();
        m_task_queue.pop();
        return true;
    }

    int get_size() {
        return m_task_queue.size();
    }
private:
    std::queue<task_func_t> m_task_queue;
    boost::mutex m_task_mutex;
};

#endif