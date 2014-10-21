/*Genetic Algorithm for PacWar*/
using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 
#include <map>
#include "PacWar.h"
#include "PacWarGuts.h"

/*Main function*/
const int GENE_LENGTH = 50;
const int SAVE_THRESHOLD = 10000;
int main(int argc, const char* argv[]){

	int populationSize, opponentSize, crossOverSite;
	double mutationRate;

	// ensure the correct number of parameters are used.
	if ( argc == 4 )
	{
		populationSize = atoi( argv[1] );
		opponentSize = atoi( argv[2] );
		mutationRate = atof( argv[3] );
		//crossOverSite = atoi( argv[4] );
 		geneticAlgorithm(populationSize, opponentSize, mutationRate);
	}
	else{
	/*something is wrong*/
	}
	return 0;
}
/*Main entrance of the algorithm
*/
void geneticAlgorithm(int populationSize, int opponentSize, double mutationRate){
	char *[] populationGenes = generateGenes(populationSize);
	char *[] opponentGenes = generateGenes(opponentSize);
	int [] scores = new int[populationSize];
	int totalScore = 0;
	int counter = 0;
	int substitutionCount = 0;
	while(true){

		totalScore = evaluation(populationGenes, opponentGenes, scores);
		selection(populationGenes, totalScore, scores);
		substituteOpponents(populationGenes, opponentGenes, substitutionCount);
		substitutionCount ++;
		if(substitutionCount > opponentSize/populationGenes ){
			substitutionCount = 0;
		}
		
		crossOver(populationGenes);
		mutation(populationGenes, mutationRate);
		counter ++;
		if(counter == SAVE_THRESHOLD){
			saveToFile(populationGenes);
			counter = 0;
		}
	}
	/**/

}
/*Selection Cheng
Given the old generation, and their respective scores, generate a new generation.
The prevalance of a certain genetype is linearly correlated to the corresponding scores divided
by scores of the whole population
*/
#include <algorithm>
void selection(char &populationGenes[][GENE_LENGTH], int totalScore, int & scores[]){
	int position = 0, length = scores.size(), numToCopy = 0;
	char geneCopy[length][populationGenes[1].size()] = populationGenes;
	for(int i = 0; i < length && position < length; i++){
		numToCopy = (scores[i] * length) / totalScore;
		cout<<"=== At index: "<<i<<", numToCopy = "<<numToCopy<<endl;
		for(int j = 0; j < numToCopy; j++){
			populationGenes[position] = geneCopy[i];
			position++;
		}
	}
	//shuffle:
	random_shuffle(populationGenes.begin(), populationGenes.end());
}

/*EvaluationFunction Peng
Give the current generation of populations. Let them fight with a large group of opponents and
return their respective scores.
Hashmap will be invoked in this function.
*/
int evaluation(char *[] populationGenes, char *[] opponentGenes, int [] scores){

	std::map<std::string, int> mapPopulations;
	/*score[i] = calculated scores*/
	int i =0;
	int j = 0;
	string tempGene;
	int tempScore;
	char * populationGenePointer;
	char * opponentGenePointer;
	PacGene genes[2];  /* Genes for the two species */
	PacGenePtr genePointers[2] = {&genes[0], &genes[1]};
	for(i = 0; i < populationSize; i ++){		
		tempGene = string(populationGenes[i]);
		tempScore = 0;

		if (mapPopulations.find(tempGene) == mapPopulations.end()){
			populationGenePointer = &populationGenes[i];
			SetGeneFromString(populationGenePointer, genePointers[0]);
			for(j = 0; j < opponentSize; j ++){
				opponentGenePointer = opponentGenes[j];
			SetGeneFromString(populationGenePointer, genePointers[0]);
			}	
		}
		
	}

	return totalScore;
}

/*crossOver Peng
cross over all individuals in the population in the way that the populationGenes[0]
cross over with the populationGenes[N-1],  populationGenes[1] with populationGenes[N-2], etc.*/
void crossOver(char *[] populationGenes){
	int i = 0;
	int crossOverSite;
	int j = 0;
	char * tempSequence ;// = new char[GENE_LENGTH/2 + 1];
	PacGenePtr g1, PacGenePtr g2;
	tempSequence = (char *) malloc(sizeof(char) * (GENE_LENGTH/2 + 1));
	for (i = 0; i < (size/2); i ++){
		
		crossOverSite = rand()%(GENE_LENGTH/2) + (GENE_LENGTH/2);
		cout<<" cross over site is "<<crossOverSite<<endl;
		for(j = crossOverSite; j < GENE_LENGTH; j ++){
			tempSequence[j - crossOverSite] = populationGenes[i][j];
		} 
		cout<<" cut piece "<<endl;
		for(j = crossOverSite; j < GENE_LENGTH; j ++){
			cout<<tempSequence[j - crossOverSite];
		}
		cout<<endl;
		for(j = crossOverSite; j < GENE_LENGTH; j ++){
			populationGenes[i][j] = populationGenes[size - 1 - i][j];
		}
		for(j = crossOverSite; j < GENE_LENGTH; j ++){
			populationGenes[size - 1 - i][j] = tempSequence[j - crossOverSite];
		}
	}
	/*no return value*/
}

/*Mutation Peng
Mutate the whole population with a given mutation rate.
No mutation on the same site in one generation is allowed.
*/

void mutation(char * []populationGenes, double mutationRate){

	int site;
	int threshold = 0;
	int nucleotide;
	int current;
	char temp[4];
	int i = 0;
	int oldSite;
	int j = 0;
	threshold = int(1/mutationRate);
	double times = (double)GENE_LENGTH/threshold;
	cout<<threshold<<" and "<<times<<endl;
	for(i = 0; i < size; i ++){
		if(times <= 1.0){

			cout<<" enter single mutation "<<endl;
			site = 	rand()%threshold;
			cout<<" site is "<<site<<endl;
			if(site < GENE_LENGTH){

				nucleotide = rand()%4;
				current = populationGenes[i][site] - '0';
				while(current == nucleotide){
					nucleotide = rand()%4;
				}
				snprintf(temp, sizeof(temp), "%d", nucleotide);
				populationGenes[i][site] = temp[0]; 
			}
		}
		else{
			cout<<" enter multiple mutations "<<endl;
			oldSite = -1;
			site = -1;
			for(j=0; j < (int)times + 1; j ++){
				while(oldSite == site){

					site = 	rand()%GENE_LENGTH;
				}
				oldSite = site;			
				cout<<" site is "<<site<<endl;

				nucleotide = rand()%4;
				current = populationGenes[i][site] - '0';
				while(current == nucleotide){
					nucleotide = rand()%4;
				}
				snprintf(temp, sizeof(temp), "%d", nucleotide);
				populationGenes[i][site] = temp[0]; 	
			}
		}
	}
	/*no return value*/
}
/*substituteOpponents Cheng
substitute some of the opponents with population genes
indexing from substitutionCount * populationGenes.size() to (substitutionCount + 1) * populationGenes.size() - 1if reachable
*/
void substituteOpponents(char &populationGenes[][], char &opponentGenes[][], int &substitutionCount){

	int size += populationGenes.size();
	int length = opponentsGenes.size();
	for(int i = 0; i < size; i++){
		opponentGenes[(i + substitutionCount) % length] = populationGenes[i];
	}
	substitutionCount = (size + substitutionCount) % length;
}
/*Initialization Peng
Randomize genes for both populationGenes and opponentGenes.
*/
char *[] generateGenes(int size){
	char *[] genes = new char *[size];
	int i = 0;
	int j = 0;
	int nucleotide;
	for(; i < size; i++){
		for(; j < GENE_LENGTH; j ++){
			nucleotide = rand(0,4);
			genes[i][j]	 =  itoa(nucleotide);
		}

	}
	/*return genes with randomized nucleotides*/
}
/*saveToFile   Cheng
save genes to a file
*/
void saveToFile(char &populationGenes[][GENE_LENGTH], int [] & scores){
	ofstream myfile;
	myfile.open ("result.txt");
	myfile << "==================== Current Top Genes ====================\n";
	priority_queue<pair<int, int>> q;
	for (int i = 0; i < scores.size(); ++i) {
		q.push(pair<int, int>(scores[i], i));
	}
	int k = 10; // number of indices we need
	for (int i = 0; i < k; ++i) {
		int ki = q.top().second;
		myfile << "Strong Gene [" << (i + 1) << "]: "<< populationGenes[ki] << endl;
		q.pop();
	}
	myfile.close();
}
