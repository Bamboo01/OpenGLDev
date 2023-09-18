#include <stdio.h>
#include <stdlib.h>
#include "Util.h"


int main()
{
	vector v;
	init_vector(&v);
	
	int* array = NULL;
	
	for (int i = 0; i < 3; i++)
	{
		int* n = malloc(sizeof(int));
		*n = i;
		push_back(&v, n);
	}
	
	pop_back(&v);
	
	for (int i = 0; i < v.size; i++)
	{
		printf("%i\n", *(int*)get_element(&v, i));
	}
	
	printf("Test\n");
	system("pause");
	return 0;
}

