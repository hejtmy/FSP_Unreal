// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "FSPTrackRider.h"
#include "FSPTrackRiderWidget.generated.h"

/**
 * 
 */
UCLASS()
class FSP_API UFSPTrackRiderWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere)
	UFSPTrackRider* TrackRider;
	
	UPROPERTY(meta = (BindWidget))
	USlider* SliderTrackPosition;

	UPROPERTY(meta = (BindWidget))
	USlider* SliderDuration;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonGo;


private:
	UFUNCTION()
	void MoveOnTrack(float TrackPosition);

	UFUNCTION()
	void SetDuration(float Duration);

	UFUNCTION()
	void SwitchGo();
};
