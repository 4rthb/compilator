#include <stdio.h>

int a = 1;
int b = 2;
int c = 3;
int d = 4;
int e = 5;
int f = 6;

int main() {
	if (a < 2) {
		printf("%d", a);
	}

	if (b <= 2) {
		printf("%d", b);
	}

	if (c == 3) {
		printf("%d", c);
	}

	if (d != 5) {
		printf("%d", d);
	}

	if (e >= 5) {
		printf("%d", e);
	}

	if (f > 5) {
		printf("%d", f);
	}
}