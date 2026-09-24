#ifndef WORKER_POOL_H
#define WORKER_POOL_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class WorkerPool {
public:
    WorkerPool(size_t threads = 2);
    ~WorkerPool();

    void enqueueJob(int submissionId);
    void start();
    void stop();

private:
    void workerThread();

    std::vector<std::thread> workers;
    std::queue<int> jobQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> shouldStop{false};
};

#endif
