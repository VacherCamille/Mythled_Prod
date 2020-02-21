// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatingComponent.h"
#include "Runtime/Engine/Public/EngineGlobals.h"

// Sets default values for this component's properties
URotatingComponent::URotatingComponent()
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
void URotatingComponent::BeginPlay()
{
	Super::BeginPlay();

	RotatingActor = GetOwner();

	if (fCurve && RotatingActor) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("Animation"));
		Timeline->AddInterpFloat(fCurve, InterpFunction, FName("Alpha"));
		Timeline->SetTimelineFinishedFunc(TimelineFinished);

		StartLocation = RotatingActor->GetActorRotation();
		EndLocation = FRotator(StartLocation.Roll + XOffset, StartLocation.Pitch + YOffset, StartLocation.Yaw + ZOffset);

		Timeline->SetLooping(false);
		Timeline->SetIgnoreTimeDilation(true);

		Timeline->Play();
	}
	
}


// Called every frame
void URotatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URotatingComponent::TimelineFloatReturn(float fval)
{
	RotatingActor->SetActorRotation(FMath::Lerp(StartLocation, EndLocation, fval));
}

void URotatingComponent::OnTimelineFinished()
{
	if (Timeline->GetPlaybackPosition() == 0.0f) {
		Timeline->Play();
	} else if (Timeline->GetPlaybackPosition() == Timeline->GetTimelineLength()) {
		Timeline->Reverse();
	}
}

