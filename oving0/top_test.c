#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("Hello world\n");
	
	for number=1 to 1052672
	{
	allocate(number);
	printf("Allokert\n",number);
	}
	printf("Minne allokert\n");
	return 0;
		
}


void allocate(int value) {
	int *ptr = NULL;
	ptr = malloc(1024*1024*sizeof(char));
	*ptr = value;
	printf("test of allocated memory: %i\n");
}
