// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(Root);

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	XOffset = 100.f;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (fCurve) {
		Timeline->AddInterpFloat(fCurve, InterpFunction, FName("Alpha"));
		Timeline->SetTimelineFinishedFunc(TimelineFinished);

		StartLocation = GetActorLocation();
		EndLocation = FVector(StartLocation.X + XOffset, StartLocation.Y + YOffset, StartLocation.Z + ZOffset);

		Timeline->SetLooping(false);
		Timeline->SetIgnoreTimeDilation(true);
	}

}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::TimelineFloatReturn(float fval)
{
	SetActorLocation(FMath::Lerp(StartLocation, EndLocation, fval));
}

void ADoor::OnTimelineFinished()
{

}

void ADoor::OpenDoor()
{
	Timeline->Play();
}

void ADoor::CloseDoor()
{
	Timeline->Reverse();
}

