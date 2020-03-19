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

	void CleanUp(); 

	float2 targetPosition;
	float2 startPosition; 
	BoundingBox* graphBox; 

	void Save(JSON_Object* jsonObject, string serializeObjectString);
	void DrawStepGUI(int stepPos, float selectableHeigth = 65);

	float GetLenght(); 
	void SetMovementSpeed(float _newSpeed); 
	
	// Get 
	float GetSpeed(); 
	float GetTargetTime(); 

private:
	float speed; 
	float targetTime;
};

enum PathPlayMode
{
	PATH_PLAY_ONCE, 
	PATH_LOOP_TIMES,
	PATH_LOOP_TELEPORT,
	PATH_LOOP_CYCLE,
};

enum MovementState
{
	MOVEMENT_ONGOING,
	MOVEMENT_IDLE,
	MOVEMENT_CYCLE,
};

class FollowPathAction : public Action
{
public:
	FollowPathAction(FlyObject* _parentObject);
	~FollowPathAction();

	void Update(float dt);
	void Draw(); 
	void DoAction(); 
	void CleanUp(); 

	// Draw ---------
	void DrawPath(); 

	// Draw UI ------
	void DrawUISettings();
	void DrawBehaviorSettings();
	void SetConstantSpeed(float newSpeed);
	void DrawVisualSettings();
	void DrawActionOccurenceCheckboxes(); 

	// Save ---------
	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false, int actionPositionInObject = 0);

	// Utility ---
	void AddStep(PathStep* newStep, int stepIndex);
	void BeginMovement(); 
	bool UpdateObjectPosition(); 
	void BeginNextStep(); 
	void ResetPathMovement(); 
	void Stop(bool goToStart);

	// Encapsulation ---
	PathPlayMode GetPathMode();
	void SetPathMode(PathPlayMode newPathMode);

	void SetIsSpeedConstant(bool isSpeedConst);

private: 
	std::list<PathStep*>* pathSteps; 
	PathPlayMode pathPlayMode; 
	FlyObjectInterpolator* flyObjectInterpolation; 
	MovementState movementState; 

	bool showVariableConditions; 

	// Runtime Movement
	int currentStepIndex; 
	float stepTime; 
	float constantSpeed; 
	bool isSpeedConstant; 

	// Run Amount Times 
	int loopsCompleted; 
	int targetLoopsAmount; 

	// Draw Path Variables
	float2 startPosition; 
	float4 graphBoxColor; 
	float4 lineColor; 
	float lineWidth; 
};

#endif 