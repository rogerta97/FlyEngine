#ifndef _DOCKPANEL_H_
#define _DOCKPANEL_H_

#include <string>

using namespace std;

enum DockPanelMember {

	MEMBER_ROOM_OVERVIEW,
	MEMBER_ROOM_EDIT,
	MEMBER_BOTH,
	MEMBER_null
};

class DockPanel {

public:

	DockPanel(std::string newPanelName, bool isVisible);
	~DockPanel();

	virtual bool Draw();

	// Utility
	bool ToggleVisibility(); 

	// Set & Get 
	string GetName() const;
	void inline SetName(string newName);

protected:

	DockPanelMember isMemberFrom = MEMBER_null;
	string panelName;
	bool visible;
};

#endif 

