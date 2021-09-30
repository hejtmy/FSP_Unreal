// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPObjectManager.h"
#include "FSPObject.h"

// Sets default values
AFSPObjectManager::AFSPObjectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool AFSPObjectManager::AddObject(UFSPObject* Object)
{
	if(Objects.Contains(Object->ObjectName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not adding object %s from Actor %s as the name is already present"),
			*Object->ObjectName.ToString(), *Object->GetOwner()->GetName());
		return false;
	}
	Objects.Add(Object->ObjectName, Object);
	return true;
}

TArray<UFSPObject*> AFSPObjectManager::GetObjects() const
{
	TArray<UFSPObject*> Result;
	Objects.GenerateValueArray(Result);
	return Result;
}

void AFSPObjectManager::ApplyTransformations()
{
	for (const auto Object : GetObjects())
	{
		Object->ApplyTransformations();
	}
}

void AFSPObjectManager::ResetTransformations()
{
	for (const auto Object : GetObjects())
	{
		Object->ResetTransformations();
	}
}

// Called when the game starts or when spawned
void AFSPObjectManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFSPObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

