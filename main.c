#include <stdio.h>
#include <stdlib.h>
#include "Util.h"


int main()
{
	vector v;
	init_vector(&v);
	
	int* array = NULL;
	
	for (int i = 0; i < 20; i++)
	{
		int* n = malloc(sizeof(int));
		*n = i;
		push_back(&v, n);
	}
	
	pop_back(&v);
	pop_front(&v);
	remove_at(&v, 2);
	remove_at(&v, 3);
	remove_at(&v, 4);
	remove_at(&v, 6);
	remove_at(&v, 9);
	
	for (int i = 0; i < v.size; i++)
	{
		printf("%i\n", *(int*)get_element(&v, i));
	}
	
	printf("Vector test complete!\n");
	system("pause");
	destroy_vector(&v);
	
	return 0;
}

