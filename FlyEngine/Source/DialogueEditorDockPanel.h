#ifndef _DIALOGUE_EDITOR_DOCK_PANEL_H_
#define _DIALOGUE_EDITOR_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ImGui/imgui.h"

class DialogueNodeGraph;
class DialogueEditorDockPanel : public DockPanel {

public:

	DialogueEditorDockPanel(bool isVisible);
	~DialogueEditorDockPanel();

	bool Draw();

	DialogueNodeGraph* GetNodeGraph();

private: 
	DialogueNodeGraph* nodeGraph;
};

#endif // !_FILE_BROWSER_DOCK_PANEL_H_

