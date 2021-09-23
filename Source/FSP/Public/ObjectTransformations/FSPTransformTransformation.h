// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSPTransformation.h"
#include "FSPTransformationInterface.h"
#include "Components/ActorComponent.h"
#include "FSPTransformTransformation.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPTransformTransformation : public UFSPTransformation
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSPTransformTransformation();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	FRotator TargetRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	FVector TargetScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	bool bChangeLocation = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	bool bChangeRotation = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	bool bChangeScale = false;

public:
	// Transformation interface implementation -----
	void ChangeState() override;
	void ResetState() override;
	
private:
	FVector OriginalLocation;
	FVector OriginalScale;
	FRotator OriginalRotation;

	void SetOriginalValues();
};
