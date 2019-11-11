#ifndef _FLY_OBJECT_EDITOR_UI_H_
#define _FLY_OBJECT_EDITOR_UI_H_

#include <string>
#include "ModuleManager.h"

class Texture;
class FlyObject;
struct FlyObjectEditorUI {

private:
	static FlyObjectEditorUI* instance;
	FlyObjectEditorUI();

public:
	static FlyObjectEditorUI* getInstance();
	~FlyObjectEditorUI();
	void Delete();

	//void Draw();

	//void DrawObjectNameBar(); 
	//void DrawAddAndDeleteButtons();

	//void OnNewToolButtonClicked();
	
	void DrawToolsList(FlyObject* toolOwnerObject);
	void DrawToolProperties(FlyObject* selectedObject, ToolType selectedObjectType);
	bool DrawToolSelectable(ToolSelectableInfo newToolInfo, bool isSelected);
	
	
	//void DrawImageToolInfo();
	//void DrawPopups();

	//void CleanBooleans();
//
//public:
//	char searchToolBuffer[256] = "";
//	char newObjectName[256] = "";
//
//	// Image Tool ----
//	bool showImageTool = false; 
//	Texture* previewImageTexture = nullptr; 
//
//	ToolType selectedInList = ToolType::AT_null; 
};

#endif

