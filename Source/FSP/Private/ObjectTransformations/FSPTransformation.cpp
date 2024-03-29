// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectTransformations/FSPTransformation.h"


// Sets default values for this component's properties
UFSPTransformation::UFSPTransformation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UFSPTransformation::ChangeState()
{
	if(Mesh == nullptr)
	{
		SetMesh();
	}
	bStateChanged = true;
}

void UFSPTransformation::ResetState()
{
	if(Mesh == nullptr)
	{
		SetMesh();
	}
	bStateChanged = false;
}

// Called when the game starts
void UFSPTransformation::BeginPlay()
{
	Super::BeginPlay();
	SetMesh();
}

void UFSPTransformation::SetMesh()
{
	TArray<UStaticMeshComponent*> Objects;
	GetOwner()->GetComponents<UStaticMeshComponent>(Objects);
	if(Objects.Num() <= 0)
	{
		Mesh = nullptr;
	}
	else
	{
		Mesh = Objects[0];
	}
	if(Objects.Num() > 1)
	{
		UE_LOG(FSP, Display, TEXT("The object %s has multiple meshes, only will be modifying the first one"),
			*GetOwner()->GetName());
	}
}
