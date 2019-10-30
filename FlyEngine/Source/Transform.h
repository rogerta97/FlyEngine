#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "MathGeoLib.h"

class Transform
{
public: 
	Transform(); 
	~Transform(); 
	
	float4x4 CalculateViewMatrix(); 

	float3 GetPosition() const; 
	void SetPosition(float3 newPosition);

	float3 GetRotation() const;
	void SetRotation(float3 newRotation);

	float3 GetScale() const;
	void SetScale(float3 newScale);

private:

	float3 position;
	float3 rotation;
	float3 scale;

	float4x4 ViewMatrix;
};

#endif

