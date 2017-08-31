#include <stdio.h>
#include <stdlib.h>

void allocate(int value);

int main(int argc, char *argv[])
{
	printf("Hello world2\n");
	int i = 0;
	for(i = 0; i>=0; i++)
	{
	allocate(i);
	if (i%250 == 0) {
		printf("Allokert%i\n",i);
	}
	}
	printf("Minne allokert\n");
	return 0;
		
}


void allocate(int value) {
	int *ptr = NULL;
	ptr = malloc(1024*1024*sizeof(char));
	*ptr = value;
	//printf("test of allocated memory: %i\n",value);
}
