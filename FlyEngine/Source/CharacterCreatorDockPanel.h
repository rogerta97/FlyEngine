#ifndef _CHARACTER_CREATOR_DOCK_PANEL_H_
#define _CHARACTER_CREATOR_DOCK_PANEL_H_

#include "DockPanel.h"

class FlyObjectCharacter; 
class Action; 
class CharacterCreatorDockPanel : public DockPanel
{
public:
	CharacterCreatorDockPanel(bool isVisible);
	~CharacterCreatorDockPanel(); 

	void ResetObjectData(); 
	void ToggleVisiblity(); 

	bool Draw();
	void DrawCharacterSequentialActionsUI();
	void DrawCharacterAnimationsUI();

private: 
	FlyObjectCharacter* creatingCharacterFO; 
	Action* selectedActionUI; 
};

#endif
