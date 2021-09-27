// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSPObjectManager.h"
#include "Components/ActorComponent.h"
#include "ObjectTransformations/FSPTransformation.h"
#include "FSPObject.generated.h"

//Forward declaration

UCLASS( ClassGroup=(FSP), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPObject : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSPObject();

	UPROPERTY(EditAnywhere, Category="FSP|Object")
	FName ObjectName;

	// TRANSFORMATIONS ---------
	UFUNCTION(BlueprintCallable, Category="FSP|Object", CallInEditor)
	void ApplyTransformations();

	UFUNCTION(BlueprintCallable, Category="FSP|Object", CallInEditor)
	void ResetTransformations();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="FSP|Object")
	bool bTransformationStateChanged = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AFSPObjectManager* ObjectManger;

	UPROPERTY()
	TArray<UFSPTransformation*> Transformations;

	AFSPObjectManager* GetObjectManger();
};
