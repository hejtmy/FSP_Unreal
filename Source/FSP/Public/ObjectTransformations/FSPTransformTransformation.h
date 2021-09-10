// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSPTransformationInterface.h"
#include "Components/ActorComponent.h"
#include "FSPTransformTransformation.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPTransformTransformation : public UActorComponent, public IFSPTransformationInterface
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
	FVector TargetRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	FVector TargetScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	bool ChangeLocation = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	bool ChangeRotation = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformation")
	bool ChangeScale = false;

public:
	// Transformation interface implementation -----
	virtual void ChangeState_Implementation() override;
	virtual void ResetState_Implementation() override;
	
private:
	FVector OriginalLocation;
	FVector OriginalScale;
	FRotator OriginalRotation;
};
