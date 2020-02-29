#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <list>
#include <string>

using namespace std; 

class Texture; 
class Animation
{
public:
	Animation();
	~Animation();

	void BuildAnimation(string loadingPath);

	// Frame Control ----
	void AddFrame(int framePos = -1);
	Texture* GetFrame(int pos);
	void Clear(); 

	// Popups ----------
	void DrawAddFramePopup(); 

	// Uitlity ----------
	int GetFramesAmount();

	// Set & Get --------
	float GetAnimationSpeed();
	void SetAnimationSpeed(float newSpeed); 

	string GetName();
	void SetName(string newName);

private:
	int framesAmount = 0; 
	list<Texture*> frameTexturesList;
	float animationSpeed = 0.0f;
	string name; 
};

#endif

