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
class ObjectCreatorDockPanel : public DockPanel {

public:
	ObjectCreatorDockPanel(bool isVisible);
	~ObjectCreatorDockPanel();

	bool Draw();
	void DrawPropertiesTab();


	void ResetObjectData();
	void Close(); 

	// Clickable Area -------------
	void DrawClickableAreaCreator();
	void DrawClickableAreaSettings();
	void PrintClickableAreaObjectVisuals();
	void DrawPrevTextureCA();
	void GetTextureSizeFitted(ImVec2& prevTextureSize);
	void DrawPreviewClickableAreaOnTexture(float2 imageTopLeft, float2 prevTextureSize);

	// Object Creator -------------
	void DrawObjectCreator(); 
	void DrawObjectActionsList(); 
	void DrawSelectable(ActionSelectableInfo selectableInfo, bool& isSelected, int posInList, int selectableHeight, Action* currentTool);
	void DrawAddAndDeleteActionButtons(); 
	void DrawDisplayImageSettings(); 
	void DrawSelectedActionSettings();
	void DrawChangeRoomActionSettings();

	void OnAddActionButtonClicked(); 
	void DrawCreateButton();
	void AddCreatingObject();


private:

	// Object Creator -------------
	char searchNewActionBuffer[256]; 
	char newObjectName[256]; 
	char newObjectDescription[256]; 
	bool showActionDictionary;

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
};

#endif 

