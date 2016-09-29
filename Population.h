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

#include <unistd.h>

using namespace std;

class Population{
	private:
		vector<Chromosome> pool;	// VAR : Define population pool
		static const int pop_size;       		// VAR : Define population size
		float prev_avg = 0;
		static float FITNESS_HALT ;
		static float HARD_LIMIT;
		
	public:
		
		float curMin;
		float curTotal;
		bool exitFlag = false;

		// CONSTRUCTOR : Population
		Population(int chromosome_length){

			// VAR : Iterator to insert chromosomes
			vector<Chromosome>::iterator it = pool.begin();
			
			// SUB  : Loop to randomly fill pool with chromsomes
			for (int i = 0; i<pop_size; i++){
				it = pool.insert(it, Chromosome(chromosome_length));
			}
		}
		
		// CONSTRUCTOR : Default
		Population(){
			
		}

		// OUTPUT OVERLOAD : Convenience
		friend ostream &operator<<(ostream& os, Population& obj)
		{
			obj.evalAll(true);

			for (int i=0; i<obj.pop_size; i++){
	    		cout <<  setw(10) << " Cr " << setw(3) << i+1 << " : " << obj.pool[i] <<  " = "
				     << setw(9) << fixed << setprecision(6)<< (obj.pool[i]).decode() << "; f = "
				     << setw(6) << fixed << setprecision(3)<< (obj.pool[i]).getEval()+(obj.curMin<0?obj.curMin:0) << "; cumulProb = "
				     << setw(5) << fixed << setprecision(3)<< (obj.pool[i]).getCumulProb() << endl;
	    	}

	    	return os;
		}
		
		// SUB : Get best performer
		void getBest(){
			int bestChromosome = -1;
			float bestEval = -999;
			
			for (int i = 0; i < pop_size; i++){
				float curChromoEval = pool[i].getEval();
				
				if (curChromoEval > bestEval){
					bestEval = curChromoEval;
					bestChromosome = i;
				}
			}
			
			cout << "\t Best Performer : \n \t\t Chromosome[" << setw(2) << bestChromosome << "]:" << pool[bestChromosome]
			<< endl << "\t\t Real var val  : " << pool[bestChromosome].decode()
			<< endl << "\t\t Fitness value : " << bestEval +(curMin<0?curMin:0)
			<< endl;
			
			cin.get();
		}
		
		
		// SUB : Get least fit eval to make all eval functions positive
		void evalAll(bool printLog){
			curMin = 100;
			curTotal = 0;
			
			for (int i=0; i<pop_size; i++){
				float eval = pool[i].eval();
				if (eval < curMin) curMin = eval;
			}
			
			if (curMin < 0){
				for (int i=0; i<pop_size; i++){
					pool[i].adjustEval(-curMin);
				}
			}
			
			for (int i=0; i<pop_size; i++){
				float eval = pool[i].getEval();
				curTotal += eval;
			}

			float runningCumulative = 0;

			for (int i=0; i<pop_size; i++){
				float indProb = pool[i].getEval()/curTotal;

				runningCumulative += indProb;

				pool[i].setCumulProb(runningCumulative);
			}
			
			if (printLog){
			
				cout << " Avg fitness : "<< setprecision(6)  << setw(10) << fixed << (curTotal/pop_size)+(curMin<0?curMin:0)
				<< "  Min : " << setw(10) << curMin << endl;

				if ((abs((curTotal/pop_size)+(curMin<0?curMin:0) - prev_avg) < FITNESS_HALT)){
					cout << "\t * ================= *\n"
					     << "\t |  Plateau reached  |\n"
						 << "\t * ================= *\n";
					
				//	if (((curTotal/pop_size)+(curMin<0?curMin:0)) > HARD_LIMIT){
						//cout << " - Near max " << endl;
						getBest();
						for (int k=0; k<pop_size; k++){
		    			cout <<  setw(10) << " Cr " << setw(3) << k+1 << " : " << pool[k] <<  " = "
					     	<< setw(9) << fixed << setprecision(6)<< (pool[k]).decode() << "; f = "
					     	<< setw(6) << fixed << setprecision(3)<< (pool[k]).getEval()+(curMin<0?curMin:0) << endl;
		    			}
		    			exit(1);
				//	}
					cout << endl;
				}
			}
		}
		
		// SUB : Create next generation
		void evolve(){
			evalAll(true);
			vector<Chromosome> nextpool;
			
			vector<Chromosome>::iterator it = nextpool.begin();
			
			for (int i = 0; i < pop_size; i++){
				float selected = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				for (int j = 0; j < pop_size; j++){
					if (pool[j].getCumulProb() < selected) continue;
	 				it = nextpool.insert(it, Chromosome(pool[j]));
					break;
				}
			}

			//Start crossover segment
			vector<Chromosome> parentPool;
			
			vector<Chromosome>::iterator parent_it = parentPool.begin();
			int parentCount = 0;
			
			int limit = pop_size;
			
			for (int j = 0; j < limit; j++){
				float selected = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				if (selected < Chromosome::PROB_XOV){
					parent_it = parentPool.insert(parent_it, Chromosome(nextpool[j]));
					parentCount++;
					
					nextpool.erase(nextpool.begin() + j);
					
					j--;
					limit--;
					
				}
			}

			/* DEBUG : Display parent pool
			cout << " Parent : \n";
			
			for (int i=0; i<parentCount; i++){
	    		cout << setw(2) << i+1 << " : " << parentPool[i] <<  " = "
				     << setw(9) << fixed << setprecision(6)<< (parentPool[i]).decode() << endl;
			}
			*/
			
			while (parentCount>1){
				int rand1 = rand()%parentCount;
				
				Chromosome parent1(parentPool[rand1]);
				
				parentPool.erase(parentPool.begin()+rand1);
				
				parentCount--;
				
				int rand2 = rand()%parentCount;

				Chromosome parent2(parentPool[rand2]);
				
				parentPool.erase(parentPool.begin()+rand2);
				
				parentCount--;

				/* DEBUG : Test Crossover
					cout << "  Pre - Parent 1 : " << parent1 <<  " -> " << parent1.decode() << endl;
					cout << "  Pre - Parent 2 : " << parent2 <<  " -> " << parent2.decode() << endl;
				*/
				
				parent1.cross(parent2);

				/* DEBUG : Test Crossover
					cout << "  Pre - Parent 1 : " << parent1 <<  " -> " << parent1.decode() << endl;
					cout << "  Pre - Parent 2 : " << parent2 <<  " -> " << parent2.decode() << endl;
				*/
				
				nextpool.insert(nextpool.end(), Chromosome(parent1));
				nextpool.insert(nextpool.end(), Chromosome(parent2));
			}
			
			if (parentCount == 1){
				nextpool.insert(nextpool.end(),Chromosome(parentPool[0]));
				parentPool.erase(parentPool.begin());
			}
			
			for (int i = 0; i < pop_size; i++){
				nextpool[i].mutate();
			}
			
			prev_avg = (curTotal/pop_size)+(curMin<0?curMin:0);
			
			pool = nextpool;
		}
};
