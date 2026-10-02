#include "WorkerPool.h"
#include <iostream>

// Constructor: Reserves memory for the worker threads vector based on the requested size
WorkerPool::WorkerPool(size_t threads) {
    workers.reserve(threads);
}

// Destructor: Ensures all threads are safely stopped and joined before destruction
WorkerPool::~WorkerPool() {
    stop();
}

// Starts the worker pool by spawning background threads
void WorkerPool::start() {
    shouldStop = false;
    
    // Spawns 2 threads
    for (size_t i = 0; i < 2; ++i) {
        workers.emplace_back(&WorkerPool::workerThread, this);
    }
}

// Enqueues a new submission ID into the job queue in a thread-safe manner
void WorkerPool::enqueueJob(int submissionId) {
    {
        // Lock the queue mutex to safely push the new job
        std::unique_lock<std::mutex> lock(queueMutex);
        jobQueue.push(submissionId);
    }
    // Wake up one waiting worker thread to process the new job
    cv.notify_one(); 
}

// The main execution loop run by each worker thread
void WorkerPool::workerThread() {
    while (true) {
        int currentSubmissionId = -1;
        {
            // Acquire lock to safely access the shared job queue and stop flag
            std::unique_lock<std::mutex> lock(queueMutex);
            
            // Wait until the pool is instructed to stop OR there is a job in the queue
            cv.wait(lock, [this] { return shouldStop || !jobQueue.empty(); });

            // Exit condition: if stop was requested and no jobs remain, terminate thread
            if (shouldStop && jobQueue.empty()) return;

            // Fetch the next job from the front of the queue
            currentSubmissionId = jobQueue.front();
            jobQueue.pop();
        }

        // Process the retrieved job outside the critical section
        if (currentSubmissionId != -1) {
            std::cout << "[Worker " << std::this_thread::get_id() 
                      << "] Running Submission: " << currentSubmissionId << std::endl;
            
            // Next: Connect to ProcessManager & ResourceManager
        }
    }
}

// Signals all worker threads to stop, wakes them up, and joins them
void WorkerPool::stop() {
    {
        // Set the stop flag under lock protection
        std::unique_lock<std::mutex> lock(queueMutex);
        shouldStop = true;
    }
    
    // Wake up all sleeping worker threads so they can exit their wait loops
    cv.notify_all(); 
    
    // Join all worker threads to ensure they finish execution cleanly
    for (std::thread &worker : workers) {
        if (worker.joinable()) worker.join();
    }
    
    // Clear the thread vector
    workers.clear();
}