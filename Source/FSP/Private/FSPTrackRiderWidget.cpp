// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPTrackRiderWidget.h"

void UFSPTrackRiderWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ButtonGo->OnClicked.AddUniqueDynamic(this, &UFSPTrackRiderWidget::SwitchGo);
	SliderDuration->OnValueChanged.AddUniqueDynamic(this, &UFSPTrackRiderWidget::SetDuration);
	SliderTrackPosition->OnValueChanged.AddUniqueDynamic(this, &UFSPTrackRiderWidget::MoveOnTrack);
}

void UFSPTrackRiderWidget::MoveOnTrack(float TrackPosition)
{
	TrackRider->SetTrackPosition(TrackPosition);
	UE_LOG(LogTemp, Display, TEXT("Track position changed to %s"), *FString::SanitizeFloat(TrackPosition));
}

void UFSPTrackRiderWidget::SetDuration(float Duration)
{
	TrackRider->Duration = Duration;
	UE_LOG(LogTemp, Display, TEXT("Track position changed to %s"), *FString::SanitizeFloat(Duration));
}

void UFSPTrackRiderWidget::SwitchGo()
{
	TrackRider->SwitchMoving();
}
