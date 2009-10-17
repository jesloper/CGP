#include "MathFunctions.h"
#include <stdlib.h>
#include "mersenne.cpp"
#include <QtDebug>
#include <ctime>
TRandomMersenne rb((unsigned int)time(0)); 

/**
  * \brief creates a random number
  *
  * \param min minimum number
  * \param max maximum number
  * \return integer between (and including) min and max
*/

int getRandInt(int min, int max){
 return	rb.IRandom(min,max);
}

int max(int f, int s){
	if(f > s)
		return f;
	return s;
}


int findUnique(double *array, int size){
	int unique = size;
	std::map<double,double> unq;
	std::map<double,double>::iterator it;
	for(int i = 0; i < size;i++){
		it = unq.find(array[i]);
		if(it != unq.end()){
			unique--;
		}else{
			unq[array[i]] = 1;
		}
	}
	return unique;
}
double findMax(double* array, int size) {
	double max = 0;
	for (int i = 0; i <= size; i++) {
		if (array[i] > max)
			max = array[i];
	}
	return max;
}
double findMin(double* array, int size) {
	double min = 0;
	for (int i = 0; i <= size; i++) {
		if (array[i] < min)
			min = array[i];
	}
	return min;
}
