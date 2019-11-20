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
	bool IsMouseInViewport();

	// Get & Set ----
	float2& GetRegionSize(); 
	float2& GetViewportSize();
	float& GetAspectRatio(); 

	GizmoMode GetGizmoMode() const;

	float2& GetViewportCenterGlobal(); 
	float2& GetMouseRelativePosition(); 
	float2& ScreenToWorld(float screenPosX, float screenPosY);

	bool aspectRatioChanged; 
	float verticalOffset; 

private: 

	float2 viewportCenterGlobalPos; 
	float2 regionSize;
	float2 viewportSize;
	float aspectRatio;
	GizmoMode gizmoMode; 
};

#endif

