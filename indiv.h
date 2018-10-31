#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>

class Indiv{
public:
	//Attributes
	std::string tgtStr; 	//Target string.
	std::string genStr; 	//Generated string.
	std::vector<std::vector<double>> charPosList; //Create a matrix of possibilities.
	std::vector<std::vector<double>> prevCharPosList; //Backup for the previous value in case the mutated version is less fit.
	double fitness; 		//Fitness score for this particular individual. The higher the fitness score is, the smaller this is going to be.
	double prevFitness;		//Backup for the previous fitness value to decide whether to preserve or discard the mutation.
	unsigned int mutatChance; 		//amount of charPos that is allowed to be changed.
	unsigned int prevMutatChance; 	//Backup for mutatChance.
	unsigned int strLength; 		//Allowed length for the generated string.
	unsigned int prevStrLength; 	//Backup for strLength.

	//Methods
	//Constructors
	Indiv();						//Default constructor. Do nothing.
	Indiv(std::string tgtStr);		//Sets the target string to the input string, generates everything according.
	Indiv(std::fstream& saveFile); 	//Reads info from the save file. Save file setup handled by the main program.
	Indiv(Indiv& src1, Indiv& src2);	//Creates a cross between src1 and src2.
	//Mutators
	int mutate();				//mutate: mutates the current individual. Returns status numbers.
	int generate();				//generate: generates a sting to be evaluated. Used only in eval, but didn't want to bother making it private.
	int eval();					//eval: Evaluates and returns the fitness score. Changes fitness value.
	int end_generation(); 		//Decides whether to keep or discard the mutations.

	//Operators
	//void operator = (Indiv& tgt); //Using void here is because there is really no need to have it return anything in the usecase. Also don't want to create new objects when returning.
	bool operator < (Indiv& tgt); //Defines this operator so sort() can be used.
};


