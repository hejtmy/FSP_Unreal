// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSPObjectManager.h"
#include "Components/ActorComponent.h"
#include "FSPObject.generated.h"

//Forward declaration

UCLASS( ClassGroup=(FSP), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPObject : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSPObject();

	UPROPERTY(EditAnywhere, Category="FSP")
	FName ObjectName;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY();
	AFSPObjectManager* ObjectManger;

	AFSPObjectManager* GetObjectManger();
};
