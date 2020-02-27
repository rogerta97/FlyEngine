#ifndef _OBJECT_PROPERTIES_DOCK_PANEL_H_
#define _OBJECT_PROPERTIES_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ModuleManager.h"
#include "MathGeoLib/Math/float2.h"

class FlyObject;
class UI_Button; 
class ObjectPropertiesDockPanel : public DockPanel {

public:
	ObjectPropertiesDockPanel(bool isVisible);
	~ObjectPropertiesDockPanel();

	bool Draw();

	// User Inteface
	void DrawUIElementProperties();
	void DrawUITextProperties(UI_Element* selectedUIElement);
	void DrawUIImageProperties(UI_Element* selectedUIElement);
	void DrawUIButtonProperties(UI_Element* selectedUIElement);

	// UI_Button UI
	void DrawOnClickActionButtonList(UI_Button* selectedButton);
	void DrawSwapImageSection(UI_Button* selectedButton);
	void DrawSearchBarButtonActions(UI_Button* selectedButton);
	void DrawColorTintSection();
	void DrawButtonMainImagePreview(UI_Button* selectedButton);

	// Action Objects
	void DrawFlyObjectProperties();
	void DrawObjectTabs();
	void DrawInventoryItemTabs(FlyObject* selectedObject);
	void DrawFixedPartObjectUI(FlyObject* selectedObject);
	void DrawTransformButtons();

	void SetSelectedObject(FlyObject* newSelectedObject);
	FlyObject* GetSelectedObject() const;

	// Clickable Area Tab
	void DrawClickableAreaTab(); 

	// Object Tools Tab
	void DrawObjectActionsTab();
	void DrawActionSettings();

	void DrawActionsList();
	void DrawAddAndDeleteButtons();
	bool DrawActionSelectable(ActionSelectableInfo& selectableInfo, Action*& currentTool, int posInList, int selectableHeigth);
	void DrawObjectPlacementCH(FlyObject* selectedObject);
	
	void DrawChangeRoomSettings();
	void DrawToolImageSettings();
	void DrawModifyVariableSettings();
	void DrawDisplayTextSettings();
	void DrawEmitSoundSettings();
	void DrawDisplayAnimationSettings();

private: 
	bool scrollToEnd = false; 
	char searchNewToolBuffer[256]; 
	FlyObject* selectedObject = nullptr; 
	bool showToolDictionary; 

	// Emit Sound 
	bool showSoundSelectionPopup = false; 

	// Conditions 
	bool showVariableConditions = false; 

	// Clickable Area 
	float2 clickableAreaPosOne; 
	float2 clickableAreaSizeOne; 
};

#endif 
