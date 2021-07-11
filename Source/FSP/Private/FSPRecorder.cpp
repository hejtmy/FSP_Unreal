// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPRecorder.h"

#include "HighResScreenshot.h"

// Sets default values for this component's properties
AFSPRecorder::AFSPRecorder()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneAnalyzer = CreateDefaultSubobject<UFSPSceneAnalyzer>(TEXT("SceneAnalyzer"));
}

// Called when the game starts
void AFSPRecorder::BeginPlay()
{
	Super::BeginPlay();

	if(bIsScreenshotting)
	{
		
	}
}

// Called every frame
void AFSPRecorder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFSPRecorder::StartRecording()
{
	
}

void AFSPRecorder::CreateScreenshots()
{
	UE_LOG(LogTemp, Display, TEXT("Starting screenshotting"));
	if(bIsScreenshotting)
	{
		UE_LOG(LogTemp, Display, TEXT("Screenshots are underway"));
		return;
	}
	//Check if Pawn has been assigned
	Pawn->TrackRider->StopMoving();
	Pawn->TrackRider->ShowControls(false);
	
	LastScreenshotTrack = 0.0;
	
	FTimerDelegate NextFuncDelegate;
	NextFuncDelegate.BindLambda([&](){
		const float newTrackPosition = Pawn->TrackRider->TrackPosition + 1.0/static_cast<float>(nScreenshots);
		UE_LOG(LogTemp, Display, TEXT("Delegate called"));
		if(newTrackPosition > 1)
		{
			FinishScreenshotting();
		}
		else
		{
			Pawn->TrackRider->SetTrackPosition(newTrackPosition);
			CreateScreenshot();
		}
	});
	//GetWorldTimerManager().SetTimer(ScreenshottingHandle, NextFuncDelegate, ScreenshotDelay,
	//	true, 0.0f);
	GetWorldTimerManager().SetTimer(ScreenshottingHandle, this, &AFSPRecorder::CreateNextScreenshot,
		ScreenshotDelay, true, 0.0f);
}

void AFSPRecorder::CreateNextScreenshot()
{
	LastScreenshotTrack = LastScreenshotTrack + 1.0/static_cast<float>(nScreenshots);
	UE_LOG(LogTemp, Display, TEXT("Taking a screenshot %s"), *FString::SanitizeFloat(LastScreenshotTrack));
	if(LastScreenshotTrack >= 1)
	{
		FinishScreenshotting();
	}
	else
	{
		Pawn->TrackRider->SetTrackPosition(LastScreenshotTrack);
		CreateScreenshot();
	}
}

void AFSPRecorder::CreateScreenshot()
{
	// Analyze scene and record scene
	//const FString Command = "HighResShot 2";
	//GetWorld()->Exec(GetWorld(), *Command);
	GetHighResScreenshotConfig().SetResolution(ScreenshotWidth, ScreenshotHeight);
	//GetHighResScreenshotConfig().FilenameOverride = FString("new filename");
	FScreenshotRequest::RequestScreenshot(false);
	OnScreenshotTaken.Broadcast();
	UE_LOG(LogTemp, Display, TEXT("Screenshot taken"));
}

void AFSPRecorder::StopScreenshotting() 
{
	if(!bIsScreenshotting) return;
	bIsScreenshotting = false;
	OnScreenshotsStopped.Broadcast();
	FinishScreenshotting();
}

void AFSPRecorder::FinishScreenshotting()
{
	GetWorldTimerManager().ClearTimer(ScreenshottingHandle);
	Pawn->TrackRider->ShowControls(true);
}
