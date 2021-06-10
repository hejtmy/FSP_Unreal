// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FSPTrackRider.h"
#include "FSPTrack.h"
#include "CineCameraComponent.h"
#include "FSPPawn.generated.h"

UCLASS()
class FSP_API AFSPPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AFSPPawn();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    UPROPERTY(VisibleAnywhere)
    UFSPTrackRider* TrackRider;

    UPROPERTY(EditAnywhere)
    float Duration;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    UCineCameraComponent* Camera;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    USceneComponent* SceneRoot;

};
