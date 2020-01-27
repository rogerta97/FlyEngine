#ifndef _OBJECT_CREATOR_DOCK_PANEL_H_
#define _OBJECT_CREATOR_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ModuleManager.h"
#include "MathGeoLib/Math/float2.h"
#include "ImGui/imgui.h"

#include <string>

class FlyObject;
class Action; 
class Texture; 
class DisplayImageAction; 
class ObjectCreatorDockPanel : public DockPanel {

public:
	ObjectCreatorDockPanel(bool isVisible);
	~ObjectCreatorDockPanel();

	bool Draw();
	void DrawInventorySettings();
	void DrawPropertiesTab();
	bool CleanUp(); 

	void ResetObjectData();
	void Close(); 

	// Clickable Area -------------
	void DrawClickableAreaCreator();
	void DrawClickableAreaSettings();
	void PrintClickableAreaObjectVisuals(bool drawClickableArea = true);
	void DrawPrevTextureCA(bool drawClickableArea = true);
	void GetTextureSizeFitted(ImVec2& prevTextureSize);
	void DrawPreviewClickableAreaOnTexture(float2 imageTopLeft, float2 prevTextureSize);

	// Object Creator -------------
	void DrawObjectCreator(); 
	void DrawObjectActionsList(); 
	void DrawSelectable(ActionSelectableInfo selectableInfo, bool& isSelected, int posInList, int selectableHeight, Action* currentTool);
	void DrawAddAndDeleteActionButtons(); 
	
	// Draw Prev Object Settings --
	void DrawSelectedActionSettings();
	void DrawDisplayImageSettings(); 
	void DrawChangeRoomActionSettings();
	void DrawModifyVariableActionSettings();
	void DrawEmitSoundActionSettings();

	void OnAddActionButtonClicked(); 
	bool DrawCloseAndCreateButton();
	void AddCreatingObject();

private:

	// Object Creator -------------
	char searchNewActionBuffer[256]; 
	char newObjectName[256]; 
	char newObjectDescription[256]; 
	bool showActionDictionary;

	// Sound Emitter 
	bool showMusicSelectionPopup = false; 
	bool showSoundSelectionPopup = false; 
	bool showImageSelectionPopup = false; 

	// Clickable Area -------------
	float2 clickableAreaSizePerc; 
	float2 clickableAreaPosPerc;
	float previewTextureMaxSize; 
	Texture* previewClickableAreaTexture; 
	float2 prevTextureOriginPos; 
	bool clickableAreaActive = false;
	bool focusClickableAreaTab = false; 

	FlyObject* creatingObject = nullptr; 
	Action* selectedAction = nullptr; 

	// Occurrence 
	bool showValueConditions = false; 
	std::string showValueConditionButtonText = "Show Conditions"; 

	// Inventory
	DisplayImageAction* displayImageAction_Inv;
	bool drawOnPickActionDictionary = false; 
	bool drawClickableAreaOver = true; 
};

#endif 

