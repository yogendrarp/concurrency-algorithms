#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../threadpool/threadsafe_sorted_list.h"
#include "../threadpool/joiner_threads.h"
#include "../threadpool/process_table.h"

std::mutex m;

class process_pool
{
    std::atomic_bool done;
    threadsafe_sorted_list<process_table> work_queue;
    std::vector<std::thread> threads;
    joiner_threads joiner_threads;

    void worker_thread()
    {

        /**
         * update code to
         * while (work_queue.size() != 100) to submit all process
         * then start sjf to see burst time in order
         */
        while (!done)
        {

            process_table _process_table;
            if (work_queue.try_pop_front(_process_table))
            {
                _process_table.run();
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

    void submit(process_table _process_table)
    {
        work_queue.push_back(_process_table);
        //std::cout << work_queue.size() << std::endl;
    }
};

int main()
{
    process_pool pool;
    std::atomic<int> counter = ATOMIC_VAR_INIT(0);

    std::cout << "Testing Process Pool" << std::endl;
    std::srand(std::time(0));

    for (int i = 0; i < 100; i++)
    {
        process_table p1(std::atomic_fetch_add(&counter, 1), (std::rand() % 1000 + 1));
        pool.submit(p1);
    }

    while (pool.size() != 0)
    {
        //std::cout << (list.pop_front().get())->burst_time << std::endl;
        ;
    }
}