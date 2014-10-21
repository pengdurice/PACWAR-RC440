using namespace std;
#include<iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 
#include <map>
#include "PacWar.h"
#include "PacWarGuts.c"
//#include "PacWarGuts.h"
#include <ctime>
const int GENE_LENGTH = 51;

void generateGenes(char * genes[], int size, int GENE_LENGTH){

	int i = 0;
	int j = 0;
	int nucleotide;
	char temp[4];
	for(i = 0; i < size; i++){
		for(j = 0; j < GENE_LENGTH - 1; j ++){
			nucleotide = rand()%4;
			snprintf(temp, sizeof(temp), "%d", nucleotide);
			genes[i][j] = temp[0];
			//cout << genes[i][j];
		}
		genes[i][GENE_LENGTH - 1] ='\0';
		//cout<<endl;
	}
}
void mutation(char * populationGenes[], double mutationRate, int size, int GENE_LENGTH){

	int site;
	int threshold = 0;
	int nucleotide;
	int current;
	char temp[4];
	int i = 0;
	int oldSite;
	int j = 0;
	threshold = int(1/mutationRate);
	double times = (double)(GENE_LENGTH - 1)/threshold;
	cout<<threshold<<" and "<<times<<endl;
	for(i = 0; i < size; i ++){
		if(times <= 1.0){

			cout<<" enter single mutation "<<endl;
			site = 	rand()%threshold;
			cout<<" site is "<<site<<endl;
			if(site < GENE_LENGTH - 1){

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
			//oldSite = -1;
			site = -1;
			for(j=0; j < (int)times + 1; j ++){
				/*while(oldSite == site){

					site = 	rand()%(GENE_LENGTH - 1);
				}*/
				//oldSite = site;	
				site = 	rand()%(GENE_LENGTH - 1);		
				cout<<" site is "<<site<<endl;
;
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
	//no return value
}
void crossOver(char * populationGenes[], int size, int GENE_LENGTH){
	int i = 0;
	int crossOverSite;
	int j = 0;
	char * tempSequence ;// = new char[GENE_LENGTH/2 + 1];

	tempSequence = (char *) malloc(sizeof(char) * ((GENE_LENGTH - 1)/2 + 1));
	for (i = 0; i < (size/2); i ++){
		
		crossOverSite = rand()%((GENE_LENGTH - 1)/2) + ((GENE_LENGTH - 1)/2);
		cout<<" cross over site is "<<crossOverSite<<endl;
		for(j = crossOverSite; j < GENE_LENGTH - 1; j ++){
			tempSequence[j - crossOverSite] = populationGenes[i][j];
		} 
		cout<<" cut piece "<<endl;
		for(j = crossOverSite; j < GENE_LENGTH - 1; j ++){
			cout<<tempSequence[j - crossOverSite];
		}
		cout<<endl;
		for(j = crossOverSite; j < GENE_LENGTH - 1; j ++){
			populationGenes[i][j] = populationGenes[size - 1 - i][j];
		}
		for(j = crossOverSite; j < GENE_LENGTH - 1; j ++){
			populationGenes[size - 1 - i][j] = tempSequence[j - crossOverSite];
		}
	}
	/*no return value*/
}
int calculateScore(int rounds, int count1, int count2){
	int score = 0;
	if(count1 == 0){
		if(count2 == 0){
			score = 10;				
		}
		else{
			if(rounds < 100){
				score = 0;
			}
			else if (rounds >= 100 and rounds < 200){
				score = 1;
			}
			else if (rounds >= 200 and rounds < 300){
				score = 2;
			}
			else if (rounds >= 300 and rounds <= 500){
				score = 3;
			}	
		}
	}
	else{
		if(count2 == 0){
			if(rounds < 100){
				score = 20;
			}
			else if (rounds >= 100 and rounds < 200){
				score = 19;
			}
			else if (rounds >= 200 and rounds < 300){
				score = 18;
			}
			else if (rounds >= 300 and rounds <= 500){
				score = 17;
			}
		}
		else{
			if((float)count1/count2 >= 10.0){
				score = 13;
			}
			else if((float)count1/count2 >= 3.0 and (float)count1/count2 < 10.0){
				score = 12;
			}
			else if((float)count1/count2 >= 1.5 and (float)count1/count2 < 3.0){
				score = 11;
			}
			else if((float)count1/count2 >= 0.666666667 and (float)count1/count2 < 1.5){
				score = 10;
			}
			else if((float)count1/count2 >= 0.333333333 and (float)count1/count2 < 0.666666667 ){
				score = 9;
			}
			else if((float)count1/count2 >= 0.1 and (float)count1/count2 < 0.333333333 ){
				score = 8;
			}
			else if((float)count1/count2 < 0.1 ){
				score = 7;
			}
		}
	}
	return score;
}
int evaluation(char* populationGenes[], char* opponentGenes[], int populationSize, int opponentSize, int scores[], int GENE_LENGTH){
	std::map<std::string, int> mapPopulations;	
	int i =0;
	int j = 0;
	string tempGene;
	int totalScore = 0;
	char * populationGenePointer;
	char * opponentGenePointer;
	PacGene genes[2];  
	int rounds = 0;
	int count1 = 0;
	int count2 = 0;
	char * temps;
	PacGenePtr genePointers[2] = {&genes[0], &genes[1]};
	
				/*temps = (char *)genePointers[0];
			cout<<"print original genes"<<endl;
			for(int x = 0; x < 50; x ++){
				cout<<*temps;
				temps ++;
			}*/
	for(i = 0; i < populationSize; i ++){	
		scores[i] = 0;	
		tempGene = string(populationGenes[i]);
		cout<<"the string is :"<<tempGene<<endl;
		//tempScore = 0;

		if (mapPopulations.find(tempGene) == mapPopulations.end()){
			//populationGenePointer = populationGenes[i];
			//populationGenePointer = "11111111111111111111111111111111111111111111111111";
			SetGeneFromString(populationGenePointer, genePointers[0]);
			
			/*for(int x = 0; x < 50; x ++){
				cout<<populationGenePointer[x];
			}
			cout<<endl;*/
			//temps = (char *)genePointers[0];
			temps = NewStringFromGene(genePointers[0], temps);
			cout<<"print its genes"<<endl;
			for(int x = 0; x < 50; x ++){
				cout<<(char)*temps;
				temps ++;
			}
			cout<<endl;
			//cout<<"tttt"<<endl;
			//cout<<(signed char *)genePointers[0]->u<<"   "<<(signed char *)genePointers[0]->w<<endl;
			//cout<<"ssss"<<endl;
			for(j = 0; j < opponentSize; j ++){
				opponentGenePointer = opponentGenes[j];
				//opponentGenePointer = "22222222222222222222222222222222222222222222222222";
				SetGeneFromString(opponentGenePointer, genePointers[1]);
				FastDuel(genePointers[0], genePointers[1], &rounds, &count1, &count2 );
				cout<<rounds<<"   "<<count1<<"   "<<count2<<endl; 
				scores[i] += calculateScore(rounds, count1, count2);
			}
			mapPopulations[tempGene] = scores[i];
		}
		else{
			scores[i] = mapPopulations[tempGene];
		}
		totalScore += scores[i];
	}

	return totalScore;
}

int main( int argc, const char* argv[] )
{
	int size = 2;
	int scores[size];
	int opponentSize = 8;
	int totalScore = 0; 
	srand (time(NULL));
	char * populationGenes[size];
	char * opponentGenes[opponentSize];
	int i = 0;
	int j = 0;
	cout<<"hello world\n";
	//time t = time();
	//clock_t begin= clock();
	//cout<<begin<<endl;
	for(i = 0; i < size; i++){
		populationGenes[i] = (char *)malloc(sizeof(char*)*GENE_LENGTH);

	}
	for(i = 0; i < opponentSize; i++){
		opponentGenes[i] = (char *)malloc(sizeof(char*)*GENE_LENGTH);
	}
	generateGenes(populationGenes, size, GENE_LENGTH);
	generateGenes(opponentGenes, opponentSize, GENE_LENGTH);	
	for(i = 0; i < size; i++){
		for(j = 0; j < GENE_LENGTH - 1; j ++){
			cout << populationGenes[i][j];
		}
		cout<<endl;
	}
	//SetGeneFromString(genes[i]);
	mutation(populationGenes, 0.1, size, GENE_LENGTH);
	cout<<"after mutation\n";
	for(i = 0; i < size; i++){
		for(j = 0; j < GENE_LENGTH - 1; j ++){
			cout << populationGenes[i][j];
		}
		cout<<endl;
	}
	crossOver(populationGenes, size, GENE_LENGTH);
	cout<<"after cross over\n";
	for(i = 0; i < size; i++){
		for(j = 0; j < GENE_LENGTH - 1; j ++){
			cout << populationGenes[i][j];
		}
		cout<<endl;
	}
	totalScore = evaluation(populationGenes, opponentGenes, size, opponentSize, scores, GENE_LENGTH);
	/*for(i = 0; i < size; i++){
		for(j = 0; j < GENE_LENGTH - 1; j ++){
			cout << populationGenes[i][j];
		}
		cout<<"     :"<<scores[i]<<endl;
	}
	*/
	
	//clock_t end= clock();	
	//cout<<float(end - begin)/CLOCKS_PER_SEC<<endl;
	//cout<<totalScore<<endl;
}
