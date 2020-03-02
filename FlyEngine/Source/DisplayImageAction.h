#ifndef _DISPLAY_IMAGE_ACTION_H_
#define _DISPLAY_IMAGE_ACTION_H_

#include "Action.h"

using namespace std; 

class FlyObject; 
class Quad; 
class Texture; 

class DisplayImageAction : public Action
{
public: 
	DisplayImageAction(FlyObject* _parentObject);
	~DisplayImageAction(); 

	// Virtuals ----------
	void Init(); 
	void Draw(); 
	void CleanUp();

	void DrawUISettings();

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false);
	void DrawActionOccurenceCheckboxes(); 
	bool CreateImage(const char* texturePath); 

	// Set & Get --------
	Quad* GetQuad() const;
	void SetQuad(Quad* newQuad);
	
	void SetTexture(Texture* newTexture);
	Texture* GetTexture() const; 

	void SetWidth(float newWidth);
	float GetWidth() const;

	void SetHeigth(float newWidth);
	float GetHeigth() const;

public:
	bool fromAnimation = false; 

private:
	Quad*		quadMesh;
	Texture*	imageTexture;
	
	float		imageWidth; 
	float		imageHeight; 
};

#endif 