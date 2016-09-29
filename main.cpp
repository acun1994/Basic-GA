#ifndef _STANDARD
#define _STANDARD
	#include <iostream>
	#include <iomanip>
	#include <cmath>
	#include <ctime>
#endif

#ifndef _CHROMOSOME
#define _CHROMOSOME
	#include "Chromosome.h"
#endif

#ifndef _VECTOR
#define _VECTOR
	#include <vector>
#endif

#include "Population.h"

using namespace std;

// Current problem = max(x.sin(10pi.x) + 1.0)
// Max should be around x=1.85
// Required chromosome length = 2^22

// VAR : Init probability of mutation
float Chromosome::PROB_MUT = 0.05;
float Chromosome::PROB_XOV = 0.25;

float Population::FITNESS_HALT = 0.0001;
float Population::HARD_LIMIT = 2.7;         //Currently unused
const int Population::pop_size = 100;

int main(){
	srand(time(0));
	Population curPool(22);
	const int MAX_ITER = 1000;
	int iter = 1;
	
	cout << " First generation : " << endl << curPool;
	
	cin.get();
	
	while(iter<= MAX_ITER){
		cout <<setw(7)<< iter++ << " : ";
		curPool.evolve();
	}
	
	cout << " Max iter reached \n";
	curPool.getBest();
	cout << curPool;

	/* Test Code for Operators
		cout << "Hello World\n";

		Chromosome test(32), test2(32);

		cout << "\n Before : \n" << test << endl << test2;

		test.cross(test2);

		cout << "\n After  : \n" << test << endl << test2;

		test2.mutate();

		cout << "\n Mutate (2): \n" << test2;
	*/

	return 0;
}
