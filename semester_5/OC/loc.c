#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4096
#define PAGE_TABLE_SIZE 1024
#define OFFSET_MASK 0xFFF

// Функция для вывода таблицы отображения виртуальной памяти на экран
void print_page_table(unsigned int* page_table) {
    printf("Virtual Page NumbertPhysical Page Numbern\n");
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        printf("%d\t\t\t%d\n", i, page_table[i]);
    }
}

int main() {
    // Заданный размер бинарного файла
    long file_size = 100000;

    // Вычисляем количество страниц виртуальной памяти
    int virtual_page_count = (file_size + PAGE_SIZE - 1) / PAGE_SIZE;

    // Выделяем память для таблицы отображения виртуальной памяти
    unsigned int* page_table = (unsigned int*)malloc(sizeof(unsigned int) * PAGE_TABLE_SIZE);

    // Заполняем таблицу случайными значениями
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        page_table[i] = rand() % (virtual_page_count + 1);
    }

    // Выводим таблицу на экран
    print_page_table(page_table);

    // Освобождаем выделенную память
    free(page_table);

    return 0;
}
