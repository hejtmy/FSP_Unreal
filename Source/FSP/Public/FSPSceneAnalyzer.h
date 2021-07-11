// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "FSPObject.h"

#include "FSPSceneAnalyzer.generated.h"

UCLASS( ClassGroup=(FSP), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPSceneAnalyzer : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UFSPSceneAnalyzer();

	UFUNCTION(BlueprintCallable, Category="FSP")
	TMap<FName, int32> AnalyzeScene(APlayerController* Player, int32 Precision, bool DrawHits = false, bool DrawDebug = false) const;

	UFUNCTION(BlueprintCallable, Category="FSP|SceneAnalysis")
	void GetScreenPosition(APlayerController* Player, UFSPObject* Object, FVector2D& Out) const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="FSP")
	float ScanDistance = 1000;

private:
	void DoTraceFromScreen(float X, float Y, float Distance, bool DrawHits, bool DrawDebug, APlayerController* Player, FName& HitName) const;
};
