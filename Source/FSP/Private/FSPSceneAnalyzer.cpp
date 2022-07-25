// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPSceneAnalyzer.h"

#include "FSPObject.h"

// Sets default values
UFSPSceneAnalyzer::UFSPSceneAnalyzer()
{
}

void UFSPSceneAnalyzer::SetRecorder(AFSPRecorder* RecorderOrig)
{
	Recorder = RecorderOrig;
}

// Called when the game starts or when spawned
void UFSPSceneAnalyzer::BeginPlay()
{
	Super::BeginPlay();
}

TMap<FName, int32> UFSPSceneAnalyzer::AnalyzeScene(AFSPPawn* Player, int32 Precision,
	bool DrawHits, bool DrawDebug, bool PrintReport) const
{
	return AnalyzeSceneTracing(Player->PlayerController(), Precision, DrawHits, DrawDebug, PrintReport);
}

TMap<FName, int32> UFSPSceneAnalyzer::AnalyzeSceneTracing(APlayerController* Player, int32 Precision,
	bool DrawHits, bool DrawDebug, bool PrintReport) const
{
	TMap<FName, int32> Out;

	Out.Add(TEXT("nothing"), 1);
	Precision = FMath::Clamp<int32>(Precision, 1, 100);
	FVector2D ViewportSize;

	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	else
	{
		return Out;
	}

	UE_LOG(LogTemp,	Display, TEXT("%s"), *ViewportSize.ToString());
	
	float CurrentScanX = 0;
	float CurrentScanY = 0;
	//while(CurrentScanX < ViewportSize.X)
	for(int32 i = 0; i <= Precision; i++)
	{
		for(int32 j = 0; j <= Precision; j++)
			//while(CurrentScanY < ViewportSize.Y)
			{
			FName HitName;
			DoTraceFromScreen(CurrentScanX, CurrentScanY, ScanDistance, bDrawHits, bDrawDebug, Player, HitName);
			if(HitName.IsNone())
			{
				Out.Add(FName(TEXT("nothing")), Out[FName(TEXT("nothing"))] + 1);
			}
			else
			{
				int32 Value = Out.Contains(HitName) ? Value = Out[HitName] + 1 : 1;
				Out.Add(HitName, Value);
			}
			CurrentScanY += ViewportSize.Y/static_cast<float>(Precision);
			CurrentScanY = FMath::Clamp<float>(CurrentScanY, 0, ViewportSize.Y);
			}
		CurrentScanY = 0;
		CurrentScanX += ViewportSize.X/static_cast<float>(Precision);
		CurrentScanX = FMath::Clamp<float>(CurrentScanX, 0, ViewportSize.X);
	}
	
	if(PrintReport)
	{
		for (auto Result : Out)
		{
			UE_LOG(LogTemp, Display, TEXT("%s:%d"), *Result.Key.ToString(), Result.Value);
		}
	}
	return Out;
}

TMap<FName, int32> UFSPSceneAnalyzer::AnalyzeSceneScreenRadius(AFSPPawn* Player, bool PrintReport) const
{
	TMap<FName, int32> Out;
	auto Objects = Recorder->ObjectManager->GetObjects();
	for (const auto Result : Objects)
	{
		const float Radius = GetObjectScreenRadius(Result->GetOwner(), Player);
		Out.Add(Result->ObjectName, Radius);
	}
	
	if(PrintReport)
	{
		for (auto Result : Out)
		{
			UE_LOG(LogTemp, Display, TEXT("%s:%d"), *Result.Key.ToString(), Result.Value);
		}
	}
	return Out;
}

void UFSPSceneAnalyzer::TestAnalyzeScene() const
{
	APlayerController* Player = GetWorld()->GetFirstPlayerController();
	AnalyzeSceneTracing(Player, this->DefaultPrecision, this->bDrawHits, this->bDrawDebug, this->bPrintHits);
}

void UFSPSceneAnalyzer::GetScreenPosition(AFSPPawn* Player, UFSPObject* Object, FVector2D& Out) const
{
	const FVector Position = Object->GetOwner()->GetActorLocation();
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	if(Player->PlayerController()->ProjectWorldLocationToScreen(Position, Out))
	{
		Out = Out/ViewportSize;
	}
	else
	{
		Out = FVector2D(-9999,-9999);
	}
}

void UFSPSceneAnalyzer::DoTraceFromScreen(float X, float Y, float Distance, bool DrawHits, bool DrawDebug,
                                          APlayerController* Player, FName& HitName) const
{
	FVector WorldLocation;
	FVector WorldRotation;

	Player->DeprojectScreenPositionToWorld(X, Y, WorldLocation, WorldRotation);
	
	FVector TraceEnd = (WorldRotation * Distance) + WorldLocation;
	FHitResult HitObject;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("SceneTrace")));
	TraceParams.bTraceComplex = false;

	if(DrawDebug)
	{
		DrawDebugLine(GetWorld(), WorldLocation, TraceEnd, FColor::Red,
		false, 0.5, 0, DebugLineThickness);
	}

	if(!GetWorld()->LineTraceSingleByChannel(HitObject, WorldLocation, TraceEnd,
		ECC_Visibility, TraceParams)) return;

	if(!HitObject.bBlockingHit) return;
	UActorComponent* Object = HitObject.Actor->GetComponentByClass(UFSPObject::StaticClass());
	if(!Object) return;
	HitName = Cast<UFSPObject>(Object)->ObjectName;
	if(DrawHits)
	{
		DrawDebugLine(GetWorld(), WorldLocation, TraceEnd, FColor::Green,
		false, 0.5, 0, DebugLineThickness);
	}
}

float UFSPSceneAnalyzer::GetObjectScreenRadius(AActor* InActor, AFSPPawn* Player)
{
	float ScreenRadius;
	int32 Width, Height;
	FVector ViewLocation;
	FRotator ViewRotation; // Not Used, but required for Function call
	float CamFOV = Player->Camera->FieldOfView; 
	/* Get the size of the viewport, and the player cameras location. */
	Player->PlayerController()->GetViewportSize(Width, Height);
	Player->PlayerController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
#if WITH_EDITOR
	float ScreenPerc = IConsoleManager::Get().FindTConsoleVariableDataFloat(TEXT("r.SCreenPercentage"))->
		GetValueOnGameThread() / 100.0f;
	Width *= ScreenPerc;
	Height *= ScreenPerc;
#endif WITH_EDITOR

	/* Easy Way To Return The Size, Create a vector and scale it. Alternative would be to use FMath::Max3 */
	float SRad = FVector2D(Width, Height).Size();
	float BoundingRadius = InActor->GetRootComponent()->Bounds.SphereRadius;
	float DistanceToObject = FVector(InActor->GetActorLocation() - ViewLocation).Size();

	/* Get Projected Screen Radius */
	ScreenRadius = FMath::Atan(BoundingRadius / DistanceToObject);
	ScreenRadius *= SRad / FMath::DegreesToRadians(CamFOV);
	return ScreenRadius;
}