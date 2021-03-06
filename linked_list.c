#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{
	list_t empty_list = malloc(sizeof(struct list));
	empty_list->head = NULL;
	empty_list->head = NULL;
	empty_list->length = 0;
	return empty_list;
}

void list_delete(list_t list)
{
	struct node *current_node = list->head;
	while (current_node != NULL) {
		struct node *next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}
	free(list);
}

void list_insert(list_t list, int index, int data)
{
	struct node *new_node = malloc(sizeof(struct node));
	new_node->data = data;
	new_node->prev = NULL;
	new_node->next = NULL;
	if (index == 0) {
		if (list->head == NULL) {
			list->head = new_node;
			list->tail = new_node;
		} else {
			new_node->next = list->head;
			list->head->prev = new_node;
			list->head = new_node;
		}
	} else if (index == list->length){
		new_node->prev = list->tail;
		list->tail->next = new_node;
		list->tail = new_node;
	} else {
		int i = 0;
		struct node *node_prev = list->head;
		for (i = 1; i < index; i++) {
			node_prev = node_prev->next;
		}
		struct node *node_next = node_prev->next;
		node_prev->next = new_node;
		node_next->prev = new_node;
		new_node->prev = node_prev;
		new_node->next = node_next;
	}
	
	list->length++;
}

void list_append(list_t list, int data)
{
	printf("adding number\n");
	struct node *new_node = malloc(sizeof(struct node));
	new_node->data = data;
	new_node->prev = NULL;
	new_node->next = NULL;
	
	if (list->length == 0) {
		list->head = new_node;
		list->tail = new_node;
	} else {
		new_node->prev = list->tail;
		list->tail->next = new_node;
		list->tail = new_node;
	}
	list->length++;
	printf("added number %i\n",data);
}

void list_print(list_t list)
{
	struct node *current_node = list->head;
	while (current_node != NULL) {
		struct node *next_node = current_node->next;
		printf("%i ", current_node->data);
		current_node = next_node;
	}	
}

long list_sum(list_t list)
{
	long sum = 0;
	struct node *current_node = list->head;
	while (current_node != NULL) {
		struct node *next_node = current_node->next;
		sum += current_node->data;
		current_node = next_node;
	}
	return current_node->data;
}

int list_get(list_t list, int index)
{
	int i = 0;
	struct node *node_current = list->head;
	for (i = 1; i < index; i++) {
		node_current = node_current->next;
	}
	return node_current->data;
}

int list_extract(list_t list, int index)
{
	int i = 0;
	struct node *node_current = list->head;
	for (i = 1; i < index; i++) {
		node_current = node_current->next;
	}
	node_current->prev->next = node_current->next;
	node_current->next->prev = node_current->prev;
	free(node_current);
	
	return node_current->data;
}
