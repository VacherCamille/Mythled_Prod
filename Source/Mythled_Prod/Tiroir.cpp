// Fill out your copyright notice in the Description page of Project Settings.

#include "Tiroir.h"
#include "Engine.h"

// Sets default values
ATiroir::ATiroir()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	XOffset = 0.f;
	YOffset = 0.f;
	ZOffset = 0.f;

	//sound
	static ConstructorHelpers::FObjectFinder<USoundCue> ActivationSoundCueObject(TEXT("SoundCue'/Game/Objects/Sound_effects/Tiroir/TiroirCue.TiroirCue'"));
	if (ActivationSoundCueObject.Succeeded()) {
		ActivationSoundCue = ActivationSoundCueObject.Object;

		ActivationAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ActivationAudioComponent"));
		ActivationAudioComponent->SetupAttachment(RootComponent);
	}

}

// Called when the game starts or when spawned
void ATiroir::BeginPlay()
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
		//sound
		if (ActivationAudioComponent && ActivationSoundCue) {
			ActivationAudioComponent->Play(0.f);
		}
		Timeline->Play();
		isActivated = true;
	}
}

void ATiroir::Desactivate() {
	if (isActivated) {
		//sound
		if (ActivationAudioComponent && ActivationSoundCue) {
			ActivationAudioComponent->Play(0.f);
		}
		Timeline->Reverse();
		isActivated = false;
	}
}