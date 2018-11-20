#include <cstdlib>
#include <string>

class Indiv{
public:
	std::string genStr;
	std::string prevGenStr;
	std::string tarStr;

	const static int INT_MAX = 2147483647;
	const static int PRINTABLE_CHAR = 96;
	const static int SPACE = 32;

	int fitness;
	int prevFitness;
	int mutChance;
	int lenStr;

	Indiv();
	Indiv(std::string);

	void mutate();
	void evaluate();
	void end_generation();
};
/**
 * Crosses two individuals and puts the result in the resultant individual.
 * @param indiv1 One of the source individuals
 * @param indiv2 One of the source individuals
 * @param rst The resultant individual from the cross that is to be overwritten.
 */
void cross(Indiv& indiv1, Indiv& indiv2, Indiv& rst);
