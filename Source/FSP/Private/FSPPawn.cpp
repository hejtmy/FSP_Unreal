// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPPawn.h"

// Sets default values
AFSPPawn::AFSPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFSPPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFSPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFSPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

