// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPObject.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFSPObject::UFSPObject()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	//Transformations = Cast<UFSPTransformation>(GetOwner()->GetComponentsByClass(UFSPTransformation::StaticClass()));
}

void UFSPObject::ChangeState()
{
	for (const auto transformation : Transformations)
	{
		transformation->ChangeState();
	}
	bTransformationStateChanged = true;
}

void UFSPObject::ResetState()
{
	for (const auto transformation : Transformations)
	{
		transformation->ResetState();
	}
	bTransformationStateChanged = false;
}

// Called when the game starts
void UFSPObject::BeginPlay()
{
	Super::BeginPlay();
	ObjectManger = GetObjectManger();
	if(ObjectManger == nullptr) return;
	ObjectManger->AddObject(this);
	GetOwner()->GetComponents<UFSPTransformation>(Transformations);
}

AFSPObjectManager* UFSPObject::GetObjectManger()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFSPObjectManager::StaticClass(), FoundActors);
	if(FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no object manager actors in the scene. PLease add one"))
		return nullptr;
	}

	if(FoundActors.Num() > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("There are more than one object manager actors in the scene. Please add JUST one"))
		return nullptr;
	}
	return Cast<AFSPObjectManager>(FoundActors[0]);
}

