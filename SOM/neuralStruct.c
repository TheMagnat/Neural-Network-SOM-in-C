
#include "neuralStruct.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "vector.h"
#include "list.h"


//This function return a random value between from and to
double randomDouble(double from, double to){
	return ((double)rand()/(double)RAND_MAX) * (to - from) + from;
}


void generateNeuralMat(neuralMat* neurMat, learningMat* learnMat, unsigned int matLen[2], double bound[2]){

	unsigned int i, j;

	neuralVec tempoVec;


	tempoVec.size = learnMat->data[0].size;
	tempoVec.data = malloc(tempoVec.size * sizeof(double));

	//Set the first data to 0
	for(j = 0; j < tempoVec.size; ++j)
		tempoVec.data[j] = 0;


	//Calcul the sum of each vector
	for(i = 0; i < learnMat->size; ++i){

		for(j = 0; j < learnMat->data[i].size; ++j){

			tempoVec.data[j] += learnMat->data[i].data[j];

		}

	}

	//Divide to obtain the average
	for(j = 0; j < tempoVec.size; ++j)
		tempoVec.data[j] /= learnMat->size;


	//Fill the neuralMatrice
	neurMat->size = matLen[0] * matLen[1];
	neurMat->data = malloc(neurMat->size * sizeof(neuralVec));

	neurMat->width = matLen[0];
	neurMat->height = matLen[1];

	for(i = 0; i < neurMat->size; ++i){

		neurMat->data[i].size = tempoVec.size;
		neurMat->data[i].data = malloc(tempoVec.size * sizeof(double));

		for(j = 0; j < tempoVec.size; ++j){

			neurMat->data[i].data[j] = tempoVec.data[j] + randomDouble(bound[0], bound[1]);

		}

		neurMat->data[i].id = 0;

	}

	free(tempoVec.data);

}


void deleteNeuralMat(neuralMat* neurMat){

	unsigned int i;

	for(i = 0; i < neurMat->size; ++i){
		free(neurMat->data[i].data);
	}

	free(neurMat->data);

}


double dist(learningVec* lVec, neuralVec* nVec){

	unsigned int i;
	double sum;

	for(i = 0, sum = 0.0; i < lVec->size; ++i){
		sum += (nVec->data[i] - lVec->data[i]) * (nVec->data[i] - lVec->data[i]);

	}

	return sqrt(sum);

}

int splitedLearning(neuralMat* neurMat, learningMat* learnMat, vector* indexVector, unsigned int* currentIteration, unsigned int maxIteration, float maxAlpha, int maxArea){
	
	unsigned int areaSize, firstPartDuration, iteration;
	double alpha, tempoAlpha;

	iteration = *currentIteration;

	areaSize = maxArea;
	alpha = maxAlpha;

	firstPartDuration = maxIteration/4;

	if(iteration < firstPartDuration){
		tempoAlpha = alpha * (1.0 - ((double)iteration/(double)firstPartDuration));
		learn(neurMat, learnMat, indexVector, areaSize, tempoAlpha);
	}
	else if(iteration < maxIteration){
		tempoAlpha = (alpha * 0.1) * (1.0 - ((double)(iteration - firstPartDuration)/(double)(maxIteration - firstPartDuration)));
		learn(neurMat, learnMat, indexVector, areaSize/3, tempoAlpha);
	}
	//Equal to maxIteration
	else{
		setId(neurMat, learnMat, indexVector);
		return 1;
	}

	++(*currentIteration);

	return 0;

}

int oneByOneSplitedLearning(neuralMat* neurMat, learningMat* learnMat, vector* indexVector, unsigned int* currentData, unsigned int* currentIteration, unsigned int maxIteration, float maxAlpha, int maxArea){

	unsigned int areaSize, firstPartDuration, iteration;
	double alpha, tempoAlpha;

	iteration = *currentIteration;

	areaSize = maxArea;
	alpha = maxAlpha;

	firstPartDuration = maxIteration/4;

	if(iteration < firstPartDuration){
		tempoAlpha = alpha * (1.0 - ((double)iteration/(double)firstPartDuration));
		oneByOneLearn(neurMat, learnMat, indexVector, areaSize, tempoAlpha, *currentData);
	}
	else if(iteration < maxIteration){
		tempoAlpha = (alpha * 0.1) * (1.0 - ((double)(iteration - firstPartDuration)/(double)(maxIteration - firstPartDuration)));
		oneByOneLearn(neurMat, learnMat, indexVector, areaSize/3, tempoAlpha, *currentData);
	}
	//Equal to maxIteration
	else{
		setId(neurMat, learnMat, indexVector);
		return 1;
	}

	++(*currentData);

	if(*currentData == learnMat->size){

		*currentData = 0;
		++(*currentIteration);

	}

	

	return 0;

}

int finishCurrentOneByOneLearnPhase(neuralMat* neurMat, learningMat* learnMat, vector* indexVector, unsigned int* currentData, unsigned int* currentIteration, unsigned int maxIteration, float maxAlpha, int maxArea){

	unsigned int areaSize, firstPartDuration, iteration, i;
	double alpha, tempoAlpha;

	iteration = *currentIteration;

	areaSize = maxArea;
	alpha = maxAlpha;

	firstPartDuration = maxIteration/4;

	if(iteration < firstPartDuration){
		tempoAlpha = alpha * (1.0 - ((double)iteration/(double)firstPartDuration));
		for(i = *currentData; i < learnMat->size; ++i){
			oneByOneLearn(neurMat, learnMat, indexVector, areaSize, tempoAlpha, *currentData);
		}
	}
	else if(iteration < maxIteration){
		tempoAlpha = (alpha * 0.1) * (1.0 - ((double)(iteration - firstPartDuration)/(double)(maxIteration - firstPartDuration)));
		for(i = *currentData; i < learnMat->size; ++i){
			oneByOneLearn(neurMat, learnMat, indexVector, areaSize, tempoAlpha, *currentData);
		}
	}
	//Equal to maxIteration
	else{
		setId(neurMat, learnMat, indexVector);
		return 1;
	}

	*currentData = 0;
	++(*currentIteration);

	return 0;

}

void oneByOneLearn(neuralMat* neurMat, learningMat* learnMat, vector* indexVec, int areaSize, double alpha, unsigned int currentData){

	unsigned int i, selectedBestIndex;
	double tempoDist, bestDist;

	learningVec* tempoLearnVec;

	list bestList;

	//Initialize the list
	createList(&bestList);

	tempoLearnVec = &learnMat->data[indexVec->data[currentData]];
	for(i = 0, bestDist = DBL_MAX; i < neurMat->size; ++i){
		
		tempoDist = dist(tempoLearnVec, &neurMat->data[i]);
		if(tempoDist <= bestDist){
			if(tempoDist < bestDist){
				clearList(&bestList);
				bestDist = tempoDist;
			}
			pushFront(&bestList, i);

		}

	}//end for i

	//For the list
	selectedBestIndex = rand()%bestList.size;



	//For the neural matrice
	//selectedBestIndex = (unsigned int) getList(&bestList, selectedBestIndex);
	spread(neurMat, tempoLearnVec, getList(&bestList, selectedBestIndex), areaSize, alpha);


	deleteList(&bestList);

}


void startLearning(neuralMat* neurMat, learningMat* learnMat, unsigned int maxIt, float maxAlpha, unsigned int maxArea){

	unsigned int areaSize, i, iterations, firstPartDuration, secondPartDuration;
	double alpha, tempoAlpha;
	vector myIndexVec;

	//Index Vector
	createVec(&myIndexVec, learnMat->size);
	fillVecWithIndex(&myIndexVec);


	areaSize = maxArea;
	alpha = maxAlpha;
	iterations = maxIt;

	firstPartDuration = iterations/4;
	secondPartDuration = (iterations/4)*3;


	//Start learning
	
	for(i = 0; i < firstPartDuration; ++i){
		tempoAlpha = alpha * (1.0 - ((double)i/(double)firstPartDuration));
		shuffleVec(&myIndexVec);

		learn(neurMat, learnMat, &myIndexVec, areaSize, tempoAlpha);
	}

	for(i = 0; i < secondPartDuration; ++i){
		tempoAlpha = (alpha * 0.1) * (1.0 - ((double)i/(double)secondPartDuration));
		shuffleVec(&myIndexVec);

		learn(neurMat, learnMat, &myIndexVec, areaSize/3, tempoAlpha);
	}

	shuffleVec(&myIndexVec);

	setId(neurMat, learnMat, &myIndexVec);

	deleteVec(&myIndexVec);

}

void interpolate(neuralVec* neurVec, learningVec* learnVec,  double alpha){

	//unsigned register int i;
	unsigned int i;

	for(i = 0; i < neurVec->size; ++i){
		neurVec->data[i] += alpha * (learnVec->data[i] - neurVec->data[i]);
	}


}

/**
 * An alternative gaussian.
 * I used customized value to get something that look smooth.
 * 
*/
void gaussInterpolate(neuralVec* neurVec, learningVec* learnVec, int winX, int winY, int neurX, int neurY, double alpha){

	//unsigned register int i;
	unsigned int i;

	double sigma, gauss;

    // Gaussian neighborhood function
	double dist = sqrt(((winX - neurX) * (winX - neurX)) + ((winY - neurY) * (winY - neurY)));
	sigma = (double)1.5;
	
	gauss = exp(-((dist * dist) / (2.0 * (sigma * sigma))));
	//printf("dist = %f, h = %f\n", dist, h);


	for(i = 0; i < neurVec->size; ++i){
		neurVec->data[i] += gauss * alpha * (learnVec->data[i] - neurVec->data[i]);
	}


}

int verifyIfIn(neuralMat* neurMat, int x, int y){
	
	if(x >= 0 && y >= 0){
		if(x < (int)neurMat->width && y < (int)neurMat->height){
			return 1;
		}
	}

	return 0;

}

void spread(neuralMat* neurMat, learningVec* learnVec, int winnerIndex, int areaSize, double alpha){

	int areaDone;
	int xBound, yBound;

	int intWidth;

	int x, y;
	int newX, newY;

	int i;


	areaDone = 0;

	intWidth = neurMat->width;

	x = winnerIndex % intWidth;
	y = winnerIndex / intWidth;

	gaussInterpolate(&neurMat->data[winnerIndex], learnVec, x, y, x, y, alpha);

	for(xBound = 1, yBound = 1; areaDone < areaSize; ++areaDone, ++xBound, ++yBound){

		/*
		XXX
		-O-
		XXX
		*/
		for(i = x - xBound; i <= x + xBound; ++i){

			/*
			---
			-O-
			XXX
			*/
			newX = i;
			newY = y + yBound;
			if(verifyIfIn(neurMat, newX, newY)){
				//interpolate(&neurMat->data[newY * intWidth + newX], learnVec, alpha);
				gaussInterpolate(&neurMat->data[newY * intWidth + newX], learnVec, x, y, newX, newY, alpha);
			}

			/*
			XXX
			-O-
			---
			*/
			newY = y - yBound;
			if(verifyIfIn(neurMat, newX, newY)){
				//interpolate(&neurMat->data[newY * intWidth + newX], learnVec, alpha);
				gaussInterpolate(&neurMat->data[newY * intWidth + newX], learnVec, x, y, newX, newY, alpha);
			}

		}

		/*
		---
		XOX
		---
		*/
		for(i = y - yBound + 1; i < y + yBound; ++i){

			/*
			---
			-OX
			---
			*/

			newX = x + xBound;
			newY = i;
			if(verifyIfIn(neurMat, newX, newY)){
				//interpolate(&neurMat->data[newY * intWidth + newX], learnVec, alpha);
				gaussInterpolate(&neurMat->data[newY * intWidth + newX], learnVec, x, y, newX, newY, alpha);
			}

			/*
			---
			XO-
			---
			*/
			newX = x - xBound;
			if(verifyIfIn(neurMat, newX, newY)){
				//interpolate(&neurMat->data[newY * intWidth + newX], learnVec, alpha);
				gaussInterpolate(&neurMat->data[newY * intWidth + newX], learnVec, x, y, newX, newY, alpha);
			}

		}


	}


}

void learn(neuralMat* neurMat, learningMat* learnMat, vector* indexVec, int areaSize, double alpha){

	unsigned int i, j, selectedBestIndex;
	double tempoDist, bestDist;

	learningVec* tempoLearnVec;

	list bestList;

	//Initialize the list
	createList(&bestList);

	for(i = 0; i < indexVec->size; ++i){


		tempoLearnVec = &learnMat->data[indexVec->data[i]];
		for(j = 0, bestDist = DBL_MAX; j < neurMat->size; ++j){
			
			tempoDist = dist(tempoLearnVec, &neurMat->data[j]);
			if(tempoDist <= bestDist){
				if(tempoDist < bestDist){
					clearList(&bestList);
					bestDist = tempoDist;
				}
				pushFront(&bestList, j);

			}

		}//end for j

		//For the list
		selectedBestIndex = rand()%bestList.size;


		//For the neural matrice
		//selectedBestIndex = (unsigned int) getList(&bestList, selectedBestIndex);
		spread(neurMat, tempoLearnVec, getList(&bestList, selectedBestIndex), areaSize, alpha);
		//No need to clear the list, it will be automatly clear with the first value of the next i
	
	}//end for i

	deleteList(&bestList);

}

void resetId(neuralMat* neurMat){

	unsigned int i;

	for(i = 0; i < neurMat->size; ++i){
		neurMat->data[i].id = 0;
	}

}

void setId(neuralMat* neurMat, learningMat* learnMat, vector* indexVec){

	unsigned int i, j;
	double tempoDist, bestDist;

	learningVec* tempoLearnVec;

	list bestList;

	//Initialize the list
	createList(&bestList);


	for(i = 0; i < learnMat->size; ++i){

		tempoLearnVec = &learnMat->data[indexVec->data[i]];
		
		for(j = 0, bestDist = DBL_MAX; j < neurMat->size; ++j){

			tempoDist = dist(tempoLearnVec, &neurMat->data[j]);


			if(tempoDist <= bestDist){

				if(tempoDist < bestDist){
					clearList(&bestList);
					bestDist = tempoDist;
				}

				pushFront(&bestList, j);

			}


		}//end for j


		//For the list
		// selectedBestIndex = rand()%bestList.size;

		// if(bestList.size > 1) printf("Plus que 1\n");

		for(j = 0; j < bestList.size; ++j){
			// if(bestList.size > 1) printf("Index : %d\n", getList(&bestList, j));
			// if(neurMat->data[getList(&bestList, j)].id != 0){
			// 	printf("Deja labeled\n");
			// }
			// else{
			neurMat->data[getList(&bestList, j)].id = tempoLearnVec->id;
			// }
		}

		//For the neural matrice
		//selectedBestIndex = (unsigned int) getList(&bestList, selectedBestIndex);
		//printf("selected neuro = %d\n", getList(&bestList, selectedBestIndex));
		
		//neurMat->data[getList(&bestList, selectedBestIndex)].id = tempoLearnVec->id;


		//No need to clear the list, it will be automatly clear with the first value of the next i
	
	}//end for i

}

void printNeuralMat(neuralMat* neurMat){

	unsigned int count;

	for(count = 0; count < neurMat->size; ++count){
		printf("Num %d : 1 == %f, 2 == %f, 3 == %f, 4 == %f, id = %d\n", count, neurMat->data[count].data[0], neurMat->data[count].data[1], neurMat->data[count].data[2], neurMat->data[count].data[3], neurMat->data[count].id);
	}

}

/**
 * Note : Since this function has color,
 * it wont use color for data that exceed 3 data type.
*/
void visualize(neuralMat* neurMat){

	unsigned int x, y;

	for(y = 0; y < neurMat->height; ++y){
		
		for(x = 0; x < neurMat->width; ++x){
			
			switch(neurMat->data[x + y * neurMat->width].id){
				case 1:
					printf("\x1B[32m1 ");
					break;

				case 2:
					printf("\x1B[35m2 ");
					break;

				case 3:
					printf("\x1B[31m3 ");
					break;

				default:
					printf("\x1B[39m0 ");
					break;
			}

		}

		printf("\n\x1B[39m");

	}

}


