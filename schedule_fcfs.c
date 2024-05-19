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

    // Вставляем в список для реализации FIFO
    insert(&tasks, task);
}

// Функция выбора задачи в соответствии с FIFO
Task *pick_NextTask() {
    // Если список пуст, ничего не возвращаем
    if (!tasks) {
        return NULL;
    }

    struct node *temp = tasks;


    // Находим последний узел списка задач
    while (temp->next != NULL) {
        temp = temp->next;
    }

    // Последний узел в списке
    Task *task = temp->task;

    // Удаляем найденный узел из списка
    delete(&tasks, task);//в итоге проходим, возращаем последний элемент , удаляем, проходим опять

    // Возвращаем последний узел 
    return task;
}


void schedule() {
    Task *task = pick_NextTask();
    while (task != NULL) {
        run(task, task->burst);
        task = pick_NextTask();
    }
}
