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
	void SetRotation(float3 newRotation);
	void SetRotation(float2 newRotation);

	float3 GetScale() const;
	void SetScale(float3 newScale);

	bool dirty; 

private:

	float3 position;
	float3 rotation;
	float3 scale;

	float4x4 ViewMatrix;
};

#endif

