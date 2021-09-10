// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPLogger.h"

// Sets default values
AFSPLogger::AFSPLogger()
{
    SceneAnalysisLog = CreateDefaultSubobject<UFSPLog>(TEXT("SceneAnalysisLog"));
    PositionLog = CreateDefaultSubobject<UFSPLog>(TEXT("PositionLog"));
    ScreenPositionLog = CreateDefaultSubobject<UFSPLog>(TEXT("ScreenPositionLog"));
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;
}

// Called when the game starts or when spawned
void AFSPLogger::BeginPlay()
{
   Super::BeginPlay();
}

bool AFSPLogger::StartLoggingPosition(AActor* Object)
{
	if(IsLoggingPosition)
	{
		UE_LOG(LogTemp, Warning, TEXT("Logging is already under progress"));
		return false;
	}
	IsLoggingPosition = true;
	ObjectBeingTracked = Object;
	
	//prepare log
	CreatePositionLog();
	FTimerDelegate PositionLoggingDelegate;

	PositionLoggingDelegate.BindUFunction(this, FName("LogPosition"), ObjectBeingTracked);
	GetWorldTimerManager().SetTimer(PositionLoggingHandle, PositionLoggingDelegate,
		1/static_cast<float>(LoggingFrequency), true);
	return true;
}

void AFSPLogger::StopLoggingPosition()
{
	//close log
	IsLoggingPosition = false;
	ObjectBeingTracked = nullptr;
	GetWorldTimerManager().ClearTimer(PositionLoggingHandle);
}

void AFSPLogger::WriteToPositionLog(TArray<FString> Messages) const
{
	if(!PositionLog->IsLogOpen())
	{
		CreatePositionLog();
	}

	PositionLog->WriteArray(Messages);
}

void AFSPLogger::LogPosition(AActor* Object, int32 iAnalysis) const
{
	FVector const Location = Object->GetActorLocation();
	WriteToPositionLog(TArray<FString>{FString::FromInt(iAnalysis), LocationToString(Location)});
}

bool AFSPLogger::LogSceneAnalysis(TMap<FName, int32> Results, int32 iAnalysis)
{
	if(!SceneAnalysisLog->IsLogOpen())
	{
		CreateSceneAnalysisLog();
	}
	for (const TPair<FName, int32>& pair : Results)
	{
		TArray<FString> Arr;
		Arr.Add(FString::FromInt(iAnalysis));
		Arr.Add(pair.Key.ToString());
		Arr.Add(FString::Printf(TEXT("%d"), pair.Value));
		bool completed = SceneAnalysisLog->WriteArray(Arr);
	}
	return true;
}

void AFSPLogger::LogObjectsPositions(TArray<UFSPObject*> Objects) const
{
	PositionLog->CreateFile("object-positions");
	PositionLog->WriteArray(TArray<FString>{"object", "position"},
	true, TEXT(";"), false);
	for (auto Object : Objects)
	{
		TArray<FString> Arr;
		Arr.Add(Object->ObjectName.ToString());
		Arr.Add(LocationToString(Object->GetOwner()->GetActorLocation()));
		PositionLog->WriteArray(Arr, true, TEXT(";"), false);
	}
	PositionLog->CloseFile();
}

void AFSPLogger::LogObjectScreenPosition(FVector2D& Position, FString& Name, int32 iAnalysis)
{
	if(!ScreenPositionLog->IsLogOpen())
	{
		CreateScreenPositionLog();
	}
	TArray<FString> Arr;
	Arr.Add(FString::FromInt(iAnalysis));
	Arr.Add(Name);
	Arr.Add(FString::Printf(TEXT("%.4f"), Position.X));
	Arr.Add(FString::Printf(TEXT("%.4f"), Position.Y));
	ScreenPositionLog->WriteArray(Arr);
}

FString AFSPLogger::LocationToString(FVector Location)
{
	const FString Out = FString::Printf(TEXT("(%.4f,%.4f,%.4f)"), Location.X, Location.Y, Location.Z);
	return Out;
}

FString AFSPLogger::ScreenPositionToString(FVector2D& Position)
{
	const FString Out = FString::Printf(TEXT("(%.4f,%.4f)"), Position.X, Position.Y);
	return Out;
}

void AFSPLogger::CreatePositionLog() const
{
	PositionLog->CreateFile(TEXT("position"));
	// ADD header
	PositionLog->WriteArray(TArray<FString>{"time", "iAnalysis", "position"}, 
	true, TEXT(";"), false);
}

void AFSPLogger::CreateSceneAnalysisLog() const
{
	SceneAnalysisLog->CreateFile(TEXT("scene-analysis"));
	SceneAnalysisLog->WriteArray(TArray<FString>{"time", "iAnalysis", "object", "nHits"},
	true, TEXT(";"), false);
}

void AFSPLogger::CreateScreenPositionLog() const
{
	ScreenPositionLog->CreateFile(TEXT("screen-position"));
	ScreenPositionLog->WriteArray(TArray<FString>{"time", "iAnalysis", "object", "screen_x", "screen_y"}, 
	true, TEXT(";"), false);
}
