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
	UE_LOG(LogFSP, Display, TEXT("Object state changing"));
}

void UFSPTransformation::ResetState()
{
	UE_LOG(LogFSP, Display, TEXT("Object state resetting"));
}

// Called when the game starts
void UFSPTransformation::BeginPlay()
{
	Super::BeginPlay();

}
