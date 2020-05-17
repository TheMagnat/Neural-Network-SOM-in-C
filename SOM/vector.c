#include "vector.h"

#include <stdlib.h>
#include <stdio.h>

void createVec(vector* vec, int size){

	vec->size = size;

	vec->data = malloc(size * sizeof(int));

	//Ajouter une verification

}

void deleteVec(vector* vec){
	free(vec->data);
}

void fillVec(vector* vec, int value){

	unsigned int i;

	for(i = 0; i < vec->size; ++i){

		vec->data[i] = value;

	}

}

void fillVecWithIndex(vector* vec){

	unsigned int i;

	for(i = 0; i < vec->size; ++i){

		vec->data[i] = i;

	}

}

void fillVecWithRandom(vector* vec, int maxValue){

	unsigned int i;

	for(i = 0; i < vec->size; ++i){

		vec->data[i] = rand()%maxValue;

	}

}




void shuffleVec(vector* vec){

	unsigned int i, size;

	int randomValue, save;

	size = vec->size;

	for(i = 0; i < size; ++i){

		randomValue = rand()%size;

		save = vec->data[i];
		vec->data[i] = vec->data[randomValue];
		vec->data[randomValue] = save;

	}

}

void printVec(vector* vec, char* separator){

	unsigned int i;

	for(i = 0; i < vec->size; ++i){

		printf("%d%s", vec->data[i], separator);

	}

}

void _quickSort(vector* vec, int from, int to){

	if(to - from <= 0) return;

	int i, small;
	int value, save;


	value = vec->data[to];

	for(i = from, small = from; i < to; ++i){

		if(vec->data[i] < value){

			save = vec->data[small];
			vec->data[small] = vec->data[i];
			vec->data[i] = save;
			++small;

		}

	}


	save = vec->data[small];
	vec->data[small] = value;
	vec->data[to] = save;

	_quickSort(vec, from, small - 1);
	_quickSort(vec, small + 1, to);


}

//Function to start the quick sort
void sort(vector* vec){
	_quickSort(vec, 0, vec->size - 1);
}
