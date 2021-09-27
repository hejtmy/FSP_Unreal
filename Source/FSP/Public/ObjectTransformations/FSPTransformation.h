// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSPUnrealLogDeclarations.h"
#include "FSPTransformation.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FSP_API UFSPTransformation : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFSPTransformation();

	UFUNCTION(BlueprintCallable, Category="FSP|Transformations")
	virtual void ChangeState();

	UFUNCTION(BlueprintCallable, Category="FSP|Transformations")
	virtual void ResetState();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="FSP|Transformations")
	bool bStateChanged = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSP|Transformations")
	UStaticMeshComponent* Mesh;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetMesh();
};
