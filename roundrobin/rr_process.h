#pragma once

#include <mutex>
#include <iostream>
#include <thread>

std::mutex processor;
const int time_quantum = 20;

struct process_table
{
    int display_value;
    int burst_time;

    int run()
    {
        std::cout << " Process id : " << std::this_thread::get_id() << " dispays value of : " << display_value << " with a burst time of : " << burst_time << std::endl;
        std::lock_guard<std::mutex> lg(processor);
        std::this_thread::sleep_for(std::chrono::milliseconds(time_quantum));
        return burst_time - time_quantum;
    }

    bool operator<(const process_table &other_process) const
    {
        return burst_time < other_process.burst_time;
    }

    process_table(int _display_value, int _burst_time) : display_value(_display_value), burst_time(_burst_time) {}

    process_table() {}
};
