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
}

void UFSPVisibilityTransformation::ChangeVisibility(bool bVisible)
{
	GetOwner()->SetActorHiddenInGame(!bVisible);
	GetOwner()->SetIsTemporarilyHiddenInEditor(!bVisible);
	TArray<UStaticMeshComponent*> Components;
	GetOwner()->GetComponents<UStaticMeshComponent>(Components);
	GetOwner()->SetActorEnableCollision(bVisible);
	for (auto Component : Components)
	{
		Component->CastShadow = bVisible;
	}
}

void UFSPVisibilityTransformation::ChangeState()
{
	Super::ChangeState();
	ChangeVisibility(!bShowOnStart);
}

void UFSPVisibilityTransformation::ResetState()
{
	Super::ResetState();
	ChangeVisibility(bShowOnStart);
}

