#ifndef _ROOMDOCKPANEL_H_
#define _ROOMDOCKPANEL_H_

#include "DockPanel.h"
#include "Action.h"
#include "MathGeoLib/Math/float4.h"

class Texture; 
class RoomDockPanel : public DockPanel {

public:

	RoomDockPanel(bool isVisible);
	~RoomDockPanel();

	bool Draw();

	void ShowBlackboardTab();

	void DrawRoomVariablesUI();

	void DrawTopButtons();

	// Hierarchy ---
	void DrawRoomHierarchy();
	void DrawMoveLayerSelectableButtons();

	// Viewport ----
	void ShowViewportSettingsTab();

private:
	float4 playStopButtonBackgroundColor; 
	Texture* playStopButtonTexture;

	bool showMusicSelectionPopup = false; 

};

#endif 
