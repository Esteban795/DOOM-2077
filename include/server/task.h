#ifndef _LIB_DOOM_SERVER_TASK_H
#define _LIB_DOOM_SERVER_TASK_H

#include <time.h>
#include "../../include/collection/vec.h"

/// A task to run at a certain time
///
/// @see task_executor_t for the task scheduler
typedef struct {
    void (*run)(void*); /// Function to run
    void* data; /// Data to pass to the function
    struct timespec scheduled; /// When to run the task
} task_t;

/// task_executor_t, the task scheduler
typedef struct {
    vec_t tasks; /// Vector of tasks
} task_executor_t;

/// Initilize a new task_executor_t
void task_executor_init(task_executor_t* executor);

/// Destroy a task_executor_t
void task_executor_destroy(task_executor_t* executor);

/// Add a task to the task_executor_t
void task_executor_add(task_executor_t* executor, task_t task);

/// Run all tasks that are scheduled to run
void task_executor_run(task_executor_t* executor, struct timespec* now);

/// Create a new Task
///
/// @param run: Function to run
/// @param data: Data to pass to the function, if data is not NULL, it must be freeable by the executor.
/// @param scheduled: When to run the task
/// @return task_t: The new task
task_t task_new(void (*run)(void*), void* data, struct timespec scheduled);
#endif