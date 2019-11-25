#ifndef _OBJECT_CREATOR_DOCK_PANEL_H_
#define _OBJECT_CREATOR_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ModuleManager.h"
#include "MathGeoLib/Math/float2.h"
#include "ImGui/imgui.h"

#include <string>

class FlyObject;
class Tool; 
class Texture; 
class ObjectCreatorDockPanel : public DockPanel {

public:
	ObjectCreatorDockPanel(bool isVisible);
	~ObjectCreatorDockPanel();

	bool Draw(); 

	void ResetObjectData();
	void Close(); 

	// Clickable Area -------------
	void DrawClickableAreaCreator();
	void DrawClickableAreaSettings();
	void PrintClickableAreaObjectVisuals();
	void DrawPrevTextureCA();
	void GetTextureSizeFitted(ImVec2& prevTextureSize);
	void DrawPreviewClickableAreaOnTexture(float2 imageTopLeft);

	// Object Creator -------------
	void DrawObjectCreator(); 
	void DrawObjectToolsList(); 
	void DrawSelectable(ToolSelectableInfo selectableInfo, bool& isSelected, Tool* currentTool);
	void DrawAddAndDeleteToolButtons(); 
	void DrawToolImageSettings(); 
	void DrawSelectedToolSettings(); 
	void OnAddToolButtonClicked(); 
	void DrawCreateButton(); 

private:

	// Object Creator -------------
	char searchNewToolBuffer[256]; 
	char newObjectName[256]; 
	bool showToolDictionary;

	// Clickable Area -------------
	float2 clickableAreaSize; 
	float2 clickableAreaPos;
	float previewTextureMaxSize; 
	Texture* previewClickableAreaTexture; 
	float2 prevTextureOriginPos; 
	bool clickableAreaActive; 

	FlyObject* creatingObject = nullptr; 
	Tool* selectedTool = nullptr; 
};

#endif 

