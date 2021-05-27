#include <iostream>
#include <thread>
#include <assert.h>
#include <atomic>
#include <algorithm>
#include <vector>

#define NUMBER_OF_THREADS 10

struct BakeryAlgorithm
{
    void entry(int thread_id)
    {
        choosing[thread_id] = true;
        number[thread_id] = *std::max_element(number, number + NUMBER_OF_THREADS);
        choosing[thread_id] = false;

        for (int j = 0; j < NUMBER_OF_THREADS - 1; j++)
        {
            while (choosing[j])
                ;
            while (number[j] != 0 && ((number[j] <= number[thread_id]) && (j < thread_id)))
                ;
        }
    }

    void exit(int thread_id)
    {
        number[thread_id] = 0;
    }

private:
    int number[NUMBER_OF_THREADS];
    bool choosing[NUMBER_OF_THREADS];
};

void total_sum(BakeryAlgorithm &bakery_algorithm, int &counter, int thread_id)
{
    bakery_algorithm.entry(thread_id);
    counter++;
    bakery_algorithm.exit(thread_id);
}

int main()
{
    BakeryAlgorithm bakery_algorithm;
    int counter = 0;
    std::atomic<int> thread_id(0);

    std::vector<std::thread> thread_collection;

        for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        thread_collection.__emplace_back([&]() { total_sum(bakery_algorithm, counter, thread_id.fetch_add(1)); });
    }

    for (auto &t : thread_collection)
    {
        t.join();
    }

    std::cout << "Counter value is : " << counter << std::endl;
}