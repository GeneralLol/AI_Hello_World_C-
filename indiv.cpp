#include <cstdlib>
#include <string>
#include <random>
#include <cmath>
#include "indiv.h"

using namespace std;

//Default constructor.
Indiv::Indiv(){
	srand(time(0));

	genStr = " ";
	prevGenStr = genStr;
	tarStr = " ";

	fitness   = INT_MAX;
	prevFitness = fitness;
	mutChance = 1;
	lenStr = 1;
}

Indiv::Indiv(string tarStr){
	this->tarStr = tarStr;
	genStr = " ";
	prevGenStr = genStr;

	fitness = INT_MAX;
	prevFitness = fitness;

	mutChance = 1;
	lenStr = 1;
}

void Indiv::mutate(){
	//Change lenStr
	int lowerBound = lenStr - 5;
	if (lowerBound < 1){
		lowerBound = 1;
	}
	lenStr = rand() % 10 + lowerBound;

	//Change genStr accordingly
	char tempChar = 32;
	int strLength = genStr.length();
	while (strLength < lenStr){
		tempChar = rand() % PRINTABLE_CHAR + SPACE;
		genStr = genStr + tempChar;
		strLength = genStr.length();
	}

	while (strLength > lenStr){
		genStr.erase(strLength - 1);
		strLength = genStr.length();
	}

	//Change some of the characters in the string
	for (int i = 0; i < mutChance; i ++){
		int index = rand() % strLength;
		tempChar = rand() % PRINTABLE_CHAR + SPACE;
		genStr[index] = tempChar;
	}

	//Change mutChance
	lowerBound = mutChance - 5;
	if (lowerBound < 1){
		lowerBound = 1;
	}
	mutChance = rand() % 10 + lowerBound;
}

void Indiv::evaluate(){
	fitness = 0;
	int index = 0;
	int lenTarStr = tarStr.length();
	//Overlapping area
	while ((index < lenStr) && (index < lenTarStr)){
		int difference = genStr[index] - tarStr[index];
		fitness += pow(difference, 2);
		index ++;
	}

	//Non-overlapping Area
	while (index < lenStr){
		int difference = genStr[index];
		fitness += pow(difference, 2);
		index ++;
	}
	while (index < lenTarStr){
		int difference = tarStr[index];
		fitness += pow(difference, 2);
		index ++;
	}
}

void Indiv::end_generation(){
	this->mutate();
	this->evaluate();
	//If fitness < prevFitness, discard mutations.
	//Otherwise, keep mutations.
	if (fitness > prevFitness){
		genStr  = prevGenStr;
		fitness = prevFitness;
	}else{
		prevGenStr = genStr;
		prevFitness= fitness;
	}
}
