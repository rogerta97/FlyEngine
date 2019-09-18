#ifndef  _RANDOM_NUMBER_GENERATOR_H_
#define _RANDOM_NUMBER_GENERATOR_H_

#include "Globals.h"

class RandomNumberGenerator {

private:
	static RandomNumberGenerator* instance;
	RandomNumberGenerator() {}; 

public:
	static RandomNumberGenerator* getInstance();
	~RandomNumberGenerator() {}; 
	void Delete(); 

	static int GetPCGRandomNumberInt(int miValue, int maValue);
	static float GetPCGRandomNumberFloat(float maxValue, float minValue); 
	static UID GenerateUID(); 
};

#endif // ! _RANDOM_NUMBER_GENERATOR_H_

