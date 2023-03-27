#include "task_queue.h"
#include "thread_pool.h"

void self_add(int& i) {
    ++i;
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