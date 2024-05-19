//
// Created by mecrimod on 23.04.24.
//

#include "stdio.h"
#include "stdlib.h"//подключаем файлы и библиотеки
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

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


Task *pick_NextTask() {
    if (!tasks) {
        return NULL;
    }

    struct node *temp = tasks;
    struct node *highest_priority_node = temp;
    int max_priority = temp->task->priority;

    // Поиск задачи с наивысшим приоритетом
    while (temp != NULL) {
        if (temp->task->priority > max_priority) {
            max_priority = temp->task->priority;
            highest_priority_node = temp;
        }
        temp = temp->next;
    }

    Task *task = highest_priority_node->task;

    // Удаляем найденную задачу из списка
    delete(&tasks, task);

    return task;
}




void schedule() {
    Task *task = pick_NextTask();
    while (task != NULL) {
        run(task, task->burst);
        task = pick_NextTask();
    }
}
