#ifndef WORKER_POOL_H
#define WORKER_POOL_H

#include <vector>
#include <thread>
#include <atomic>

#include "../scheduler/Scheduler.h"
#include "../executor/ExecutionWorker.h"

// WorkerPool manages multiple worker threads.
//
// Its main responsibility is to create and manage a group of
// worker threads. Each worker gets a job from the Scheduler
// and sends that job to the ExecutionWorker for execution.
class WorkerPool {
private:

    // Reference to the Scheduler.
    //
    // The scheduler maintains the job queue. Workers will
    // request the next available job from this scheduler.
    Scheduler& scheduler;

    // Reference to the ExecutionWorker.
    //
    // ExecutionWorker is responsible for actually processing
    // a submission: compiling the code, running it, checking
    // the output, and updating the database.
    ExecutionWorker& executionWorker;

    // Stores all worker threads created by this pool.
    //
    // Each element represents one independent thread that
    // continuously takes jobs from the scheduler.
    std::vector<std::thread> workers;

    // Number of worker threads that should be created.
    //
    // For example, if workerCount = 4, the pool will create
    // four worker threads.
    int workerCount;

    // Indicates whether the worker pool is currently running.
    //
    // atomic is used because this variable can potentially
    // be accessed by multiple threads safely.
    std::atomic<bool> running;

    // Function executed by every worker thread.
    //
    // Each worker repeatedly:
    // 1. Gets a job from the Scheduler.
    // 2. Checks whether the scheduler has stopped.
    // 3. Sends the job to ExecutionWorker.
    void workerLoop();

public:

    // Constructor.
    //
    // scheduler:
    //     Scheduler from which workers receive jobs.
    //
    // executionWorker:
    //     Component responsible for executing submissions.
    //
    // workerCount:
    //     Number of worker threads to create.
    WorkerPool(
        Scheduler& scheduler,
        ExecutionWorker& executionWorker,
        int workerCount
    );

    // Starts the worker pool.
    //
    // This creates workerCount threads.
    // Each thread starts executing workerLoop().
    void start();

    // Stops the worker pool.
    //
    // This shuts down the scheduler and waits for all
    // worker threads to finish before returning.
    void stop();
};

#endif
