#include "FileSystem.h"

FileSystem* FileSystem::instance = 0; 

FileSystem::FileSystem() {
	solutionDirectory = SOLUTION_DIR; 
}

FileSystem* FileSystem::getInstance()
{
	if (instance == 0)
		instance = new FileSystem();
	
	return instance;
}

FileSystem::~FileSystem() {
	delete instance;
}
