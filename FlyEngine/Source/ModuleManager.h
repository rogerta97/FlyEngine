#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "Module.h"
#include "Globals.h"
#include "Tool.h"

#include <list>
#include <string>

struct ToolSelectableInfo
{
	std::string toolName;
	std::string toolDescription;
	ToolType toolType; 
};

class ModuleManager : public Module
{
public:
	ModuleManager(bool start_enabled = true);
	~ModuleManager();

	bool Init();
	int GetToolsAmount() const; 

	// Desctiption Dictionary -----
	std::list<ToolSelectableInfo> GetToolsNamesDescriptionsList() const; 
	void AddToolsNameDescription(ToolSelectableInfo newInfo); 
	ToolSelectableInfo GetToolNameDescription(std::string toolName) const;
	ToolSelectableInfo GetToolNameDescription(int pos) const;

private: 
	std::list<ToolSelectableInfo> toolNamesDescriptions; 
};

#endif