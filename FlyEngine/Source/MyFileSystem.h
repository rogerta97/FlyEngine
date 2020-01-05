#ifndef _FILE_SYSTEM_H_
#define _FILE_SYETEM_H_

#include "Globals.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std; 

struct MyFileSystem {


private: 
	static MyFileSystem* instance; 
	MyFileSystem();

public: 
	static MyFileSystem* getInstance();
	~MyFileSystem();

	static void Init(); 
	static void Delete(); 

	// Utility -----------
	static string GetLastPathItem(string path, bool keepTermination);
	static void DeleteFileExtension(string& path);
	static FileExtension GetFileExtension(string path);
	static void DeleteLastPathItem(string& path);
	static void DeleteFrontItem(string& path); 
	static bool IsFolder(string& directory);

	// Get Directories ---
	static string GetIconsDirectory();
	static string GetSavedDataDirectory(); 
	static string GetSolutionDirectory();
	static string GetGameDirectory(); 
	static string GetResourcesDirectory(); 
	static void GetRelativeDirectory(string& directory); 

	// File --------------
	static void GetFilesInDirectory(const char* directory, std::vector<string>& list, bool include_path = false);

private: 
	static int GetBarsCount(string countStr); 

private: 
	std::string solutionDirectory = "";
	std::string gameDirectory = "";
	
	std::string iconsDirectory = "";
};



#endif
