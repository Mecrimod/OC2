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


void schedule() {
    while (tasks) {
        struct node *current = tasks;
        while (current) {
            Task *task = current->task;
            int slice = QUANTUM;

            // Если время выполнения меньше кванта, используем оставшееся время
            if (task->burst <= QUANTUM) {
                slice = task->burst;
            }

            // Выполнение задачи
            run(task, slice);

            // Обновление времени выполнения задачи
            task->burst -= slice;

            // Если задача еще не завершена, возвращаем ее в конец списка
            if (task->burst > 0) {
                current = current->next;
            } else {  // Если задача завершена, удаляем ее из списка
                struct node *temp = current;
                current = current->next;
                delete(&tasks, temp->task);
            }
        }
    }
}