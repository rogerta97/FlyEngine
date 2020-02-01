#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "Module.h"
#include "Globals.h"
#include "Action.h"

#include <list>
#include <string>

enum DictionaryPopupFilter
{
	FILTER_ACTIONS_INVENTORY_CLICK, 
	FILTER_ACTIONS_UI_BUTTON,
	FILTER_ACTIONS_NONE, 
};

class FlyObject; 
class Texture; 
class FlyVariable; 
class UI_Element; 
class Texture; 
class FlyObject; 
class ModuleManager : public Module
{
public:
	ModuleManager(bool start_enabled = true);
	~ModuleManager();

	update_status PostUpdate(float dt); 

	bool Init();
	bool Start(); 
	bool CleanUp(); 

	// Utils -----------------------
	int GetToolsAmount() const; 
	ActionSelectableInfo* DrawActionDictionaryUI(DictionaryPopupFilter popupFilter = FILTER_ACTIONS_NONE);
	Texture* GetIconFromActionType(ActionType toolType); 

	// Delete Obejcts --------------
	void AddDeleteFromListObject(FlyObject* newObjectToDelete);
	void DeleteObjectsFromListNow();

	void AddDeleteObject(FlyObject* newObjectToDelete);
	void DeleteObjectsNow();

	// Widgets ---------------------
	void DrawActionListWithSettings(FlyObject* ownerObejct); 
	void DrawImageFitInCenter(Texture* textureToShow);

	// Easy Acces ------------------
	FlyObject* GetSelectedFlyObject(); 
	void SetSelectedFlyObject(FlyObject* newSelectedObject); 

	UI_Element* GetSelectedUIElement();
	void SetSelectedUIElement(UI_Element* newSelectedObject);

	// Desctiption Dictionary -----
	std::list<ActionSelectableInfo> GetToolsNamesDescriptionsList() const; 
	void AddToolsNameDescription(std::string name, std::string description, ActionType newTooltype);
	ActionSelectableInfo GetToolNameDescription(std::string toolName) const;
	ActionSelectableInfo GetToolNameDescription(int pos) const;

	FlyVariable* fakeVarInitAttach = nullptr;

private: 
	std::list<ActionSelectableInfo> toolNamesDescriptions; 

	std::list<FlyObject*> deleteObjectsFromListTick; 
	std::list<FlyObject*> deleteObjectsTick; 
};

#endif