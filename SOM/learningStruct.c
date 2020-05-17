#include "learningStruct.h"

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This constant contain the max size of a char buffer
#define MAXCHAR 1024

void generateLearningMat(learningMat* mat, char* fileName, unsigned int nbLine, unsigned int nbData, dataLabel allLabels){
	openAndReadLearningData(fileName, mat, nbLine, nbData, allLabels);
	normalizeLearningMat(mat);
}

void deleteLearningMat(learningMat* mat){

	unsigned int i;

	for(i = 0; i < mat->size; ++i){

		free(mat->data[i].data);

	}

	free(mat->data);

}


void pickData(char* str, learningVec* toFill, unsigned int nbData, dataLabel allLabels){

	unsigned int n;

	//Note : It's 3 characteres because it count the '\0' at the end.
	const char s[3] = ",\n";
	char *token;

	toFill->data = malloc(nbData * sizeof(double));
	toFill->size = nbData;

	token = strtok(str, s);

	for(n = 0; n < nbData; ++n){
		toFill->data[n] = atof(token);
		token = strtok(NULL, s);
	}

	for(n = 0; n < allLabels.size; ++n){
		if(strcmp(allLabels.labels[n], token) == 0){
			toFill->id = n+1;
		}
	}

}

void openAndReadLearningData(char* filename, learningMat* learnMat, unsigned int nbLine, unsigned int nbData, dataLabel allLabels){
	
	unsigned int count;

	FILE *fp;
	char str[MAXCHAR];

    fp = fopen(filename, "r");

    //THIS COUNT LINES
	// for (c = getc(fp), count = 0; c != EOF; c = getc(fp)){
 //        if (c == '\n'){
	// 		if(count){
	// 			lineNumber = lineNumber + 1;
	// 		}
 //        	count = 0;
 //        }
 //        else if(c == '\r'){
 //        	continue;
 //        }
 //        else{
 //        	++count;
 //        }
 //    }


	// fseek(fp, 0, SEEK_SET);





	learnMat->size = nbLine;
	learnMat->data = malloc(learnMat->size * sizeof(learningVec));


	if (fp == NULL){
		printf("Could not open file %s",filename);
		exit(1);
	}

	for(count = 0; count < nbLine; ++count){
		if(!fgets(str, MAXCHAR, fp)){
			printf("Error, can't read line %d\n", count);
		}

		pickData(str, &learnMat->data[count], nbData, allLabels);

	}


	fclose(fp);

}


void normalizeLearningMat(learningMat* mat){

	unsigned int i, j;
	double sum;


	for(i = 0; i < mat->size; ++i){
		
		sum = 0;
		
		for(j = 0; j < mat->data[i].size; ++j)
			sum += mat->data[i].data[j] * mat->data[i].data[j];

		sum = sqrt(sum);
		mat->data[i].norme = sum;

		
		for(j = 0; j < mat->data[i].size; ++j){
			mat->data[i].data[j] /= sum;
		}

	}

}
