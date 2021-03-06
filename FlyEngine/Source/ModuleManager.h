#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "Module.h"
#include "Globals.h"
#include "Action.h"
#include "Math/float4.h"

#include <list>
#include <string>

enum DictionaryPopupFilter
{
	FILTER_ACTIONS_INVENTORY_CLICK, 
	FILTER_ACTIONS_UI_BUTTON,
	FILTER_ACTIONS_FIXED,
	FILTER_ACTIONS_CHARACTER,
	FILTER_ACTIONS_NONE, 
};

class FlyObject; 
class Texture; 
class FlyVariable; 
class UI_Element; 
class Texture; 
class FlyObject; 
class DialogueEditorDockPanel; 
class ModuleManager : public Module
{
public:
	ModuleManager(bool start_enabled = true);
	~ModuleManager();

	update_status PostUpdate(float dt); 

	bool Init();
	bool Start();
	void StartGame(const UID& firstRoomUID);

	bool CleanUp(); 

	// Dialogues -------------------
	DialogueEditorDockPanel* GetCurrentDialogueEditor(); 

	// Utils -----------------------
	int GetToolsAmount() const; 
	ActionSelectableInfo* DrawActionDictionaryUI(DictionaryPopupFilter popupFilter = FILTER_ACTIONS_NONE);
	Texture* GetIconFromActionType(ActionType toolType);
	void NotifyVariableChange(FlyVariable* variableCurrentValue); 

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
	
	float4 GetSceneUITint(); 
	void SetSceneUITint(float4 _colorTint);

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
	
	float4 uiBackgroundTint = float4::zero; 
};

#endif