#include "list.h"
#include <stdlib.h>
#include <stdio.h>

void createList(list *l){

	l->size = 0;

	l->start = malloc(sizeof(node));
	l->end = malloc(sizeof(node));


	l->start->next = l->end;

}

void deleteList(list *l){

	clearList(l);

	free(l->start);
	free(l->end);

}

void pushFront(list *l, int val){

	node* newNode;


	newNode = malloc(sizeof(node));
	newNode->value = val;


	newNode->next = l->start->next;
	l->start->next = newNode;

	++l->size;
}

int getList(list* l, unsigned int index){

	unsigned int count;
	node *it;

	for(it = l->start->next, count = 0; count < index; it = it->next, ++count);


	return it->value;


}

void clearList(list* l){

	node *it, *tempo;

	for(it = l->start->next; it != l->end; ){

		tempo = it;
		it = it->next;

		free(tempo);

	}

	l->start->next = l->end;

	l->size = 0;

}

void printList(list* l){

	node* it;

	for(it = l->start->next; it != l->end; it = it->next){

		printf("Value : %d\n", it->value);

	}

}






