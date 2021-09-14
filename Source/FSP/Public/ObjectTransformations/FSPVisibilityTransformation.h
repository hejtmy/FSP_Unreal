// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSPTransformation.h"
#include "Components/ActorComponent.h"
#include "FSPTransformationInterface.h"
#include "FSPVisibilityTransformation.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPVisibilityTransformation : public UFSPTransformation
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSPVisibilityTransformation();
	
	void ChangeState() override;
	void ResetState() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FSP|Transformations")
	bool bShowOnStart = false;
public:	
};
