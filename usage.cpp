#include "task_queue.h"
#include "thread_pool.h"

void self_add(int& i) {
    ++i;
}

int get_threadnum(int job_num) {
    uint32_t ncpu_num = get_nprocs();//the number of available processors
    int nthread_count = 0;    //线程个数
    if (job_num >= ncpu_num) {
        nthread_count = ncpu_num; //多线程
    } else if (job_num >= 2) {
        nthread_count = job_num; //多线程
    } else { 
        nthread_count = 1; //单线程
    }
    return nthread_count;
}

int main() {
    int task_num = 10000;
    int n_threadnum = get_threadnum(task_num);
    ThreadPool tp(n_threadnum);
    int num = 0;
    for (int i = 0; i < task_num; ++i) {
        
        task_func_t task = boost::bind(self_add, std::ref(num));
        tp.post(task);
    }
    tp.init();
    tp.wait();
}