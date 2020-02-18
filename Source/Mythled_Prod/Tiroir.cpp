// Fill out your copyright notice in the Description page of Project Settings.

#include "Tiroir.h"

// Sets default values
ATiroir::ATiroir()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	XOffset = 100.f;

}

// Called when the game starts or when spawned
void ATiroir::BeginPlay()
{
	Super::BeginPlay();

	if (fCurve) {
		Timeline->AddInterpFloat(fCurve, InterpFunction, FName("Alpha"));
		Timeline->SetTimelineFinishedFunc(TimelineFinished);

		StartLocation = GetActorLocation();
		EndLocation = FVector(StartLocation.X + XOffset, StartLocation.Y, StartLocation.Z);

		Timeline->SetLooping(false);
		Timeline->SetIgnoreTimeDilation(true);
	}
	
}

// Called every frame
void ATiroir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATiroir::TimelineFloatReturn(float fval)
{
	SetActorLocation(FMath::Lerp(StartLocation, EndLocation, fval));
}

void ATiroir::OnTimelineFinished()
{

}

void ATiroir::Activate() {
	if (!isActivated) {
		Timeline->Play();
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Activated")));
		isActivated = true;
	}
}

void ATiroir::Desactivate() {
	if (isActivated) {
		Timeline->Reverse();
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Desactivated")));
		isActivated = false;
	}
}