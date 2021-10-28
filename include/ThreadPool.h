/*线程池用于实现多线程处理报文*/
#include <pthread.h>
#include <thread>
#include <cstdio>
#include <exception>
#include <queue>
#include <functional>
#include <mutex>
#include <future>
#include <utility>
#include <condition_variable>

#include "SafeQueue.h"

using namespace std;

class ThreadPool
{

    using task = function<void()>;

private:
    int m_numThreads;//线程的数目
    bool m_is_running;//进程池的状态
    
    mutex m_conditional_mutex;
    condition_variable m_conditional_lock;

    SafeQueue<task> m_workqueue;//请求队列
    vector<thread> m_threads;//线程池
    
    class Threadworker{
        private:
            int m_id;
            ThreadPool *m_pool;
        public:
            Threadworker(ThreadPool *pool, const int id):m_pool(pool),m_id(id){}
            ~Threadworker(){}
            //线程函数是task func,定义取请求的操作
            void operator() (){
                task func;
                bool dequeued;
                while(m_pool->m_is_running){
                    //为什么要加大括号？
                    {
                        //加锁，等同于lock_guard
                        std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
                        //如果请求为空，等待
                        if(m_pool->m_workqueue.empty()){
                            m_pool->m_conditional_lock.wait(lock);
                        }
                        dequeued = m_pool->m_workqueue.dequeue(func);
                    }
                    if(dequeued){
                        func();
                    }
                    
                }
            }
                        
    };

 public: 
    ThreadPool(int num):m_numThreads(num),m_threads(vector<thread>(num)),m_is_running(false){}

    ~ThreadPool(){}

    //ThreadPool(int num);
    //禁用的初始化
    // ThreadPool(const ThreadPool &) = delete;
    // ThreadPool(ThreadPool &&) = delete;
    //~ThreadPool();

    void start(){
        m_is_running = true;
        for(int i = 0;i < m_numThreads;i++){
            m_threads[i] = thread(Threadworker(this,i));
        }
    }
    void stop(){
        m_is_running = false;
        //唤醒所有等待队列中阻塞的线程,轮询式等待锁
        m_conditional_lock.notify_all();

        for(int i = 0;i < m_threads.size();i++){
            //如果线程i可执行则执行i
            if(m_threads[i].joinable()){
                m_threads[i].join();
            }  
        }
    }

    //input 函数名和参数
    //仅支持_stdcall函数，普通成员函数的调用方式为_thiscal
    // Submit a function to be executed asynchronously by the pool
    template <typename F, typename... Args> 
    auto submit(F &&f, Args &&...args) -> future<decltype(f(args...))>
    {
        if(!m_is_running)
            throw runtime_error("submit on ThreadPool is stopped");
        
        //返回值类型重命名
        using RetType = decltype(f(args...));
        // Create a function with bounded parameters ready to execute
        function<RetType ()> func = bind(forward<F> (f), forward<Args>(args)...);
        // Encapsulate it into a shared ptr in order to be able to copy construct / assign
        auto task_ptr = make_shared<packaged_task<RetType()>> (func);

        // Wrap packaged task into void function,因为队列里的task是void*,因此要封装
        function<void()> wrapper_func = [task_ptr]() {
        (*task_ptr)(); 
        };

        m_workqueue.enqueue(wrapper_func);
        //唤醒一个在等待的线程
        m_conditional_lock.notify_one();

        return task_ptr->get_future();
    }

    bool append( std::function<void(std::shared_ptr<void>)> fun) {
        if (!m_is_running) {
            std::cout << "ThreadPool has shutdown" << std::endl;
            return false;
        }


    //     MutexLockGuard guard(this->mutex_);
    //     if (request_queue.size() > max_queue_size) {
    //         std::cout << max_queue_size;
    //         std::cout << "ThreadPool too many requests" << std::endl;
    //         return false;
    //     }
    //     ThreadTask threadTask;
    //     threadTask.arg = arg;
    //     threadTask.process = fun;

    //     request_queue.push_back(threadTask);
    // //    if (request_queue.size() == 1) {
    // //        condition_.notify();
    // //    }
    //     // 之前是先判断当前队列是否为空，为空才有线程等待在上面，才需要signal
    //     // 而后发现其实直接signal也没事，因为signal信号就算没有等待在信号上的也没事
    //     condition_.notify();
    //     return true;
    }
};


/*
提交成员函数
Functor functor(std::bind(&HTTPData::deal_message, da, info));
Functor functor(std::bind(&Server::make_response,this, info->request));
*/