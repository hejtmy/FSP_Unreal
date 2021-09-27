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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="FSP Values")
	UFSPTrackRider* TrackRider;
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	USlider* SliderTrackPosition;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USlider* SliderDuration;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonGo;

public:
	UFUNCTION()
	void MoveOnTrack(float TrackPosition);

	UFUNCTION()
	void SetDuration(float Duration);

	UFUNCTION()
	void SwitchGo();

	UFUNCTION(BlueprintGetter)
	float CurrentSpeed() const;
	
};
