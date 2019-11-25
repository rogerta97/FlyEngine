#ifndef _TOOL_H_
#define _TOOL_H_

#include <string>

enum ToolType
{
	AT_IMAGE,
	AT_CHANGE_SCENE,
	AT_null,
};

struct ToolSelectableInfo
{
	std::string toolName;
	std::string toolDescription;
	ToolType toolType;
};

class Tool
{
public:
	Tool();
	~Tool();

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void CleanUp();

	std::string GetToolName() const; 
	void SetToolName(std::string newName);

	std::string GetToolDescription() const;
	void SetToolDescription(std::string newDescription);

	ToolType GetToolType() const;
	void SetToolType(ToolType newToolType);

	bool& IsSelected(); 
	bool& HasVisual(); 

	ToolSelectableInfo GetToolSelectableInfo(); 

protected:
	ToolType toolType;
	std::string toolName; 
	std::string toolDescription; 
	bool isSelected; 
	bool isVisual; 
}; 

#endif

