#ifndef _GAMEVIEWPORTDOCKPANEL_H_
#define _GAMEVIEWPORTDOCKPANEL_H_

#include "DockPanel.h"
#include "Globals.h"
#include "vec2.hpp"

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

	void ObjectCreatorPopup();

	glm::vec2 GetRegionSize() const; 
	glm::vec2 GetViewportSize() const;  

private: 

	int topBarWidth;

	glm::vec2 regionSize; 
	glm::vec2 viewportSize; 

};

#endif

