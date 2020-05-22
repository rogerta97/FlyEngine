#ifndef _DISPLAY_IMAGE_ACTION_H_
#define _DISPLAY_IMAGE_ACTION_H_

#include "Action.h"
#include "Math/float2.h"

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
	void DoAction(); 
	void CleanUp();

	void Draw(); 
	void PushAlphaDraw(float alphaIntensity); 

	void DrawUISettings();

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false, int actionPositionInObject = 0);
	void DrawActionOccurenceCheckboxes(); 
	bool CreateImage(const char* texturePath);
	void SetImageTextureByPath(const char* texturePath);

	bool IsVertical();

	void SetOwnPosition(float2 newPosition); 

	// Set & Get --------
	Quad* GetQuad() const;
	void SetQuad(Quad* newQuad);
	
	void SetTexture(Texture* newTexture);
	Texture* GetTexture() const; 

	void SetWidth(float newWidth);
	float GetWidth() const;

	void SetHeigth(float newWidth);
	float GetHeigth() const;

	void SetVisible(bool newVisible);

public:
	bool fromAnimation = false; 
	bool holdingData = false; 
	bool isSlotIcon = false; 

private:
	Quad*		quadMesh;
	Texture*	imageTexture;

	float2		ownPosition; 
	
	float		imageWidth; 
	float		imageHeight; 
	float		alphaDrawIntensity;
};

#endif 