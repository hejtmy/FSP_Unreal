// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectTransformations/FSPVisibilityTransformation.h"

// Sets default values for this component's properties
UFSPVisibilityTransformation::UFSPVisibilityTransformation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UFSPVisibilityTransformation::BeginPlay()
{
	Super::BeginPlay();
	ResetState();
}

void UFSPVisibilityTransformation::ChangeState()
{
	GetOwner()->SetActorHiddenInGame(bShowOnStart);
}

void UFSPVisibilityTransformation::ResetState()
{
	GetOwner()->SetActorHiddenInGame(!bShowOnStart);
}
