#ifndef _FILE_SYSTEM_H_
#define _FILE_SYETEM_H_

#include <windows.h>
#include <string>
#include <iostream>

using namespace std; 

struct FileSystem {

private: 
	static FileSystem* instance; 
	FileSystem();

public: 
	static FileSystem* getInstance();
	~FileSystem();

	void Delete(); 

public: 
	std::string solutionDirectory; 

};



#endif
