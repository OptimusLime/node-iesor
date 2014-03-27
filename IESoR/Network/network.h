#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <JSON/json.h>
#include "math.h"

class Network 
{
	public:

		//bring it in via json object
		Network(Json::Value jsonNetwork);
		Network(std::string sJSONNetwork);

		//activate the networks
		double* activate(double* inputs);

		int InputCount(){return inputCount;}
		int OutputCount(){ return outputCount;}
		double* GetRegisters() { return registers;}

	private:

		void init(Json::Value startNetwork);

		double* registers;
		double* weights;
		int* nodeOrder; 
		int** registerArrays; 
		int** weightArrays;
		int* nodeIncoming;
		int* activationTypes;

		int nodeCount;
		int connectionCount;

		int inputCount; 
		int biasCount;
		int outputCount;

		int activationToInteger(std::string activationType);


		~Network();
};

#endif //NETWORK_H
