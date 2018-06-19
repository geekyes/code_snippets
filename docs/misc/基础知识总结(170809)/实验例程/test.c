/*****************************************************
 *
 * test.c
 *
 * Description : Hello World in C, ANSI-style
 *
 */

#include <stdio.h>

unsigned int test_var = 100;

unsigned char aa[100] = {1};

typedef struct {
	unsigned char a;
	unsigned int b;
	unsigned char a_s[10];
	unsigned int b_bf_h: 8;
	unsigned int b_bf_l: 6;
} test_s;

test_s g_test_s = {0};


void test(void)
{
	unsigned char str[100] = {0};
	unsigned char z = 'a';
	test_s l_test_s = {1};

	for (int i = 0; i < sizeof(str); i++) {
		if (i > (int)z) {
			str[i] = z;
			z++;
		} else {
			str[i] = (unsigned char)i;
		}
	}

	for (int i = 0; i < sizeof(aa); i++) {
		aa[i] = str[i];
	}

	l_test_s.a = 10;
	l_test_s.b_bf_h = 0x80;
	l_test_s.b_bf_l = (1U << 6U);

	for (int i = 0; i < sizeof(l_test_s.a_s); i++) {
		l_test_s.a_s[i] = (unsigned char)i;
	}

	g_test_s = l_test_s;

}

void cpnnw(unsigned int *, unsigned int *, int);

int main(void)
{
	unsigned char str[20] = {0};

	unsigned char ss = 'a';

	unsigned int *p = &test_var;

	unsigned long t = (unsigned long)p;

	test_var += 200;

	while (1) {
		test();
		{
			unsigned int r2[10] = {0};
			unsigned int r3[10] = {1, 1, 1};
			cpnnw(r2, r3, sizeof(r3));
		}
		test_var += 10;
		if (test_var >= 1000) {
			test_var = 100 * sizeof(int);
			ss++;
			printf("t = %ld,\t ss = %c \n", t, ss);
			for (int i = 0; i < sizeof(str); i++) {
				str[i] = (unsigned char)i;
				if (i > 7) {
					str[i] = ss + i;
				}
			}
		}
		//code
	}
}










void cpnnw(unsigned int *to, unsigned int *from, int size)
{
	if (0 == size) return;

	do {
		*to++ = *from++;
	} while (--size != 0);
}















