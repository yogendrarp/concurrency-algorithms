#include <vector>
#include <thread>

class joiner_threads
{
    std::vector<std::thread> &threads;

public:
    explicit joiner_threads(std::vector<std::thread>& _threads) : threads(_threads) {}

    ~joiner_threads()
    {
        for (long i = 0; i < threads.size(); i++)
        {
            if (threads[i].joinable())
            {
                threads[i].join();
            }
        }
    }
};