// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FSPTrack.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "Math/UnrealMathUtility.h"
#include "FSPTrackRider.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTrackRiderStateChanged);

class UFSPTrackRiderWidget;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPTrackRider : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSPTrackRider();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsMoving = true;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="FSP")
	float TrackPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP")
	bool bMoveAutomatically;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP")
	float Duration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP")
	AFSPTrack* Track;

	UPROPERTY(EditAnywhere, Category = "FSP|UI")
	TSubclassOf<class UUserWidget> WidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSP|UI")
	UFSPTrackRiderWidget* WidgetControl;

	UFUNCTION(BlueprintCallable, Category = "FSP")
	void SwitchMoving();

	UFUNCTION(BlueprintCallable, Category = "FSP")
	void StartMoving();

	UFUNCTION(BlueprintCallable, Category = "FSP")
	void StopMoving();

	UFUNCTION(BlueprintCallable, Category = "FSP")
	void ShowControls(bool b);

	UFUNCTION(BlueprintCallable, Category = "FSP")
	void SetTrackPosition(float TrackRatio);

	// This function automatically updates the position on track
	UFUNCTION(BlueprintCallable, Category = "FSP")
	void UpdateTrackPosition(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "FSP")
	FVector GetSplinePosition(float TrackRatio) const;

	UFUNCTION(BlueprintCallable, Category = "FSP")
	FRotator GetSplineRotation(float trackRatio) const;

	UPROPERTY(BlueprintAssignable, Category=Callback)
	FTrackRiderStateChanged OnTrackFinished;

private:
	void GetSplinePositionAndRotation(float TrackRatio, FVector& Position, FRotator& Rotation) const;
};
