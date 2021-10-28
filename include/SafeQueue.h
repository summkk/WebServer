//一个安全的队列，避免多线程同时读写队列
//Question:怎么将模板类的声明跟实现分离？
#include <queue>
#include <mutex>
#include <iostream>

using namespace std;

template <typename T>

class SafeQueue{
private:
    queue<T> m_queue;    
    mutex m_mutex;

public:
    SafeQueue(){}
    ~SafeQueue(){}

    bool empty(){
        unique_lock<mutex> lock(m_mutex);//互斥信号量加锁
        return m_queue.empty();
    }

    int size(){
        unique_lock<mutex> lock(m_mutex);//互斥信号量加锁
        return m_queue.size();
    }

    void enqueue(T &t){
        unique_lock<mutex> lock(m_mutex);//互斥信号量加锁
        m_queue.push(t);
    }

    //STL中如何进行空队列的异常处理？
    T get_top(){
        unique_lock<mutex> lock(m_mutex);//互斥信号量加锁
        if(m_queue.empty()){
            cout << "queue is empty, can't get top" << endl;
        }
        return m_queue.front();
    }

    bool dequeue(T &t){
        unique_lock<mutex> lock(m_mutex);//互斥信号量加锁
        if(m_queue.empty()){
            return false;
        }
        //这个地方为什么要用move？
        //
        t = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }
};