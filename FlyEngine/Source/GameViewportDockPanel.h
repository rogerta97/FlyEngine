#ifndef _GAMEVIEWPORTDOCKPANEL_H_
#define _GAMEVIEWPORTDOCKPANEL_H_

#include "DockPanel.h"
#include "Globals.h"
#include "MathGeoLib.h"

class TextureMSAA; 
class GameViewportDockPanel : public DockPanel
{
public: 
	GameViewportDockPanel(bool isVisible); 
	~GameViewportDockPanel(); 

	bool Draw();
	void ReceiveEvent(FlyEngineEvent eventType);
	void FitViewportToRegion();

	void DrawTopBar();

	void ResetAttributeSelectionBooleans();

	void ObjectCreatorPopup();

	float2 GetRegionSize() const; 
	float2 GetViewportSize() const;  

	bool aspectRatioChanged; 

private: 
	int topBarWidth;

	float2 regionSize;
	float2 viewportSize;

	// Object Creator -------
	bool containsAttributeImage = false; 
};

#endif

