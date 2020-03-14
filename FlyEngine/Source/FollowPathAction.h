#ifndef _FOLLOW_PATH_ACTION_H_
#define _FOLLOW_PATH_ACTION_H_

#include "Action.h"
#include "Math/float2.h"
#include "Math/float4.h"
#include <list>

class FlyObjectInterpolator; 
class BoundingBox; 
class PathStep
{
public: 

	PathStep();
	~PathStep(); 

	float2 targetPosition; 
	float targetTime; 
	BoundingBox* graphBox; 

	void Save(JSON_Object* jsonObject, string serializeObjectString);
	void DrawStepGUI(int stepPos, float selectableHeigth = 65);
};

enum PathPlayMode
{
	PATH_ONE_TIME, 
	PATH_AMOUNT_TIMES,
	PATH_LOOP_TELEPORT,
	PATH_LOOP_CYCLE,
};

enum MovementState
{
	MOVEMENT_ONGOING,
	MOVEMENT_IDLE,
};

class FollowPathAction : public Action
{
public:
	FollowPathAction(FlyObject* _parentObject);
	~FollowPathAction();

	void Update(float dt);
	void Draw(); 
	void DoAction(); 

	// Draw ---------
	void DrawPath(); 

	// Draw UI ------
	void DrawUISettings();
	void DrawBehaviorSettings();
	void DrawVisualSettings();

	// Save ---------
	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false);

	// Utility ---
	void AddStep(PathStep* newStep);
	void BeginMovement(); 
	bool UpdateObjectPosition(); 
	void BeginNextStep(); 

	// Encapsulation ---
	PathPlayMode GetPathMode();
	void SetPathMode(PathPlayMode newPathMode);

private: 
	std::list<PathStep*>* pathSteps; 
	PathPlayMode pathPlayMode; 
	FlyObjectInterpolator* flyObjectInterpolation; 
	MovementState movementState; 

	// Runtime Movement
	int currentStepIndex; 
	float stepTime; 

	// Draw Path Variables
	float2 startPosition; 
	float4 graphBoxColor; 
	float4 lineColor; 
	float lineWidth; 
};

#endif 