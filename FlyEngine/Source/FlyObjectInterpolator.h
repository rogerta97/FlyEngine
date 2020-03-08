#ifndef _FLY_OBJECT_INTERPOLATOR_H_
#define _FLY_OBJECT_INTERPOLATOR_H_

#include "Math/float2.h"

class FlyObject; 
class FlyObjectInterpolator
{
public: 
	FlyObjectInterpolator(FlyObject* _targetObject);
	~FlyObjectInterpolator(); 

	void UpdateObjectPositionFromTime(float timeInterpolating);

	// Set & Get
	void SetInterpolationSegment(float2 _startPosition, float2 _finishPosition); 
	float2 GetInterpolationStartPos(); 
	float2 GetInterpolationEndPos(); 

	void SetTargetTime(float _targetTime);
	float GetTargetTime(); 

private: 
	FlyObject* targetObject; 
	LineSegment* interpolationSegment;
	float targetTime; 

};

#endif