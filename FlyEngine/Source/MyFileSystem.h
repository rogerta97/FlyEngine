#ifndef _FILE_SYSTEM_H_
#define _FILE_SYETEM_H_

#include <windows.h>
#include <string>
#include <iostream>

using namespace std; 

struct MyFileSystem {


private: 
	static MyFileSystem* instance; 
	MyFileSystem();

public: 
	static MyFileSystem* getInstance();
	~MyFileSystem();
	void Delete(); 

	// Utility -----------
	string GetLastPathItem(string path, bool keepLastPathItem);
	void DeleteFileExtension(string& path); 

	string GetIconsDirectory();
	string GetSolutionDirectory();

private: 
	std::string solutionDirectory = ""; 
	std::string iconsDirectory = ""; 
};



#endif
