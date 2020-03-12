// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Engine.h"

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

	//sound
	if (!ActivationSoundCue) {
		static ConstructorHelpers::FObjectFinder<USoundCue> ActivationSoundCueObject(TEXT("SoundCue'/Game/Objects/Sound_effects/Pestle/pestleCue.pestleCue'"));
		ActivationSoundCue = ActivationSoundCueObject.Object;
		if (ActivationSoundCueObject.Succeeded()) {
			ActivationAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ActivationAudioComponent"));
			ActivationAudioComponent->SetupAttachment(RootComponent);
		}
	} else {
		ActivationAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ActivationAudioComponent"));
		ActivationAudioComponent->SetupAttachment(RootComponent);
	}

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

	//sound
	if (ActivationAudioComponent && ActivationSoundCue) {
		ActivationAudioComponent->SetSound(ActivationSoundCue);
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
	if (ActivationAudioComponent && ActivationSoundCue) {
		ActivationAudioComponent->Stop();
	}
}

void ADoor::OpenDoor()
{
	Timeline->Play();

	//sound
	if (ActivationAudioComponent && ActivationSoundCue) {
		ActivationAudioComponent->Play(0.f);
	}
}

void ADoor::CloseDoor()
{
	Timeline->Reverse();
	//sound
	if (ActivationAudioComponent && ActivationSoundCue) {
		ActivationAudioComponent->Play(0.f);
	}
}

