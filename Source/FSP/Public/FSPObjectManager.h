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

	UFUNCTION(BlueprintCallable, Category="FSP|Objects")
	bool AddObject(UFSPObject* Object);

	UFUNCTION(BlueprintGetter, Category="FSP|Objects")
	TArray<UFSPObject*> GetObjects() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSP|Objects")
	TMap<FName, UFSPObject*> Objects;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
