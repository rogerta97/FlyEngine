#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "Module.h"
#include "Globals.h"
#include "Action.h"

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
	ActionSelectableInfo* DrawActionDictionaryUI(); 
	Texture* GetIconFromActionType(ActionType toolType); 

	// Easy Acces -------
	FlyObject* GetSelectedFlyObject(); 
	void SetSelectedFlyObject(FlyObject* newSelectedObject); 

	// Desctiption Dictionary -----
	std::list<ActionSelectableInfo> GetToolsNamesDescriptionsList() const; 
	void AddToolsNameDescription(std::string name, std::string description, ActionType newTooltype);
	ActionSelectableInfo GetToolNameDescription(std::string toolName) const;
	ActionSelectableInfo GetToolNameDescription(int pos) const;

private: 
	std::list<ActionSelectableInfo> toolNamesDescriptions; 
};

#endif