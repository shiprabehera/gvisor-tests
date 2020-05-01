#include <stdio.h>
#include <unistd.h>

int main() {
	FILE* file = fopen("/dev/null", "w");
	int data = 0;
	for(ssize_t i = 1000000; i > 0; i--) {
		fwrite(&data, sizeof(data), 1, file);
	}
	fclose(file);
	return 0;
}