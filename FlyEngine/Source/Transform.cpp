#include "Transform.h"
#include "ViewportManager.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"

#include "mmgr.h"

Transform::Transform()
{
	position = float3(0, 0, 0);
	rotationEuler = float3(0, 0, 0);
	scale = float3(1, 1, 1);

	scalePrevTick = GetScale();

	CalculateViewMatrix(); 
}

Transform::~Transform()
{
}

float2 Transform::GetPosition(bool withAR)
{
	float2 posToRet = float2(position.x, position.y); 

	if (withAR){
		posToRet *= App->moduleImGui->gameViewportDockPanel->GetAspectRatio(); 
	}

	return posToRet;
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

void Transform::SetPosition(float newPositionX, float newPositionY)
{
	SetPosition(float2(newPositionX, newPositionY)); 
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

float2 Transform::GetScale() const
{
	return float2(scale.x, scale.y);
}

void Transform::SetScale(float3 newScale)
{
	scale = newScale; 
}

void Transform::SetScale(float2 newScale)
{
	SetScale(float3(newScale.x, newScale.y, 1));
}

float2& Transform::GetScaleInc()
{
	float2 currentScale = GetScale(); 
	float incX = scalePrevTick.x / currentScale.x;
	float incY = scalePrevTick.y / currentScale.y;

	float2 inc = float2(incX, incY);
	return inc;
}

float4x4 Transform::CalculateViewMatrix()
{
	float4x4 new_mat = float4x4::identity;

	new_mat = new_mat * new_mat.Scale(scale);

	new_mat.RotateX(rotationEuler.x); 
	new_mat.RotateY(rotationEuler.y); 

    new_mat.SetTranslatePart(position);
	
	ViewMatrix = new_mat;
	return ViewMatrix; 
}

float4x4& Transform::GetViewMatrix()
{
	return ViewMatrix; 
}
