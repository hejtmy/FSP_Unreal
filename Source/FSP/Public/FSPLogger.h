// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSPLog.h"
#include "FSPLogger.generated.h"

UCLASS()
class FSP_API AFSPLogger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFSPLogger();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UFSPLog* Log;
    
    UPROPERTY(VisibleAnywhere)
    USceneComponent* RootSceneComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
