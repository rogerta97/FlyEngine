#ifndef _FILE_BROWSER_DOCK_PANEL_H_
#define _FILE_BROWSER_DOCK_PANEL_H_

#include "DockPanel.h"

class FileBrowserDockPanel : public DockPanel {

public:

	FileBrowserDockPanel(bool isVisible);
	~FileBrowserDockPanel();

	bool Draw(); 
	void DrawDirectoryRecursive(string& directory); 

private:
	UID selectedResourceUID; 
};

#endif // !_FILE_BROWSER_DOCK_PANEL_H_

