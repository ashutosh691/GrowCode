#include <iostream>
#include "execution-engine/scheduler/Scheduler.h"

int main()
{
    // create a scheduler object
    Scheduler scheduler;

    // add jobs to the scheduler queue
    scheduler.addJob(10);
    scheduler.addJob(20);
    scheduler.addJob(30);

    // Retrieve the jobs in order (First-In, First-Out)
    std::cout << "Queue empty: "<< (scheduler.empty() ? "Yes" : "No") << std::endl;

    // get the first job from the queue
    int job1 = scheduler.getNextJob();
    int job2 = scheduler.getNextJob();
    int job3 = scheduler.getNextJob();

    // display the jobs in the order they were retrieved
    std::cout << "Job 1: " << job1 << std::endl;
    std::cout << "Job 2: " << job2 << std::endl;
    std::cout << "Job 3: " << job3 << std::endl;

    // check whether the queue is empty after
    // removing all three jobs
    std::cout << "Queue empty after processing: " << (scheduler.empty() ? "Yes" : "No") << std::endl;

    // shut down the scheduler
    scheduler.shutdown();

    // after shutdown, getNextJob() should return -1
    int result = scheduler.getNextJob();
    std::cout << "Job after shutdown: " << result << std::endl;

    return 0;
}
