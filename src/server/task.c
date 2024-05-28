#include "../../include/server/task.h"
#include "../../include/server/state.h"

int compare_task(const void* a, const void* b) {
    task_t* task_a = *(task_t**) a;
    task_t* task_b = *(task_t**) b;
    return compare_instant(&task_a->scheduled, &task_b->scheduled);
}

void task_executor_init(task_executor_t* executor) {
    vec_init(&executor->tasks);
}

// Destroy a task_executor_t
void task_executor_destroy(task_executor_t* executor) {
    for (size_t i = 0; i < (int) vec_length(&executor->tasks); i++) {
        task_t* task = vec_get(&executor->tasks, i);
        if (task->data != NULL) {
            free(task->data);
        }
    }
    vec_destroy(&executor->tasks, true);
}

// Add a task to the task_executor_t
void task_executor_add(task_executor_t* executor, task_t task) {
    task_t* task_ = malloc(sizeof(task_t));
    *task_ = task;
    int ind = vec_binary_search(&executor->tasks, task_, compare_task);
    if (ind < 0) {
        ind = ~ind;
    }
    vec_insert(&executor->tasks, ind, task_);
}

// Run all tasks that are scheduled to run
void task_executor_run(task_executor_t* executor, struct timespec* now) {
    int i = 0;
    // Run all tasks that are scheduled to run
    while (i < (int) vec_length(&executor->tasks)) {
        task_t* task = vec_get(&executor->tasks, i);
        if (compare_instant(&task->scheduled, now) <= 0) {
            task->run(task->data);
            if (task->data != NULL) {
                free(task->data);
            }
            free(task);
            i++;
        } else {
            break;
        }
    }

    // Remove all tasks that have been run
    for (int j = 0; j + i < (int) vec_length(&executor->tasks); j++) {
        vec_set(&executor->tasks, j, vec_get(&executor->tasks, j + i));
    }
    executor->tasks.length -= i;
}

// Create a new Task
//
// run: Function to run
// data: Data to pass to the function, if data is not NULL, it must be freeable by the executor.
// scheduled: When to run the task
task_t task_new(void (*run)(void*), void* data, struct timespec scheduled) {
    task_t task;
    task.run = run;
    task.data = data;
    task.scheduled = scheduled;
    return task;
}