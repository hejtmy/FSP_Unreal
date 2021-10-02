// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPObjectManager.h"

#include "EditorLevelUtils.h"
#include "FSPObject.h"
#include "Kismet/GameplayStatics.h"

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

void AFSPObjectManager::CollectAllLevelObjects()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
	for (const auto Actor : Actors)
	{
		UFSPObject* Component = Actor->FindComponentByClass<UFSPObject>();
		if(Component)
		{
			this->AddObject(Component);
		}
	}
}

void AFSPObjectManager::FakeCollectAllLevelObjects()
{
	UE_LOG(FSP, Warning, TEXT("This function is not implemented yet"));
#if WITH_EDITOR
#endif
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

