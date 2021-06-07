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

std::mutex m;

void run(int display_value, int burst_time)
{
    std::cout << " Process id : " << std::this_thread::get_id() << " dispays value of : " << display_value << " with a burst time of : " << burst_time << std::endl;
    std::lock_guard<std::mutex> lg(m);
    std::this_thread::sleep_for(std::chrono::milliseconds(burst_time));
}

class process_pool
{
    std::atomic_bool done;
    threadsafe_sorted_list<std::function<void()> > work_queue;
    std::vector<std::thread> threads;
    joiner_threads joiner_threads;

    void worker_thread()
    {

        while (!done)
        {

            std::function<void()> task;
            if (work_queue.try_pop_front(task))
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
        work_queue.push_back(std::function<void()>(f));
        std::cout << work_queue.size() << std::endl;
    }
};

int main()
{
    threadsafe_sorted_list<int> list;
    list.push_back(10);
    list.push_back(101);
    list.push_back(9);
    list.push_back(8);
    list.push_back(24);

    std::cout << *(list.pop_front().get()) << std::endl;
    std::cout << *(list.pop_front().get()) << std::endl;
    std::cout << *(list.pop_front().get()) << std::endl;
    std::cout << *(list.pop_front().get()) << std::endl;
}