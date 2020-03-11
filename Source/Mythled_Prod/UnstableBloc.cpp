// Fill out your copyright notice in the Description page of Project Settings.

#include "UnstableBloc.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/EngineGlobals.h"

// Sets default values
AUnstableBloc::AUnstableBloc()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->AttachTo(Root);
	HitBox->InitBoxExtent(FVector(45,35,40));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(Root);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AUnstableBloc::OnOverlapBegin);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AUnstableBloc::OnOverlapEnd);

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	ZOffset = 50.f;

}

// Called when the game starts or when spawned
void AUnstableBloc::BeginPlay()
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
void AUnstableBloc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AUnstableBloc::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (GEngine) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Begin Overlap"));
			Timeline->Play();
		}
	}
}

void AUnstableBloc::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (GEngine) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Begin End"));
			Timeline->Reverse();
		}
	}
}

void AUnstableBloc::TimelineFloatReturn(float fval)
{
	SetActorLocation(FMath::Lerp(StartLocation, EndLocation, fval));
}

void AUnstableBloc::OnTimelineFinished()
{

}

