// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatformeComponent.h"
#include "Runtime/Engine/Public/EngineGlobals.h"

// Sets default values for this component's properties
UMovingPlatformeComponent::UMovingPlatformeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	XOffset = 50.0f;
	YOffset = 0.0f;
	ZOffset = 0.0f;
}


// Called when the game starts
void UMovingPlatformeComponent::BeginPlay()
{
	Super::BeginPlay();

	MovingActor = GetOwner();

	if (fCurve && MovingActor) {
		Timeline->AddInterpFloat(fCurve, InterpFunction, FName("Alpha"));
		Timeline->SetTimelineFinishedFunc(TimelineFinished);

		StartLocation = MovingActor->GetActorLocation();
		EndLocation = FVector(StartLocation.X + XOffset, StartLocation.Y + YOffset, StartLocation.Z + ZOffset);

		Timeline->SetLooping(false);
		Timeline->SetIgnoreTimeDilation(true);

		Timeline->Play();
	}
	
}


// Called every frame
void UMovingPlatformeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMovingPlatformeComponent::TimelineFloatReturn(float fval)
{
	MovingActor->SetActorLocation(FMath::Lerp(StartLocation, EndLocation, fval));
}

void UMovingPlatformeComponent::OnTimelineFinished()
{
	if (Timeline->GetPlaybackPosition() == 0.0f) {
		Timeline->Play();
	} else if (Timeline->GetPlaybackPosition() == Timeline->GetTimelineLength()) {
		Timeline->Reverse();
	}
}

