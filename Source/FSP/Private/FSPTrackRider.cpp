// Fill out your copyright notice in the Description page of Project Settings.

#include "FSPTrackRider.h"

#include "FSPTrackRiderWidget.h"

// Sets default values for this component's properties
UFSPTrackRider::UFSPTrackRider()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UFSPTrackRider::BeginPlay()
{
	Super::BeginPlay();
	bIsMoving = false;

	/** Make sure asset was loaded and class was set */
	if( !WidgetClass )
	{
		return;
	}
	/** Create the widget of our UUserWidget type (UGUserWidget) from the class we loaded from the Content Browser */
	WidgetControl = CreateWidget<UFSPTrackRiderWidget>( GetWorld( ), WidgetClass );

	/** Make sure widget was created */
	if( WidgetControl ) 
	{
		WidgetControl->AddToViewport( );
		WidgetControl->TrackRider = this;
	}
}

// Called every frame
void UFSPTrackRider::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!bMoveAutomatically) return;
	SetTrackPosition(TrackPosition);
	UpdateTrackPosition(DeltaTime);
}

void UFSPTrackRider::SwitchMoving()
{
	bMoveAutomatically = !bMoveAutomatically;
}

void UFSPTrackRider::StartMoving()
{
	bMoveAutomatically = true;
}

void UFSPTrackRider::StopMoving()
{
	bMoveAutomatically = false;
}

void UFSPTrackRider::ShowControls(bool b)
{
	if(!WidgetControl) return;
	if(b)
	{
		WidgetControl->AddToViewport();
	}
	else
	{
		WidgetControl->RemoveFromParent();
	}
}

void UFSPTrackRider::SetTrackPosition(float TrackRatio)
{
	TrackRatio = FMath::Clamp<float>(TrackRatio, 0, 1);

	FVector Location;
	FRotator Rotation;
	GetSplinePositionAndRotation(TrackRatio, Location, Rotation);

	this->GetOwner()->SetActorLocationAndRotation(Location, Rotation);
	this->TrackPosition = TrackRatio;
}

void UFSPTrackRider::UpdateTrackPosition(const float DeltaTime)
{
	TrackPosition += DeltaTime/Duration;
	if(TrackPosition >= 1)
	{
		OnTrackFinished.Broadcast();
		TrackPosition = 0;
	}
}

FVector UFSPTrackRider::GetSplinePosition(float TrackRatio) const
{
	const float Distance = FMath::Lerp<float, float>(0, Track->Track->GetSplineLength(), TrackRatio);
	const FVector Out = Track->Track->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	return Out;
}

FRotator UFSPTrackRider::GetSplineRotation(float TrackRatio) const
{
	const float Distance = FMath::Lerp<float, float>(0, Track->Track->GetSplineLength(), TrackRatio);
	const FRotator Out = Track->Track->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	return Out;
}

void UFSPTrackRider::GetSplinePositionAndRotation(float TrackRatio, FVector& Position, FRotator& Rotation) const
{
	const float Distance = FMath::Lerp<float, float>(0, Track->Track->GetSplineLength(), TrackRatio);
	Position = Track->Track->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	Rotation = Track->Track->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
}