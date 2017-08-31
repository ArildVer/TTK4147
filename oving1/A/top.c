#include <stdio.h>
#include <stdlib.h>

int allocate(int value);

int main(int argc, char *argv[])
{
	printf("Hello world2\n");
	int i = 0;
	for(i = 0; i>=0; i++)
	{
	if (!allocate(i)){
		return 1;
	}
	if (i%250 == 0) {
		printf("Allokert%i\n",i);
	}
	}
	printf("Minne allokert\n");
	return 0;
		
}


int allocate(int value) {
	int *ptr = NULL;
	ptr = malloc(1024*1024*sizeof(char));
	if (ptr == NULL){
		perror("ERROR: ");
		return 0;
	}
	*ptr = value;
	return 1;
	//printf("test of allocated memory: %i\n",value);
}
