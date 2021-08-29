// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPLogger.h"

// Sets default values
AFSPLogger::AFSPLogger()
{
    SceneAnalysisLog = CreateDefaultSubobject<UFSPLog>(TEXT("SceneAnalysisLog"));
    PositionLog = CreateDefaultSubobject<UFSPLog>(TEXT("PositionLog"));
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
		static_cast<float>(LoggingFrequency/1000), true);
	return true;
}

void AFSPLogger::StopLoggingPosition()
{
	//close log
	IsLoggingPosition = false;
	ObjectBeingTracked = nullptr;
	GetWorldTimerManager().ClearTimer(PositionLoggingHandle);
}

void AFSPLogger::LogPosition(AActor* Object) const
{
	if(!PositionLog->IsLogOpen())
	{
		CreatePositionLog();
	}
	FVector const Location = Object->GetActorLocation();
	PositionLog->WriteMessage(LocationToString(Location));
}

bool AFSPLogger::LogSceneAnalysis(TMap<FName, int32> Results)
{
	if(!SceneAnalysisLog->IsLogOpen())
	{
		CreateSceneAnalysisLog();
	}
	for (const TPair<FName, int32>& pair : Results)
	{
		TArray<FString> Arr;
		Arr.Add(pair.Key.ToString());
		Arr.Add(FString::Printf(TEXT("%d"),pair.Value));
		bool completed = SceneAnalysisLog->WriteArray(Arr);
	}
	return true;
}

void AFSPLogger::LogItemsPositions(TArray<UFSPObject*> Objects) const
{
	PositionLog->CreateFile("item_positions");
	for (auto Object : Objects)
	{
		TArray<FString> Arr;
		Arr.Add(Object->ObjectName.ToString());
		Arr.Add(LocationToString(Object->GetOwner()->GetActorLocation()));
		PositionLog->WriteArray(Arr);
	}
	PositionLog->CloseFile();
}

FString AFSPLogger::LocationToString(FVector Location) const
{
	const FString Out = FString::Printf(TEXT("(%.4f,%.4f,%.4f)"), Location.X, Location.Y, Location.Z);
	return Out;
}

void AFSPLogger::CreatePositionLog() const
{
	PositionLog->CreateFile(TEXT("position"));
}

void AFSPLogger::CreateSceneAnalysisLog() const
{
	SceneAnalysisLog->CreateFile(TEXT("scene"));
}
