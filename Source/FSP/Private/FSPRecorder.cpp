// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPRecorder.h"

#include "HighResScreenshot.h"

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
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Object manager is not assigned to the Recorder. Cannot log object positions."));
	}
	
	GetWorldTimerManager().SetTimer(SceneAnalysisLoggingHandle, this, &AFSPRecorder::LogSceneAnalysis,
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

void AFSPRecorder::LogSceneAnalysis()
{
	UE_LOG(LogTemp, Display, TEXT("Data have been logged"));
	Logger->LogPosition(Pawn, iSceneRecording);
	const TMap<FName, int32> Results = SceneAnalyzer->AnalyzeScene(Cast<APlayerController>(Pawn->Controller), Precision);
	Logger->LogSceneAnalysis(Results, iSceneRecording);
	iSceneRecording += 1;
}

void AFSPRecorder::LogScreenshotPlayerPosition()
{
	TArray<FString> Messages;
	Messages.Add(FString::FromInt(iSceneRecording));
	Messages.Add(Logger->LocationToString(Pawn->GetActorLocation()));
	Logger->WriteToPositionLog(Messages);
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
	
	ResetSceneRecordingIndex();
	this->Logger = Logging;

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

	OnScreenshotTaken.AddDynamic(this, &AFSPRecorder::LogSceneAnalysis);
	OnScreenshotTaken.AddDynamic(this, &AFSPRecorder::LogScreenshotPlayerPosition);
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
	OnScreenshotsStopped.Broadcast();
	UE_LOG(LogTemp, Display, TEXT("Screenshot stopped"));
	FinishScreenshotting();
}

void AFSPRecorder::FinishScreenshotting()
{
	bIsScreenshotting = false;
	UE_LOG(LogTemp, Display, TEXT("Screenshot finished"));
	GetWorldTimerManager().ClearTimer(ScreenshottingHandle);
	OnScreenshotTaken.RemoveDynamic(this, &AFSPRecorder::LogSceneAnalysis);
	OnScreenshotTaken.RemoveDynamic(this, &AFSPRecorder::LogScreenshotPlayerPosition);
	Pawn->TrackRider->ShowControls(true);

}
