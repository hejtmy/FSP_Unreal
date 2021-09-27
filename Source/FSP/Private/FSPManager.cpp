// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPManager.h"

// Sets default values
AFSPManager::AFSPManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFSPManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFSPManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AFSPManager::ValidataLogger()
{
	if(IsValid(Logger)) return true;
	UE_LOG(FSP, Warning, TEXT("Logger actor is not valid"));
	return false;
}

bool AFSPManager::ValidataRecorder()
{
	if(IsValid(Recorder)) return true;
	UE_LOG(FSP, Warning, TEXT("Recorder actor is not valid"));
	return false;
	
}

void AFSPManager::TakeScreenshots_Implementation()
{
	Recorder->CreateScreenshots(Logger);	
}

void AFSPManager::StopScreenshotting_Implementation()
{
	if(ValidataRecorder()) { return; }
	Recorder->StopScreenshotting();
}

void AFSPManager::MakeARecording_Implementation()
{
	if(!(ValidataLogger() && ValidataRecorder()))
	{
		UE_LOG(FSP, Warning, TEXT("Objects are not valid, cannot make a recording"));
		return;
	}
	Recorder->StartRecording(Logger);
}

void AFSPManager::FinishRecording_Implementation()
{
	if(ValidataRecorder()) return;
	Recorder->StopRecording();
}

