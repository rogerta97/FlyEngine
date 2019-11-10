#ifndef _FLY_OBJECT_H_
#define _OBJECT_CREATOR_H_

#include <string>
#include "ModuleManager.h"

class FlyObject; 
class Texture; 
class ObjectCreator
{
public:
	ObjectCreator();
	~ObjectCreator();

	void Draw();

	void DrawObjectNameBar(); 
	void DrawToolsList(); 
	void DrawAddAndDeleteButtons();
	void DrawSelectedToolProperties(); 
	void DrawPopups();

	void CleanBooleans();

	void OnNewToolButtonClicked(); 
	bool DrawToolSelectable(ToolSelectableInfo newToolInfo);

public:
	char searchToolBuffer[256] = "";

	// Image Tool ----
	bool showImageTool = false; 
	Texture* previewImageTexture = nullptr; 

	ToolType selectedInList = ToolType::AT_null; 
};

#endif

