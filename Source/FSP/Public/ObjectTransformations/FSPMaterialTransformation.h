// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSPTransformation.h"
#include "Components/ActorComponent.h"
#include "FSPMaterialTransformation.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPMaterialTransformation : public UFSPTransformation
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSPMaterialTransformation();
	
	// Transformation interface implementation -----
	virtual void ChangeState() override;
	virtual void ResetState() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Transformations")
	UMaterialInterface* MaterialNew;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSP|Transofrmations")
	UMaterialInterface* MaterialDefault;

};
