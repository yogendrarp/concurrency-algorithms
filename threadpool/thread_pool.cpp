#include <atomic>
#include <vector>
#include <iostream>
#include "threadsafe_queue.h"
#include "joiner_threads.h"
#include <chrono>

class process_pool
{
    std::atomic_bool done;
    threadsafe_queue<std::function<void()> > work_queue;
    std::vector<std::thread> threads;
    joiner_threads joiner_threads;

    void worker_thread()
    {

        while (!done)
        {

            std::function<void()> task;
            if (work_queue.try_pop(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

public:
    process_pool() : done(false), joiner_threads(threads)
    {
        int const thread_count = std::thread::hardware_concurrency();
        try
        {
            for (int i = 0; i < thread_count; i++)
            {
                threads.push_back(std::thread(&process_pool::worker_thread, this));
            }
        }
        catch (...)
        {
            done = true;
            throw;
        }
    }

    int size()
    {
        return work_queue.size();
    }

    ~process_pool()
    {
        done = true;
    }

    template <typename Function_type>
    void submit(Function_type f)
    {
        work_queue.push(std::function<void()>(f));
        std::cout << work_queue.size() << std::endl;
    }
};

void runner()
{
    process_pool pool;
    std::cout << "Testing Thread Pool" << std::endl;

    for (int i = 0; i < 100; i++)
    {
        pool.submit([=]
                    {
                        printf("%d \n", std::this_thread::get_id());
                        for (int i = 0; i < 10000; i++)
                        {
                            for (int j = 0; j < 65531; j++)
                            {
                            }
                        }
                    });
    }

    while (pool.size() != 0)
    {
        std::cout << pool.size()
                  << std::endl;
    }
    std::cout << "Exit \n"
              << std::endl;
}