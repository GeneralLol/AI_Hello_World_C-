#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "indiv.h"

using namespace std;

int main(){
	srand(0);
	int populationSize = 10;
	string tgtStr = "Hello World!";
	//create a vector of individuals.
	vector<Indiv> indivList;
	//Generate the list.
	for (int i = 0; i < populationSize; i ++){
		Indiv tempIndiv(tgtStr);
		indivList.push_back(tempIndiv);
	}

	//Check, evaluate and repopulate the list.
	while (1){
		for (int i = 0; i < populationSize; i ++){
			indivList[i].mutate();
			indivList[i].eval();
			indivList[i].end_generation();
		}
		//Sort the list according to fitness.
		sort(indivList.begin(), indivList.end());
		cout << indivList[0].genStr << "\t" << indivList[0].fitness<< "\n";

		//Erase half of the population
		int populationErased = populationSize / 2;
		indivList.erase(
						indivList.begin() + populationErased,
						indivList.end()
						);
		//Repopulate the list
		for (int i = 0; i < populationErased; i ++){
			Indiv tempIndivPtr(indivList[0], indivList[i]);
			indivList.push_back(tempIndivPtr);
		}
	}
}
