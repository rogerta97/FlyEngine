#ifndef _GAMEVIEWPORTDOCKPANEL_H_
#define _GAMEVIEWPORTDOCKPANEL_H_

#include "DockPanel.h"
#include "Globals.h"
#include "MathGeoLib.h"

enum GizmoMode
{
	GIZMO_SELECT,
	GIZMO_MOVE,
	GIZMO_null, 
};

class TextureMSAA; 
class GameViewportDockPanel : public DockPanel
{
public: 
	GameViewportDockPanel(bool isVisible); 
	~GameViewportDockPanel(); 

	// Draw ---
	bool Draw();
	void DrawTopBar();
	void FitViewportToRegion();

	void ReceiveEvent(FlyEngineEvent eventType);

	// Get & Set ----
	float2 GetRegionSize() const; 
	float2 GetViewportSize() const;

	GizmoMode GetGizmoMode() const;

	float2& GetViewportCenterGlobal(); 

	bool aspectRatioChanged; 

private: 

	float2 viewportCenterGlobal; 
	float2 regionSize;
	float2 viewportSize;
	GizmoMode gizmoMode; 
};

#endif

