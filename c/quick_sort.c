#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100

typedef int (*COMPARE)(const void *e1, const void *e2);

int compare_float(const void *e1, const void *e2)
{
    const float dif = 0.0f;
    float result = *((float*)e1) - *((float *)e2);
    if (result > dif) { // e1 > e2 1代表正数
        return 1;
    } else if (result < dif) { // e1 < e2, -1代表负数
        return -1;
    } else { // e1 = e2
        return 0;
    }
}

void swap(void *obj1, void *obj2, size_t size)
{
    char *p1 = (char *)obj1;
    char *p2 = (char *)obj2;
    for (size_t i = 0; i < size; i++, p1++, p2++) {
        char temp = *p1;
        *p1 = *p2;
        *p2 = temp;
    }
}

// 递归调用自己实现切换分区
void quick_sort(void *base, size_t number, size_t size, COMPARE compare)
{
    void *pivot = base;
    size_t left = 0;
    size_t right = number - 1;

    if (number < 2) return;

    while (left != right) {
        // 必须让right先去找比pivot小的，
        // 不然在交换pivot是就把一个大于pivot的数交换到了小于的一边
        if (compare(pivot, base + size * right) <= 0) { // 向前没有小于*pivot
            right--;
        } else if (compare(pivot, base + size * left) >= 0) { // 向后没有大于*pivot
            left++;
        } else {
            swap(base + size * left, base + size * right, size);
        }
    }
    swap(pivot, base + size * left, size); // 让*pivot归位

    quick_sort(base, left, size, compare); // 处理pivot的前半分区
    quick_sort(base + (left + 1) * size,\
            number - (left + 1), size, compare); // 处理pivot的后半分区
}

void show_arr(const float *p, size_t number)
{
    for (size_t i = 0; i < number; i++) {
        printf("%8.2f ", *(p + i));
        if (!((i + 1) % 10))
            putchar('\n');
    }
    putchar('\n');
}

int main(int argc, char *argv[])
{
    float f_arr[SIZE] = {0.0f};
    srand((unsigned)time(NULL));
    for (size_t i = 0; i < SIZE; i++) {
        f_arr[i] = rand() % (SIZE * 1000) / 100.0f;
    }
    puts("Before sorting: ");
    show_arr(f_arr, SIZE);
    quick_sort(f_arr, SIZE, sizeof(f_arr[0]), compare_float);
    puts("After sorting: ");
    show_arr(f_arr, SIZE);
    
    return 0;
}
