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
	void AddFrame(Texture* newFrame);
	void Clear(); 

	Texture* GetFrameByPos(int pos);
	int GetFramePos(Texture* frame); 

	// Popups ----------
	void DrawAddFramePopup(); 

	// Uitlity ----------
	int GetFramesAmount();
	float GetFramesInterval(); 

	// Set & Get --------
	float& GetSpeed();
	void SetSpeed(float newSpeed); 

	string GetName();
	void SetName(string newName);

private:
	int framesAmount = 0; 
	list<Texture*> frameTexturesList;
	float animationSpeed = 0.0f;
	string name; 
};

#endif

