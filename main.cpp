#include <cstdlib>
#include <iostream>
#include <string>
#include "indiv.h"

using namespace std;

int main(){
	string tgtStr = "Hello World!";
	Indiv indiv(tgtStr);

	int generation = 0;

	while (1){
		generation ++;
		indiv.end_generation();
		cout << generation << "\t" << indiv.genStr << "\t" << indiv.fitness << "\n";
		if (indiv.fitness == 0){
			cout << "End Simutation!" << "\n";
			break;
		}
	}
}
