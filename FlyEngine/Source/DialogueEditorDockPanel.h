#ifndef _DIALOGUE_EDITOR_DOCK_PANEL_H_
#define _DIALOGUE_EDITOR_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ImGui/imgui.h"

class NodeGraph; 
class DialogueEditorDockPanel : public DockPanel {

public:

	DialogueEditorDockPanel(bool isVisible);
	~DialogueEditorDockPanel();

	bool Draw();

private: 
	NodeGraph* nodeGraph; 
};

#endif // !_FILE_BROWSER_DOCK_PANEL_H_

