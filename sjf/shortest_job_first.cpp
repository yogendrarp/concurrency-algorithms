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

int main()
{
    threadsafe_sorted_list<int> list;
    list.push_back(10);
    list.push_back(101);
    list.push_back(9);
    list.push_back(8);
    list.push_back(24);

    
    

}