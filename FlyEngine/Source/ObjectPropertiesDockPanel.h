#ifndef _OBJECT_PROPERTIES_DOCK_PANEL_H_
#define _OBJECT_PROPERTIES_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ModuleManager.h"
#include "MathGeoLib/Math/float2.h"

class FlyObject;
class ObjectPropertiesDockPanel : public DockPanel {

public:
	ObjectPropertiesDockPanel(bool isVisible);
	~ObjectPropertiesDockPanel();

	bool Draw();

	void DrawUIElementProperties();

	void DrawUIImageProperties(UI_Element* selectedUIElement);

	void DrawFlyObjectProperties();

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
	void DrawActionSelectable(ActionSelectableInfo& selectableInfo, Action*& currentTool, int posInList, int selectableHeigth);
	void DrawObjectPlacementCH(FlyObject* selectedObject);
	
	void DrawChangeRoomSettings();
	void DrawToolImageSettings();
	void DrawModifyVariableSettings();
	void DrawEmitSoundSettings();

private: 
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
