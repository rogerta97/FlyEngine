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
	static void Delete(); 

	// Utility -----------
	static string GetLastPathItem(string path, bool keepTermination);
	static void DeleteFileExtension(string& path);
	static void DeleteLastPathItem(string& path);
	static void DeleteFrontItem(string& path); 

	// Get Directories ---
	static string GetIconsDirectory();
	static string GetSolutionDirectory();
	static string GetGameDirectory(); 
	static void GetRelativeDirectory(string& directory); 

private: 
	static int GetBarsCount(string countStr); 

private: 
	std::string solutionDirectory = "";
	std::string gameDirectory = "";
	
	std::string iconsDirectory = "";
};



#endif
