#include "FlyObjectInterpolator.h"
#include "Globals.h"
#include "Transform.h"
#include "FlyObject.h"

FlyObjectInterpolator::FlyObjectInterpolator(FlyObject* _targetObject)
{
	targetObject = _targetObject; 
	interpolationSegment = new LineSegment();
	targetTime = 1.0f;
}

FlyObjectInterpolator::~FlyObjectInterpolator()
{
}

void FlyObjectInterpolator::CleanUp()
{
	delete interpolationSegment; 
	targetObject = nullptr; 
}

void FlyObjectInterpolator::UpdateObjectPositionFromTime(float timeInterpolating)
{
	if (targetObject == nullptr)
	{
		FLY_ERROR("Interpolating Fly Object With No Fly Object Existing"); 
	}

	float timePercentageDone = timeInterpolating / targetTime; 

	if(timeInterpolating > targetTime)
		timePercentageDone = 1;

	float3 currentDesiredPos = interpolationSegment->GetPoint(timePercentageDone); 
	targetObject->transform->SetPosition(currentDesiredPos); 
}

void FlyObjectInterpolator::SetInterpolationSegment(float2 _startPosition, float2 _finishPosition)
{
	interpolationSegment->a = float3(_startPosition.x, _startPosition.y, 1.0f);
	interpolationSegment->b = float3(_finishPosition.x, _finishPosition.y, 1.0f);
}

float2 FlyObjectInterpolator::GetInterpolationStartPos()
{
	float2 beginPoint = float2(interpolationSegment->a.x, interpolationSegment->a.y); 
	return beginPoint;
}

float2 FlyObjectInterpolator::GetInterpolationEndPos()
{
	float2 endPoint = float2(interpolationSegment->b.x, interpolationSegment->b.y);
	return endPoint;
}

void FlyObjectInterpolator::SetTargetTime(float _targetTime)
{
	targetTime = _targetTime; 
}

float FlyObjectInterpolator::GetTargetTime()
{
	return targetTime;
}

