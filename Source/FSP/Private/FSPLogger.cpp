// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPLogger.h"


// Sets default values
AFSPLogger::AFSPLogger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Log = CreateDefaultSubobject<UFSPLog>(TEXT("Log"));
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;
}

// Called when the game starts or when spawned
void AFSPLogger::BeginPlay()
{
   Super::BeginPlay();
	
}

// Called every frame
void AFSPLogger::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

