#ifndef LZS_PR_TOOLS_THREAD_POOL_H
#define LZS_PR_TOOLS_THREAD_POOL_H 

include "task_queue.h"

class ThreadPool : public boost::noncopyable {
public:
    ThreadPool(int num) : m_thread_num(num), is_run(false) {}
    virtual ~ThreadPool(){}

    // 初始化
    void init() {
        is_run = true;
        if (m_thread_num <= 0) {
            return;
        }

        for (int i = 0; i < m_thread_num; ++i) {
            m_thread_group.add_thread(new boost::thread(boost::bind(&ThreadPool::run, this)));
        }
    }

    void stop() {
        is_run = false;
    }

    void post(const task_func_t& task) {
        m_queue.emplace_back(task);
    }

    void wait() {
        m_thread_group.join_all();
    }

private:
    TaskQueue m_queue;
    boost::thread_group m_thread_group;
    int m_thread_num;
    volatile bool is_run;

    void run() {
        while (is_run) {
            task_func_t task;
            bool ret = m_queue.get_task(task);
            if (false == ret) {
                break;
            }
            task();
        }
    }
};

#endif