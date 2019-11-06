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
	float3& GetPositionRef(); 
	void SetPosition(float3 newPosition);
	void SetPosition(float2 newPosition);

	float3 GetRotation() const;
	void SetRotationEuler(float3 newRotation);
	void SetRotationEuler(float2 newRotation);

	float3 GetScale() const;
	void SetScale(float3 newRotation);
	void SetScale(float2 newRotation);

	bool dirty; 

private:

	float3 position;
	float3 rotationEuler;
	float3 scale;

	float4x4 ViewMatrix;
};

#endif

