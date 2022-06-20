// Fill out your copyright notice in the Description page of Project Settings.

#include "FSPRecorder.h"

#include "HighResScreenshot.h"
#include "CameraTrack/FSPTrackRider.h"

// Sets default values for this component's properties
AFSPRecorder::AFSPRecorder()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneAnalyzer = CreateDefaultSubobject<UFSPSceneAnalyzer>(TEXT("SceneAnalyzer"));
}

// Called when the game starts
void AFSPRecorder::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFSPRecorder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFSPRecorder::StartRecording(AFSPLogger* Logging)
{
	UFSPTrackRider* Rider = Pawn->TrackRider;
    Rider->SetTrackPosition(0);
    Rider->ShowControls(false);
    Rider->OnTrackFinished.AddUniqueDynamic(this, &AFSPRecorder::FinishRecording);
	this->Logger = Logging;
	ResetSceneRecordingIndex();

	if(ObjectManager != nullptr)
	{
		Logger->LogObjectsPositions(ObjectManager->GetObjects());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Object manager is not assigned to the Recorder. Cannot log object positions."));
	}
	
	GetWorldTimerManager().SetTimer(SceneAnalysisLoggingHandle, this, &AFSPRecorder::LogSceneData,
		1/static_cast<float>(SceneAnalysisLoggingFrequency), true);
    Rider->StartMoving();
	UE_LOG(LogTemp, Display, TEXT("Recording started"));
}

void AFSPRecorder::StartRecordingWithoutLogging()
{
	UFSPTrackRider* Rider = Pawn->TrackRider;
    Rider->SetTrackPosition(0);
    Rider->ShowControls(false);
    Rider->OnTrackFinished.AddUniqueDynamic(this, &AFSPRecorder::FinishRecording);
    Rider->StartMoving();
}

void AFSPRecorder::LogSceneData()
{
	Logger->LogPosition(Pawn, iSceneRecording);
	LogSceneAnalysis();
	LogObjectPositions();
	
	UE_LOG(LogTemp, Display, TEXT("Data have been logged"));
	iSceneRecording += 1;
}

void AFSPRecorder::LogSceneAnalysis()
{
	const TMap<FName, int32> Results = SceneAnalyzer->AnalyzeScene(Cast<APlayerController>(Pawn->Controller), Precision);
	Logger->LogSceneAnalysis(Results, iSceneRecording);
}

void AFSPRecorder::LogObjectPositions()
{
	TArray<UFSPObject*> Objects;
	if(ObjectManager != nullptr)
	{
		Objects = ObjectManager->GetObjects();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Object manager is not assigned to the Recorder. Cannot log object positions."));
		return;
	}
	for(UFSPObject* Obj : Objects)
	{
		// gets the positiosn
		FVector2D Out;
		SceneAnalyzer->GetScreenPosition(Cast<APlayerController>(Pawn->Controller), Obj, Out);
		// logs the position
		FString Name = Obj->ObjectName.ToString();
		Logger->LogObjectScreenPosition(Out, Name, iSceneRecording);
	}
}

void AFSPRecorder::StopRecording()
{
	UE_LOG(LogTemp, Display, TEXT("Recording Stopped"));
	FinishRecording();
}

void AFSPRecorder::FinishRecording()
{
	UFSPTrackRider* Rider = Pawn->TrackRider;
	UE_LOG(LogTemp, Display, TEXT("Recording finished"));
	Rider->StopMoving();
	Rider->ShowControls(true);
	Rider->OnTrackFinished.RemoveDynamic(this, &AFSPRecorder::FinishRecording);
	GetWorldTimerManager().ClearTimer(SceneAnalysisLoggingHandle);
	Logger = nullptr;
}

void AFSPRecorder::ResetSceneRecordingIndex()
{
	iSceneRecording = 0;
}

void AFSPRecorder::CreateScreenshots(AFSPLogger* Logging)
{
	UE_LOG(LogTemp, Display, TEXT("Starting screenshotting"));
	if(bIsScreenshotting)
	{
		UE_LOG(LogTemp, Display, TEXT("Screenshots are already underway"));
		return;
	}
	//Check if Pawn has been assigned
	Pawn->TrackRider->StopMoving();
	Pawn->TrackRider->ShowControls(false);
	
	LastScreenshotTrack = 0.0;

	// This needs to be assigned as this function 
	this->Logger = Logging;
	if(ObjectManager != nullptr)
	{
		Logger->LogObjectsPositions(ObjectManager->GetObjects());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Object manager is not assigned to the Recorder. Cannot log object positions."));
	}
	
	ResetSceneRecordingIndex();

	OnScreenshotTaken.AddDynamic(this, &AFSPRecorder::LogSceneData);
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
	GetHighResScreenshotConfig().SetResolution(ScreenshotWidth, ScreenshotHeight);
	if(this->bSaveScreenshots)
	{
		FScreenshotRequest::RequestScreenshot(false);
	}
	OnScreenshotTaken.Broadcast();
	UE_LOG(LogTemp, Display, TEXT("Screenshot taken"));
}

void AFSPRecorder::StopScreenshotting() 
{
	if(!bIsScreenshotting) return;
	OnScreenshotsStopped.Broadcast();
	UE_LOG(LogTemp, Display, TEXT("Screenshot stopped"));
	FinishScreenshotting();
}

void AFSPRecorder::FinishScreenshotting()
{
	bIsScreenshotting = false;
	UE_LOG(LogTemp, Display, TEXT("Screenshot finished"));
	GetWorldTimerManager().ClearTimer(ScreenshottingHandle);
	OnScreenshotTaken.RemoveDynamic(this, &AFSPRecorder::LogSceneData);
	Pawn->TrackRider->ShowControls(true);
}
