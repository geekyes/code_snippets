#include <stdio.h>
#include <stdint.h>
// 生成随机数
#include <stdlib.h>
#include <time.h>

#define SIZE 4

enum {UPPER = 1, LOWER, LEFT, RIGHT};

void Print(int const *pBuf);
void Calc(int *pBuf, const int direction);
void Move(int *pBuf, int step);
int NeedMove(int *pBuf, int step);
void Add(int *pBuf, int step);
void Merge(int *pBuf, int step);

int main(int argc, char **argv)
{
    char direction = 0;
    int buf[SIZE * SIZE] = {0};

    while ((direction = getchar()) != 'q') { // 输入q退出
        switch (direction) {
            case 'w':
                Calc(buf, UPPER);
                Print(buf);
                break;
            case 's':
                Calc(buf, LOWER);
                Print(buf);
                break;
            case 'a':
                Calc(buf, LEFT);
                Print(buf);
                break;
            case 'd':
                Calc(buf, RIGHT);
                Print(buf);
                break;
           default: break;
        }
    }

    return 0;
}


void Print(int const *pBuf)
{
    for (int i = 0; i < SIZE * SIZE; i++) {
        printf("| %4d ", pBuf[i]);
        if (0 == (i + 1) % SIZE) {
            putchar('|');
            putchar('\n');
            puts("-----------------------------");
        }
    }
}

 
void Calc(int *pBuf, const int direction)
{
    static uint8_t tmp = 0;  // 让srand()执行一次
    uint8_t flag = 0; // 有空位的标志
    // 生成一个随机的位置
    if (!tmp) {
        tmp = 1;
        srand((unsigned)time(NULL));
    }
    for (int i = 0; i < SIZE * SIZE; i++) { // 找空位
        if (0 == pBuf[i]) {
            flag = 1;
        }
    }
    while (flag) { //生成随机位置
        int number = rand() % (SIZE * SIZE);
        if (0 == pBuf[number] || 2 == pBuf[number]) {
            pBuf[number] += 2;
            break;
        }
    }

    // 根据w s a d (上，下，左，右)来计算
    switch (direction) {
        case UPPER:
            for (int i = 0; i < SIZE; i++) { // SIZE * SIZE 的左上角
                Merge(pBuf, 4);
            }
            break;
        case LOWER:
            for (int i = 12; i < SIZE * SIZE; i++) { // SIZE * SIZE 的左下角
                Merge(pBuf, -4);
            }
            break;
        case LEFT:
            for (int i = 0, j = 0; j < SIZE; i += 4, j++) { // SIZE * SIZE 的左上角
                Merge(pBuf, 1);
            }
            break;
        case RIGHT:
            for (int i = 3, j = 0; j < SIZE; i += 4, j++) { // size * size 的右上角
                Merge(pBuf, -1);
            }
            break;
        default: break;
    }
}

void Move(int *pBuf, int step)
{
    for (int i = 0, j = 0, k = 0; k < SIZE; i += step, k++) { // 移除不等于零元素之间的零
        if (0 != *(pBuf + i)) {
            if (i != j) {
                *(pBuf + j) = *(pBuf + i);
                *(pBuf + i) = 0;
            }
            j += step;
        }
    }
}

void Add(int *pBuf, int step)
{
    for (int i = 0, j = 0; i < SIZE; i++, j += step) { // 前后两个相等，就相加
        if (NeedMove(pBuf, step)) {
            Move(pBuf, step);
            i = j = 0;
        }

        if (*(pBuf + j) == *(pBuf + j + step)) {
            *(pBuf + j) += *(pBuf + j + step);
            *(pBuf + j + step) = 0;
        } 
    }
}

void Merge(int *pBuf, int step)
{
    Add(pBuf, step);
}

int NeedMove(int *pBuf, int step)
{
    int retVal = 0;
    for (int i = 0, j = 0; i < SIZE; i++, j += step) {
        if (0 != *(pBuf + j)) {
            retVal++;
        }
    }

    // 以下情况不用移动
    if ((1 == retVal && *(pBuf)) ||\
        (2 == retVal && 0 == *(pBuf + (SIZE - 1) * step) && 0 == *(pBuf + (SIZE - 2) * step)) ||\
        (3 == retVal && 0 == *(pBuf + (SIZE - 1) * step)) ||\
        4 == retVal) {
        retVal = 0;
    }

    return retVal;
}

