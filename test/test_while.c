#include <stdio.h>

int a = 1;
int value = 5;

int main() {
	while (value > 0) {
		printf("%d", value);
		value = (value - a);	
	}
	
	printf("%d", value);
}