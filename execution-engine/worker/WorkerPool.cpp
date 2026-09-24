#include "WorkerPool.h"
#include <iostream>

WorkerPool::WorkerPool(size_t threads) {
    workers.reserve(threads);
}

WorkerPool::~WorkerPool() {
    stop();
}

void WorkerPool::start() {
    shouldStop = false;
    for (size_t i = 0; i < 2; ++i) {
        workers.emplace_back(&WorkerPool::workerThread, this);
    }
}

void WorkerPool::enqueueJob(int submissionId) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        jobQueue.push(submissionId);
    }
    cv.notify_one(); 
}

void WorkerPool::workerThread() {
    while (true) {
        int currentSubmissionId = -1;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this] { return shouldStop || !jobQueue.empty(); });

            if (shouldStop && jobQueue.empty()) return;

            currentSubmissionId = jobQueue.front();
            jobQueue.pop();
        }

        if (currentSubmissionId != -1) {
            std::cout << "[Worker " << std::this_thread::get_id()<< "] Running Submission: " << currentSubmissionId << std::endl;
        }
    }
}
void WorkerPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        shouldStop = true;
    }
    cv.notify_all(); 
    for (std::thread &worker : workers) {
        if (worker.joinable()) worker.join();
    }
    workers.clear();
}
