// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectTransformations/FSPMaterialTransformation.h"

// Sets default values for this component's properties
UFSPMaterialTransformation::UFSPMaterialTransformation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSPMaterialTransformation::ChangeState()
{
	if(Mesh != nullptr)
	{
		Mesh->SetMaterial(0, MaterialNew);
	}
	Super::ChangeState();
}

void UFSPMaterialTransformation::ResetState()
{
	if(Mesh != nullptr)
	{
		Mesh->SetMaterial(0, MaterialDefault);
	}
	Super::ResetState();
}

// Called when the game starts
void UFSPMaterialTransformation::BeginPlay()
{
	Super::BeginPlay();
	if(Mesh != nullptr)
	{
		MaterialDefault = Mesh->GetMaterial(0);
	}
}