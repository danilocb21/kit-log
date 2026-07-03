#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <memory>

class Data {
public:
	Data(int, char *);

	void read();
	void printCapacity();
	void printWeight();
	inline int getDimension() { return dimension; }
	inline double getCapacity() { return capacity; }
	inline double getWeight(int i) { return weight[i]; }
	
	std::string getInstanceName(); //Get instance's name

private:
	std::string instaceName;
	int nbOfPar;

	int dimension;
	double capacity;

	std::shared_ptr<double[]> weight;
};

#endif
