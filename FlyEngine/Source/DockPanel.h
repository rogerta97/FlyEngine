#ifndef _DOCKPANEL_H_
#define _DOCKPANEL_H_

#include <string>

using namespace std;

class DockPanel {

public:

	DockPanel(std::string newPanelName);
	~DockPanel();

	virtual void Draw();

	string GetName() const;
	void inline SetName(string newName);

protected:
	string panelName;
	bool visible;
};

#endif 

