#include "Transform.h"
#include "Globals.h"
#include "mmgr.h"

Transform::Transform()
{
	position = float3(0, 0, 0);
	rotationEuler = float3(0, 0, 0);
	scale = float3(1, 1, 1);

	CalculateViewMatrix(); 
}

Transform::~Transform()
{
}

float3 Transform::GetPosition() const
{
	return position; 
}

float3& Transform::GetPositionRef()
{
	return position;
}

void Transform::SetPosition(float3 newPosition)
{
	position = newPosition; 
}

void Transform::SetPosition(float2 newPosition)
{
	SetPosition(float3(newPosition.x, newPosition.y, 0)); 
}

float3 Transform::GetRotation() const
{
	return rotationEuler; 
}

void Transform::SetRotationEuler(float3 newRotation)
{
	rotationEuler = newRotation;
}

void Transform::SetRotationEuler(float2 newRotation)
{
	SetRotationEuler(float3(newRotation.x, newRotation.y, 0)); 
}

float3 Transform::GetScale() const
{
	return scale;
}

void Transform::SetScale(float3 newScale)
{
	scale = newScale; 
}

void Transform::SetScale(float2 newScale)
{
	SetScale(float3(newScale.x, newScale.y, 1));
}

float4x4 Transform::CalculateViewMatrix()
{
	//new_mat = new_mat * transform.rotation;
	float4x4 new_mat = float4x4::identity;

	new_mat = new_mat * new_mat.Scale(scale);

	new_mat.RotateX(rotationEuler.x); 
	new_mat.RotateY(rotationEuler.y); 

    new_mat.SetTranslatePart(position);
	

	ViewMatrix = new_mat;
	return ViewMatrix; 
}
