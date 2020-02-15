// Fill out your copyright notice in the Description page of Project Settings.

#include "PressurePlate.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(Root);

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->AttachTo(Root);
	HitBox->InitBoxExtent(FVector(42.5, 42.5, 5.0));

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapBegin);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnOverlapEnd);

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	ZOffset = -8.0;
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	if (fCurve) {
		Timeline->AddInterpFloat(fCurve, InterpFunction, FName("Alpha"));
		Timeline->SetTimelineFinishedFunc(TimelineFinished);

		StartLocation = GetActorLocation();
		EndLocation = FVector(StartLocation.X, StartLocation.Y, StartLocation.Z + ZOffset);

		Timeline->SetLooping(false);
		Timeline->SetIgnoreTimeDilation(true);
	}
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APressurePlate::TimelineFloatReturn(float fval)
{
	SetActorLocation(FMath::Lerp(StartLocation, EndLocation, fval));
}

void APressurePlate::OnTimelineFinished()
{
}

void APressurePlate::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (Door) {
			Door->OpenDoor();
			Timeline->Play();
		}
	}
}

void APressurePlate::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (Door) {
			if (Cast<ACharacter>(OtherActor)) {
				Door->CloseDoor();
				Timeline->Reverse();
			}
		}
	}
}