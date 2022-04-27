// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectTransformations/FSPTransformTransformation.h"

// Sets default values for this component's properties
UFSPTransformTransformation::UFSPTransformTransformation()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UFSPTransformTransformation::BeginPlay()
{
	Super::BeginPlay();
}

void UFSPTransformTransformation::ChangeState()
{
	Super::ChangeState();
	if(bChangeLocation)
	{
		GetOwner()->SetActorLocation(TargetLocation);
	}
	if(bChangeRotation)
	{
		GetOwner()->SetActorRotation(TargetRotation);
	}
	if(bChangeScale)
	{
		GetOwner()->SetActorScale3D(TargetScale);
	}
}

void UFSPTransformTransformation::ResetState()
{
	Super::ResetState();
	GetOwner()->SetActorLocation(OriginalLocation);
	GetOwner()->SetActorRotation(OriginalRotation);
	GetOwner()->SetActorScale3D(OriginalScale);
}

void UFSPTransformTransformation::SetOriginalValues()
{
	OriginalLocation = GetOwner()->GetActorLocation();
	OriginalRotation = GetOwner()->GetActorRotation();
	OriginalScale = GetOwner()->GetActorScale();
}
