#include "Util.h"
#include <stdlib.h>
#define MAX_DEPTH 3
#define NODE_SIZE sizeof(node)
#define NODE_POINTER_SIZE sizeof(node*)

/*
 * This is a C++ style vector that dynamically allocates memory for void* pointers
 *
 * Reason for this is because I want to have a generic array that can efficiently
 * push and remove elements for future C projects
 *
 * In order to optimise performance, this uses a linked list that has different nodes as steps
 * They're stored in a array that are spaced out based on the maximum depth the traversal can go into
 *
 * INDEX OF CHECKPOINT where traversal depth is 4
 *
 * |
 * v
 * 0-----------1-----------2-----------3-------
 *
 * E1 E2 E3 E4|E5 E6 E7 E8|E9 EA EB EC|EE EF EG
 *
 * In this case, we can easily access the elements we want using an array style index by simply
 * finding out which step the index should exist in (i.e. index 5 exists in step 1)
 * Next, we just need to traverse the nodes based on the remaining number of elements we have left
 *
 *
 * Bamboo01 15-09-2023 
*/

// PS: This is my first few times writing in C, forgive my bad practices D:

typedef struct node node;

struct node
{
	struct node* next;
	void* nodePointer;
};

// DEBUG
 void printShid(node* start)
 {
 	printf("------------------------------------\n");
 	if (start->nodePointer)
 	{
 		printf("Node %p, value %i, next %p \n", start, *(int*)start->nodePointer, start->next);
 	}
 	else
 	{
 		printf("Node %p, value 0x0, next %p \n", start, start->next);
 	}
 	
 	if (!start->next)
 		return;
 	else
 		printShid(start->next);
 }
 void printSteps(node* start)
 {
 	printf("==================PRINTING STEPS==================\n");
 	
 	node* n = start;
 	
 	for(int i = 0; i < MAX_DEPTH; i++)
 	{
 		if (n->nodePointer)
 		{
 			printf("Node %p, value %i, next %p \n", n, *(int*)n->nodePointer, n->next);
 		}
 		else
 		{
 			printf("Node %p, value 0x0, next %p \n", n, n->next);
 		}
 		
 		n = n->next;
 	}
 	
 	printf("==================PRINTING STEPS==================\n");
 }

// NODE HELPER FUNCTIONS
node* create_node()
{
	node* temp = malloc(NODE_SIZE);
	temp->next = NULL;
	temp->nodePointer = NULL;
	
	return temp;
}

node* traverse_linked_list(const node* n, int* curr_depth, const int depth)
{
	if (*curr_depth >= depth)
	{
		return n;
	}
	else if (n->next == NULL)
	{
		return NULL;
	}
	else
	{
		(*curr_depth)++;
		traverse_linked_list(n->next, curr_depth, depth);	
	}
}

node* create_node_array()
{
	node** nodeArr = malloc(NODE_POINTER_SIZE * MAX_DEPTH);
	
	nodeArr[0] = create_node();
	node* firstHead = nodeArr[0];	
		
	for (int i = 1; i < MAX_DEPTH; i++)
	{
		nodeArr[i] = create_node();
		nodeArr[i - 1]->next = nodeArr[i];
	}
	
	// Fucking C shenanigans
	for (int i = 0; i < MAX_DEPTH; i++)
	{
		nodeArr[i] = NULL;
	}
	
	free(nodeArr);
	
	// Returns the head of this node
	return firstHead;
}

node* get_node_at_index(const vector* v, int index)
{
	if (index < 0 || index >= v->capacity)
		return NULL;
	
	int linkedListIndex = index / MAX_DEPTH;
	int linkedListTargetDepth = index - (linkedListIndex * MAX_DEPTH);
	
	// Empty node
	int currDepth = 0;
	node* startNode = NULL;
	startNode = ((node**)v->data)[linkedListIndex];
	node* targetNode = NULL;
	targetNode = traverse_linked_list(startNode, &currDepth, linkedListTargetDepth);
	
	if (!targetNode)
		return NULL;
	else
		return targetNode;
}

// UTIL FUNCTIONS
int init_vector(vector* v)
{
	int test = NODE_SIZE;
	v->capacity = MAX_DEPTH;
	v->size = 0;
	v->data = malloc(NODE_POINTER_SIZE);
	((node**)v->data)[0] = create_node_array();
	return 0;
}

int push_back(vector* v, void* item)
{
	// Empty node
	node* startNode = NULL;
	
	// Find out which linked list this item will be indented to
	node* targetNode = get_node_at_index(v, v->size);
		targetNode->nodePointer = item;
	v->size++;
	
	// Check if we have reached max capacity
	if (v->size == v->capacity)
	{
		int linkedListIndex = v->size / MAX_DEPTH;
		int numLists = linkedListIndex + 1;
		
		// Realloc to add new node pointer
		void* temp = NULL;
		temp = realloc(v->data, (numLists * NODE_POINTER_SIZE));
		
		// Error checking...
		if (temp != NULL)
			v->data = temp;
		else
			return -1;
		
		// Create new node array and assign the head to the array
		node* newHead = create_node_array();
		((node**)v->data)[linkedListIndex] = newHead;
		
		// Update the capacity of the list
		v->capacity = MAX_DEPTH * numLists;
		
		// Point previously node to this new node
		targetNode->next = newHead;
	}
		
	return 0;
}

void* get_element(vector* v, int index)
{
	node* targetNode = get_node_at_index(v, index);
	
	if (!targetNode)
		return NULL;
	else
		return targetNode->nodePointer;
}

int pop_front(vector* v)
{
	printShid(((node**)v->data)[0]);
	
	// We add an empty node to the end to account for the removal of the current node
	node* finalNode = get_node_at_index(v, v->capacity - 1);
	finalNode->next = create_node();
	
	int numSteps = v->capacity / MAX_DEPTH;
	
	// Set the node in front of the first element to be the new start
	node* front = ((node**)v->data)[0];
	
	// We need to reshuffle the whole array
	for(int i = 0; i < numSteps; i++)
	{	
		((node**)v->data)[i] = (((node**)v->data)[i])->next;
		printSteps(((node**)v->data)[i]);
	}
	
	free(front);
	//printShid(((node**)v->data)[0]);
	
	v->size--;
	
	return 0;
}

int pop_back(vector* v)
{
	printShid(((node**)v->data)[0]);
	node* secondRearNode = get_node_at_index(v, v->size - 2);
	node* rearNode = secondRearNode->next;
	
	if (v->size == v->capacity)
	{
		printShid(((node**)v->data)[0]);
		secondRearNode->next = create_node();
		printShid(((node**)v->data)[0]);
	}
	else
	{
		// We add an empty node to the end to account for the removal of the current node
		node* finalNode = get_node_at_index(v, v->capacity - 1);
		finalNode->next = create_node();
		
		secondRearNode->next = rearNode->next;
		printShid(((node**)v->data)[0]);	
	}
	
	free(rearNode->nodePointer);
	printShid(((node**)v->data)[0]);
	free(rearNode);
	printShid(((node**)v->data)[0]);
	v->size--;
	
	return 0;
}


int remove_at(vector* v, int index)
{	
	if (index == v->size - 1)
		return pop_back(v);
	else if (index == 0)
		return pop_front(v);

	// We add an empty node to the end to account for the removal of the current node
	node* finalNode = get_node_at_index(v, v->capacity - 1);
	finalNode->next = create_node();
	
	// For every step ahead of the step this index is in, we push it to point towards the next node
	int numSteps = v->capacity / MAX_DEPTH;
	int linkedListIndex = (index / MAX_DEPTH);
	
	for(int i = linkedListIndex + 1; i < numSteps; i++)
	{		
		((node**)v->data)[i] = (((node**)v->data)[i])->next;
	}
	
	node* befTargetNode = get_node_at_index(v, index - 1);
	node* targetNode = befTargetNode->next;		
	befTargetNode->next = targetNode->next;
	free(targetNode->nodePointer);
	free(targetNode);
	
	v->size--;
	
	return 0;
}

int destroy_vector(vector* v)
{
	
}
