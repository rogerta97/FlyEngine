#ifndef _FILE_BROWSER_DOCK_PANEL_H_
#define _FILE_BROWSER_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ImGui/imgui.h"

class Texture; 
class FileBrowserDockPanel : public DockPanel {

public:

	FileBrowserDockPanel(bool isVisible);
	~FileBrowserDockPanel();

	bool Draw();
	void DrawRightColumn();
	void DrawLeftColumn();

	void DrawDirectoryRecursive(string& directory); 
	ImVec2 GetImageDimensionsInPreview(Texture* texture); 

private:
	UID selectedResourceUID; 
	float maxImageWidth; 
};

#endif // !_FILE_BROWSER_DOCK_PANEL_H_

