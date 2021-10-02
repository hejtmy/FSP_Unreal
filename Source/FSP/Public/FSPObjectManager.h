// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSPObjectManager.generated.h"

// Forward declaraion
class UFSPObject;

UCLASS()
class FSP_API AFSPObjectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFSPObjectManager();

	UFUNCTION(BlueprintCallable, Category="FSP|Object")
	bool AddObject(UFSPObject* Object);

	UFUNCTION(BlueprintPure, Category="FSP|Object")
	TArray<UFSPObject*> GetObjects() const;
	
	UFUNCTION(BlueprintCallable, Category="FSP|Object", CallInEditor)
	void ApplyTransformations();

	UFUNCTION(BlueprintCallable, Category="FSP|Object", CallInEditor)
	void ResetTransformations();

	/**
	 * Iterates through all actors in the level and checks if they implement FSP object. If so,
	 * this will enforce them registering with this object.
	 * 
	 */
	void CollectAllLevelObjects();
	
	/**
	 * This abuses the fact that objects automatically register when begin play is called. So this function basically
	 * just hits play and quits again.
	 */
	void FakeCollectAllLevelObjects();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSP|Object")
	TMap<FName, UFSPObject*> Objects;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
