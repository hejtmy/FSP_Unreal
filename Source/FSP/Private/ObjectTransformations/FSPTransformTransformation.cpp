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
	OriginalLocation = GetOwner()->GetActorLocation();
	OriginalRotation = GetOwner()->GetActorRotation();
	OriginalScale = GetOwner()->GetActorScale();
}

void UFSPTransformTransformation::ChangeState_Implementation()
{
	IFSPTransformationInterface::ChangeState();
}

void UFSPTransformTransformation::ResetState_Implementation()
{
	GetOwner()->SetActorLocation(OriginalLocation);
	GetOwner()->SetActorRotation(OriginalRotation);
	GetOwner()->SetActorScale3D(OriginalScale);
}