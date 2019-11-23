#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "MathGeoLib.h"

class Transform
{
public: 
	Transform(); 
	~Transform(); 
	
	float4x4 CalculateViewMatrix(); 
	float4x4& GetViewMatrix(); 

	float2 GetPosition(bool withAR = false);
	float3& GetPositionRef(); 

	void SetPosition(float3 newPosition);
	void SetPosition(float2 newPosition);
	void SetPosition(float newPositionX, float newPositionY);

	float3 GetRotation() const;
	void SetRotationEuler(float3 newRotation);
	void SetRotationEuler(float2 newRotation);

	float2 GetScale() const;
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

