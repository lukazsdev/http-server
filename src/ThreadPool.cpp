#include "ThreadPool.h"

ThreadPool::ThreadPool(const size_t num_threads) : stop(false) {
    for (size_t i = 0; i < num_threads; i++) {
        workers.emplace_back([this]() {
           while (true) {
               std::function<void()> task;
               {
                   std::unique_lock<std::mutex> lock(queue_mutex);

                   // Wait until tasks are available or stop flag is set
                   cv.wait(lock, [this]() {return stop || !tasks.empty();});
                   if (stop && tasks.empty()) return;

                   task = std::move(tasks.front());
                   tasks.pop();
               }
               task();
           }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop = true;
    }

    cv.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

void ThreadPool::EnqueueTask(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        tasks.push(std::move(task));
    }
    cv.notify_one();
}


