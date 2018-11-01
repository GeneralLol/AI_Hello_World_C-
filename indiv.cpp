#include <cstdlib>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include "indiv.h"

using namespace std;

//Default Constructor.
//Sets everything to placeholder values.
//Vector remains empty.
Indiv::Indiv(){
	tgtStr = "";
	genStr = "";
	fitness= 2147483647;	//Fitness set to a trivially large value to ensure it does not fit.
	prevFitness = fitness;
	mutatChance = 0;
	prevMutatChance = mutatChance;
	strLength   = 0;
	prevStrLength = strLength;
}

//Takes in the target string and generates the charPosList according.
Indiv::Indiv(string tgtStr){
	this->tgtStr = tgtStr;
	fitness = 2147483647;
	prevFitness = fitness;
	mutatChance = rand() % 10;
	prevMutatChance = mutatChance;
	strLength   = rand() % 100;
	prevStrLength = strLength;
	//Generate the charPos matrix.
	for (unsigned int i = 0; i < strLength; i ++){
		vector<double> tempVector(96);	//temporary vector that is used to generate all the data.
		double sum = 0;
		for (int j = 0; j < 96; j ++){
			tempVector[j] = rand() % 100;
			sum += tempVector[j];
		}
		//Make sure the possibility list add up to 100.
		double multiplier = 100 / sum;
		for (int j = 0; j < 96; j ++){
			tempVector[j] *= multiplier;
		}
		//Pop the generated vector into the charPosList vector.
		charPosList.push_back(tempVector);
	}
	prevCharPosList = charPosList;
}

//Takes in the save file opened by the parent program.
Indiv::Indiv(fstream& saveFile){
	/**
	 * Save file should be in the following format:
	 * 1st line: tgtStr
	 * 2nd line: strLength
	 * 3rd line: mutatChance
	 * 4th line after: charPosList
	 */
	tgtStr = "";
	strLength   = 0;
	mutatChance = 0;
	saveFile >> tgtStr;
	saveFile >> strLength;
	prevStrLength = strLength;
	saveFile >> mutatChance;
	prevMutatChance = mutatChance;

	for (unsigned int i = 0; i < strLength; i ++){
		vector<double> tempVector(96);
		for (int j = 0; j < 96; j ++){
			saveFile >> tempVector[j];
		}
		charPosList.push_back(tempVector);
		tempVector.clear();
	}

	fitness = 2147483647;
	prevFitness = fitness;

}

//Crosses info from the two source individuals.
Indiv::Indiv(Indiv& src1, Indiv& src2){
	this->tgtStr = src1.tgtStr;

	//Set the mutatChance and strLength to from a random parent.
	bool paternal = rand() % 2;
	if (paternal){
		this->mutatChance = src1.mutatChance;
		this->strLength   = src1.strLength;
	}else{
		this->mutatChance = src2.mutatChance;
		this->strLength   = src2.strLength;
	}
	prevMutatChance = mutatChance;
	prevStrLength   = strLength;
	fitness = 2147483647;
	prevFitness = fitness;

	//Set the charPosList.
	//Make the charPosList according to strLength.
	for (unsigned int i = 0; i < strLength; i ++){
		vector<double> tempVector(96);
		for (int j = 0; j < 96; j ++){
			tempVector[j] = 0;
		}
		charPosList.push_back(tempVector);
		tempVector.clear();
	}
	//TODO: Fix the situation where the two charPosLists may not have the same length.
	for (unsigned int i = 0; i < strLength; i ++){
		for (int j = 0; j < 96; j ++){
			paternal = rand() % 2;
			//If there are no values at [i][j] in one of the sources, set the value to be from the other one.
			if ((src1.strLength-1) < i){
				paternal = 0;
			}else if ((src2.strLength-1) < i){
				paternal = 1;
			}

			if (paternal){
				charPosList[i][j] = src1.charPosList[i][j];
			}else{
				charPosList[i][j] = src2.charPosList[i][j];
			}
		}
	}
}

//Mutator.
//Changes mutatChance, strLength, adds/removes terms form charPosList according,
//and alters some amount of values according to mutatChance.
int Indiv::mutate(){
	//Change the strLength and mutatChance.
	int upperBound = mutatChance + 5;
	int lowerBound = mutatChance - 5;
	if (lowerBound < 0) { lowerBound = 0; }
	mutatChance = rand() % (upperBound - lowerBound) + lowerBound;

	upperBound = strLength + 5;
	lowerBound = strLength - 5;
	if (lowerBound < 0) { lowerBound = 0; }
	strLength = rand() % (upperBound - lowerBound) + lowerBound;

	//Add/remove elements from the charPosList.
	if (strLength < charPosList.size()){
		//If strLength is shorter than the list stored, delete items from the back of the list.
		for (unsigned int i = charPosList.size()-1; i > strLength; i --){
			charPosList.erase(charPosList.begin() + i - 1);
		}
	}else if (strLength > charPosList.size()){
		//If strLength is larger than the list stored, add more randomly generated terms.
		for (unsigned int i = charPosList.size(); i < strLength; i ++){
			vector<double> tempVector(96);
			int sum = 0;
			for (int j = 0; j < 96; j ++){
				tempVector[j] = rand() % 100;
				sum += tempVector[j];
			}

			double multiplier = 100 / sum;
			for (int j = 0; j < 96; j ++){
				tempVector[j] *= multiplier;
			}

			charPosList.push_back(tempVector);
		}
	}

	//randomly choose mutatChance possibilities to change.
	for (unsigned int i = 0; i < mutatChance; i ++){
		int x = rand() % charPosList.size();
		int y = rand() % 96;

		int upperBound = charPosList[x][y] + 5;
		int lowerBound = charPosList[x][y] - 5;
		if (lowerBound < 0) { lowerBound = 0; }
		charPosList[x][y] = rand() % (upperBound - lowerBound) + lowerBound;
	}

	//Go through the matrix to ensure all of the sublists add up to 100.
	for (unsigned int i = 0; i < strLength; i ++){
		double sum = 0;
		for (int j = 0; j < 96; j ++){
			sum += charPosList[i][j];
		}
		double multiplier = 100 / sum;
		for (int j = 0; j < 96; j ++){
			charPosList[i][j] *= multiplier;
		}
	}
	return 1;
}

//Generates the string according to the charPosList.
int Indiv::generate(){
	string tempStr = "";
	char   tempChar= ' ';
	vector<double> steps(96);
	double randDouble = 0;
	//Generate the step list.
	for (unsigned int i = 0; i < charPosList.size(); i ++){
		//Generate the step list.
		steps = charPosList[i];
		for (int j = 1; j < 96; j ++){
			steps[j] += steps[j-1];
		}
		//Create a random number and see which letter does the number fall on.
		randDouble = (rand() % 1000000) / 1000; //Generates a double between 0 to 100.
		for (int j = 0; j < 96; j ++){
			if (randDouble > steps[j]){
				//If the random double is larger than the current step,
				//Add a character that correlates to the current step to tempStr.
				tempChar = j + 32;
				tempStr  = tempStr + tempChar;
				break;
			}
		}
	}
	//Move tempStr to genStr.
	genStr = tempStr;
	return 1;
}

//Evaluates the fitness score of the individual according to the string it generated.
int Indiv::eval(){
	//Generate a string first, then evaluate.
	this->generate();
	unsigned int lenTgtStr = tgtStr.size();
	unsigned int lenGenStr = genStr.size();
	unsigned int cycleLen  = min(lenTgtStr, lenGenStr);
	int tempFitness = 0;

	//Go through the overlapping portions of the strings.
	for (unsigned int i = 0; i < cycleLen; i ++){
		tempFitness += pow((tgtStr[i] - genStr[i]), 2);
	}
	//Go through the non-overlapping portions.
	if (cycleLen == lenTgtStr){
		for (unsigned int i = cycleLen; i < lenGenStr; i ++){
			tempFitness += genStr[i] ^ 2;
		}
	}else if (cycleLen == lenGenStr){
		for (unsigned int i = cycleLen; i < lenTgtStr; i ++){
			tempFitness += tgtStr[i] ^ 2;
		}
	}
	fitness = tempFitness;
	return fitness;
}

//Decides whether to keep or discard the mutations.
int Indiv::end_generation(){
	if (fitness < prevFitness){
		mutatChance = prevMutatChance;
		strLength = prevStrLength;
		charPosList = prevCharPosList;
	}else{
		prevMutatChance = mutatChance;
		prevStrLength = strLength;
		prevCharPosList = charPosList;
	}
	return 1;
}

//Operators
//<: Used for sort() function.
bool Indiv::operator < (Indiv& tgt){
	return this->fitness < tgt.fitness;
}
