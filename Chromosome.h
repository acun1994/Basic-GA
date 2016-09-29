#ifndef _STANDARD
#define _STANDARD
	#include <iostream>
	#include <iomanip>
	#include <cmath>
	#include <ctime>
#endif

using namespace std;

const float M_PI = atan(1)*4;

class Chromosome{
	private:
		static float PROB_MUT;
		
		short int* gene;
		int length;
		float fitness = 0;
		float cumulProb = 0;
		
	public:
		static float PROB_XOV;
		
		// CONSTRUCTOR : Chromosome
		Chromosome(int geneLength){
			length = geneLength;
			gene = new short int[geneLength];

			for (int i=0; i<geneLength; i++){
				gene[i] = rand()%2;
			}
		};
		
		// CONSTRUCTOR : Copy Chromosome
		Chromosome(const Chromosome& origin){
			length = origin.length;
			gene = new short int[length];
			
			for (int i=0; i< length; i++){
				this->gene[i] = origin.gene[i];
			}
		}

		// OUTPUT OVERLOAD : Convenience
		friend ostream &operator<<(ostream& os, const Chromosome& obj)
		{
	    	for (int i=0; i<obj.length; i++){
	    		cout << " " << obj.gene[i];
	    	}
	    	return os;
		}

		// SUB : Decode chromosome into real value
		// Adjust code here for problem-specific
		// Current code is for -1<= x <= 2, with precision to 6 dp
		// Chromosome length = 22
		float decode(){
			float leftBound = -1.0;
			float domainLength = 3;

			float decoded = 0;
			for (int i = length-1; i>=0; i--){
				decoded += pow(2, length-1-i) * gene[i];
			}

			decoded = leftBound + decoded * domainLength / (pow(2,length) -1);

			return decoded;
		}

		// SUB : Fitness function a.k.a. evaluation
		// Adjust code here for problem-specific
		float eval(){
			float evaluated = 0;
			float x = decode();

			evaluated = x * sin(10*M_PI*x) + 1.0;

			fitness = evaluated;

			return evaluated;
		}
		
		// SUB : Adjust fitness to be non-negative if necessary
		void adjustEval(float adjustAmount){
			fitness += adjustAmount;
		}
		
		// SUB : Retrieve fitness value for chromosome
		float getEval(){
			return fitness;
		}
		
		// SUB : Set cumulative probability for chromosome
		void setCumulProb(float cumulProb){
			this->cumulProb = cumulProb;
		}
		
		// SUB : Retrieve probability value for chromosome
		float getCumulProb(){
			return cumulProb;
		}

		// SUB : Crossover Operator
		void cross(Chromosome& that){
			//Select random crossover point
			int crosspoint = rand()%length;

			//Swap genes past corssover point
			for (int i = crosspoint-1; i<length; i++){
				short int temp = this->gene[i];
				this->gene[i] = that.gene[i];
				that.gene[i] = temp;
			}
		}

		// SUB : Mutate Operator
		void mutate(){
			float mut_chance = 0;
			for (int i = 0; i<length; i++){
				// Randomly mutate gene if mut_chance <= Global PROB_MUT
				srand(rand());
				mut_chance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				if (mut_chance <= Chromosome::PROB_MUT){
					gene[i] = rand()%2;
				}
			}
		}
};


