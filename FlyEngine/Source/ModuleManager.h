#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "Module.h"
#include "Globals.h"
#include "Tool.h"

#include <list>
#include <string>

class FlyObject; 
class Texture; 
class ModuleManager : public Module
{
public:
	ModuleManager(bool start_enabled = true);
	~ModuleManager();

	bool Init();

	bool CleanUp(); 

	// Utils -----------------------
	void LoadEngineIcons();
	int GetToolsAmount() const; 
	ToolSelectableInfo* DrawToolDictionaryUI(); 
	Texture* GetToolTypeIcon(ToolType toolType); 

	// Easy Acces -------
	FlyObject* GetSelectedFlyObject(); 
	void SetSelectedFlyObject(FlyObject* newSelectedObject); 

	// Desctiption Dictionary -----
	std::list<ToolSelectableInfo> GetToolsNamesDescriptionsList() const; 
	void AddToolsNameDescription(std::string name, std::string description, ToolType newTooltype);
	ToolSelectableInfo GetToolNameDescription(std::string toolName) const;
	ToolSelectableInfo GetToolNameDescription(int pos) const;

private: 
	std::list<ToolSelectableInfo> toolNamesDescriptions; 
};

#endif