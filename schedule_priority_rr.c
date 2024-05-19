//
// Created by mecrimod on 23.04.24.
//

#include "stdio.h"
#include "stdlib.h"//подключаем файлы и библиотеки
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

#define QUANTUM 10
// Глобальный указатель на список задач
struct node *tasks = NULL;
// Функция добавления задачи в список
void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = name;
    task->priority = priority;
    task->burst = burst;


    insert(&tasks, task);
}

void run_rr(struct node **taskList) {
    struct node *current = *taskList;
    while (*taskList != NULL) {
        current = (current == NULL ? *taskList : current);//Если current равен NULL (это может произойти, если мы достигли конца списка), то возвращаемся к началу списка.
        Task *task = current->task;
        int slice = task->burst > QUANTUM ? QUANTUM : task->burst;
        run(task, slice);
        task->burst -= slice;

        if (task->burst == 0) {
            struct node *nextTask = current->next;
            delete(taskList, task);
            current = nextTask;
        } else {
            current = current->next;
        }
    }
}

void run_single_task(Task *task) {
    run(task, task->burst);
}

int count_tasks(struct node *list) {
    int count = 0;
    struct node *current = list;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

int get_max_priority(struct node *list) {
    int max_priority = -1;
    struct node *current = list;
    while (current != NULL) {
        if (current->task->priority > max_priority) {
            max_priority = current->task->priority;
        }
        current = current->next;
    }
    return max_priority;
}

void schedule() {
    int max_priority;
    while (tasks != NULL) {
        max_priority = get_max_priority(tasks);
        struct node *same_priority_tasks = NULL;

        // вытаскиваекм задачи с макс приоритетом
        struct node *temp = tasks;
        while (temp != NULL) {
            struct node *next = temp->next;
            if (temp->task->priority == max_priority) {
                insert(&same_priority_tasks, temp->task);
                delete(&tasks, temp->task);
            }
            temp = next;
        }

        // Если в этом приоритете одна задача - просто выполняем. Иначе используем rr
        if (count_tasks(same_priority_tasks) > 1) {
            run_rr(&same_priority_tasks);
        } else if (same_priority_tasks != NULL) {
            run_single_task(same_priority_tasks->task);
            free(same_priority_tasks);
        }
    }
}