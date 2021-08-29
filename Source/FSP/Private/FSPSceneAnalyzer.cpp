// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPSceneAnalyzer.h"

#include "FSPObject.h"

// Sets default values
UFSPSceneAnalyzer::UFSPSceneAnalyzer()
{
	
}

// Called when the game starts or when spawned
void UFSPSceneAnalyzer::BeginPlay()
{
	Super::BeginPlay();
}

TMap<FName, int32> UFSPSceneAnalyzer::AnalyzeScene(APlayerController* Player, int32 Precision,
	bool DrawHits, bool DrawDebug) const
{
	TMap<FName, int32> Out;

	Out.Add(TEXT("nothing"), 1);
	Precision = FMath::Clamp<int32>(Precision, 1, 100);
	float CurrentScanX = 0;
	float CurrentScanY = 0;
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
	
	//while(CurrentScanX < ViewportSize.X)
	for(int32 i = 0; i < 10; i++)
	{
		for(int32 j = 0; j < 10; j++)
		//while(CurrentScanY < ViewportSize.Y)
		{
			FName HitName;
			DoTraceFromScreen(CurrentScanX, CurrentScanY, ScanDistance, DrawHits, DrawDebug, Player, HitName);
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
		}
		CurrentScanY = 0;
		CurrentScanX += ViewportSize.X/static_cast<float>(Precision);
	}
	return Out;
}

void UFSPSceneAnalyzer::GetScreenPosition(APlayerController* Player, UFSPObject* Object, FVector2D& Out) const
{
	const FVector Position = Object->GetOwner()->GetActorLocation();
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	if(Player->ProjectWorldLocationToScreen(Position, Out))
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
		false, 1, 0, 1);
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
		false, 1, 0, 1);
	}
}
