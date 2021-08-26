// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPTrack.h"

// Sets default values
AFSPTrack::AFSPTrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
	
	Track = CreateDefaultSubobject<USplineComponent>(TEXT("Track"));

    Track->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AFSPTrack::BeginPlay()
{
	Super::BeginPlay();	
}
