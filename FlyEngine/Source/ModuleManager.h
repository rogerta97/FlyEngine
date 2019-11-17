#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "Module.h"
#include "Globals.h"
#include "Tool.h"

#include <list>
#include <string>

class ModuleManager : public Module
{
public:
	ModuleManager(bool start_enabled = true);
	~ModuleManager();

	bool Init();
	bool CleanUp(); 

	void LoadEngineIcons();
	int GetToolsAmount() const; 
	ToolSelectableInfo* DrawDictionaryUI(); 

	// Desctiption Dictionary -----
	std::list<ToolSelectableInfo> GetToolsNamesDescriptionsList() const; 
	void AddToolsNameDescription(std::string name, std::string description, ToolType newTooltype);
	ToolSelectableInfo GetToolNameDescription(std::string toolName) const;
	ToolSelectableInfo GetToolNameDescription(int pos) const;

private: 
	std::list<ToolSelectableInfo> toolNamesDescriptions; 
};

#endif