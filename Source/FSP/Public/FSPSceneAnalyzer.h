// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "FSPObject.h"
#include "FSPPawn.h"
#include "FSPSceneAnalyzer.generated.h"

//Forward declaring recorder
class AFSPRecorder;

UCLASS( ClassGroup=(FSP), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPSceneAnalyzer : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UFSPSceneAnalyzer();

	void SetRecorder(AFSPRecorder* RecorderOrig);
	
	/**Analyses scene and counts hwat portion of a screen each FSP object takes with given precision.
	  * @param Player pointer to a player object from which the raytracing originates. The player needs to have a valid camera
	  * @param Precision how many rays to shoot (
	  * @param DrawHits debug purpose, set true to visualise hits of UFSPObjects
	  * @param DrawDebug draws the raycasted rays
	  * @return TMap with an object and int32 describing how many rays have struck the object
	**/
	UFUNCTION(BlueprintCallable, Category="FSP|SceneAnalysis", BlueprintPure=false, CallInEditor)
	TMap<FName, int32> AnalyzeScene(AFSPPawn* Player, int32 Precision, bool DrawHits = false,
		bool DrawDebug = false, bool PrintReport = false) const;

	UFUNCTION(BlueprintCallable, Category="FSP|SceneAnalysis", BlueprintPure=false, CallInEditor)
	TMap<FName, int32> AnalyzeSceneTracing(APlayerController* Player, int32 Precision, bool DrawHits = false,
		bool DrawDebug = false, bool PrintReport = false) const;

	UFUNCTION(BlueprintCallable, Category="FSP|SceneAnalysis", BlueprintPure=false, CallInEditor)
	TMap<FName, int32> AnalyzeSceneScreenRadius(AFSPPawn* Player, bool PrintReport = false) const;
	
	UFUNCTION(BlueprintCallable, Category="FSP|SceneAnalysis", CallInEditor)
	void TestAnalyzeScene() const;	
	
	UFUNCTION(BlueprintCallable, Category="FSP|SceneAnalysis")
	void GetScreenPosition(AFSPPawn* Player, UFSPObject* Object, FVector2D& Out) const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="FSP|SceneAnalysis")
	bool bDrawHits = false;
	
	UPROPERTY(EditAnywhere, Category="FSP|SceneAnalysis")
	bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, Category="FSP|SceneAnalysis")
	float DebugLineThickness = 0.1;
	
	UPROPERTY(EditAnywhere, Category="FSP|SceneAnalysis")
	bool bPrintHits = false;
	
	UPROPERTY(EditAnywhere, Category="FSP|SceneAnalysis")
	float ScanDistance = 1000;

	UPROPERTY(EditAnywhere, Category="FSP|SceneAnalysis")
	int32 DefaultPrecision = 32;

private:
	void DoTraceFromScreen(float X, float Y, float Distance, bool DrawHits, bool DrawDebug,
		APlayerController* Player, FName& HitName) const;
	static float GetObjectScreenRadius(AActor* InActor, AFSPPawn* Player);

	UPROPERTY(VisibleAnywhere, Category="FSP|SceneAnalysis")
	AFSPRecorder* Recorder;
};
