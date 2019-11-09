#ifndef _TOOL_H_
#define _TOOL_H_

#include <string>

enum ToolType
{
	AT_IMAGE,
	AT_null,
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

public:
	ToolType toolType;

	std::string toolName; 
	std::string toolDescription; 
}; 

#endif

