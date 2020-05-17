
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "learningStruct.h"
#include "neuralStruct.h"
#include "vector.h"



//Global Info
//This structure will contain all the info about the current learning data.
struct info{
	unsigned int nbLine;
	unsigned int nbData;
	
	unsigned int matLen[2];

	double bound[2];

	dataLabel allLabels;

};
typedef struct info info;



int main (void){

	unsigned int i;

	srand(time(NULL));

	info mainInfo;

	neuralMat neurMat;
	learningMat allIris;


	//We fill the learning data constant
	mainInfo.nbLine = 150;
	mainInfo.nbData = 4;
	
	mainInfo.bound[0] = -0.05;
	mainInfo.bound[1] = 0.025;
	mainInfo.matLen[0] = 10;
	mainInfo.matLen[1] = 6;

	//We also set the label of the data
	mainInfo.allLabels.size = 3;
	mainInfo.allLabels.labels = malloc(mainInfo.allLabels.size * sizeof(char*));
	mainInfo.allLabels.labels[0] = "Iris-setosa";
	mainInfo.allLabels.labels[1] = "Iris-versicolor";
	mainInfo.allLabels.labels[2] = "Iris-virginica";


	generateLearningMat(&allIris, "iris.data", mainInfo.nbLine, mainInfo.nbData, mainInfo.allLabels);
	generateNeuralMat(&neurMat, &allIris, mainInfo.matLen, mainInfo.bound);

	startLearning(&neurMat, &allIris, 2000, 0.9, 3);
	
	//Learning done, now print the matrice	
	visualize(&neurMat);

	printf("\n");
	for(i = 0; i < mainInfo.allLabels.size; ++i){
		switch(i+1){
			case 1:
				printf("\x1B[32m");
				break;

			case 2:
				printf("\x1B[35m");
				break;

			case 3:
				printf("\x1B[31m");
				break;

			default:
				printf("\x1B[39m");
				break;
		}
		printf("%d = %s\n\x1B[39m", i+1, mainInfo.allLabels.labels[i]);
	}


	free(mainInfo.allLabels.labels);


	deleteNeuralMat(&neurMat);
	deleteLearningMat(&allIris);

	return 0;

}
