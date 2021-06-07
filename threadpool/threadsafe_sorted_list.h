#pragma once

#include <mutex>
#include <list>
#include <memory>
#include <condition_variable>
#include <thread>

template <typename T>
class threadsafe_sorted_list
{
private:
    std::mutex m;
    std::condition_variable cv;
    std::list<T> list;

public:
    threadsafe_sorted_list(){};

    void push_back(T value)
    {
        std::lock_guard<std::mutex> lg(m);
        list.push_back(value);
        list.sort();
        cv.notify_one();
    }

    std::shared_ptr<T> pop_front()
    {
        std::lock_guard<std::mutex> lg(m);
        if (list.empty())
        {
            return std::shared_ptr<T>();
        }
        else
        {
            std::shared_ptr<T> ref = std::make_shared<T>(list.front());
            list.pop_front();
            return ref;
        }
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lg(m);
        return list.empty();
    }

    std::shared_ptr<T> wait_pop_front()
    {
        std::lock_guard<std::mutex> lg(m);
        cv.wait(lg, [this]
                { return !list.empty(); });
        std::shared_ptr<T> ref = std::make_shared<T>(list.front());
        list.pop_front();
        return ref;
    }

    size_t size()
    {
        std::lock_guard<std::mutex> lg(m);
        return list.size();
    }

    bool try_pop_front(T &value)
    {
        std::lock_guard<std::mutex> lg(m);
        if (list.empty())
            return false;
        value = std::move(*(std::make_shared<T>(list.front())));
        list.pop_front();
        return true;
    }
};