#ifndef ZERO_THREADPOOL_H
#define ZERO_THREADPOOL_H

#include <future>
#include <functional>
#include <iostream>
#include <queue>
#include <mutex>
#include <memory>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
using namespace std;


void getNow(timeval *tv);
int64_t getNowMs();

#define TNOW      getNow()
#define TNOWMS    getNowMs()


class ZERO_ThreadPool
{
    protected:
        struct TaskFunc
        {
            TaskFunc(uint64_t expireTime) : _expireTime(expireTime)
            { }

            std::function<void()>   _func;
            int64_t                _expireTime = 0;
        };
        typedef shared_ptr<TaskFunc> TaskFuncPtr;
    public:
        ZERO_ThreadPool();

        virtual ~ZERO_ThreadPool();

        bool init(size_t num);

        size_t getThreadNum()
        {
            std::unique_lock<std::mutex> lock(mutex_);

            return threads_.size();
        }

        size_t getJobNum()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            return tasks_.size();
        }

        void stop();

        bool start(); // 创建线程

        template <class F, class... Args>
        auto exec(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
        {
            return exec(0,f,args...);
        }

        template <class F, class... Args>
        auto exec(int64_t timeoutMs, F&& f, Args&&... args) -> std::future<decltype(f(args...))>
        {
            int64_t expireTime =  (timeoutMs == 0 ? 0 : TNOWMS + timeoutMs);  
            using RetType = decltype(f(args...));  
            auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

            TaskFuncPtr fPtr = std::make_shared<TaskFunc>(expireTime);  
            fPtr->_func = [task]() 
            { 
                (*task)();
            };

            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.push(fPtr);              
            condition_.notify_one();      

            return task->get_future();
        }

        bool waitForAllDone(int millsecond = -1);

    protected:
        bool get(TaskFuncPtr&task);

        bool isTerminate() { return bTerminate_; }

        void run();

    protected:

        queue<TaskFuncPtr> tasks_;

        std::vector<std::thread*> threads_; 

        std::mutex                mutex_;

        std::condition_variable   condition_;

        size_t                    threadNum_;

        bool                      bTerminate_;

        std::atomic<int>          atomic_{ 0 };
};

#endif 




