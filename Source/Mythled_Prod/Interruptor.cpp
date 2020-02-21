// Fill out your copyright notice in the Description page of Project Settings.

#include "Interruptor.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AInterruptor::AInterruptor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(Root);

	HitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBox"));
	HitBox->AttachTo(Root);
	HitBox->InitCapsuleSize(80.0f, 80.0f);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AInterruptor::OnOverlapBegin);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AInterruptor::OnOverlapEnd);

	//sound
	static ConstructorHelpers::FObjectFinder<USoundCue> ActivationSoundCueObject(TEXT("SoundCue'/Game/Objects/Sound_effects/Fire/FireLoopCue.FireLoopCue'"));
	if (ActivationSoundCueObject.Succeeded()) {
		ActivationSoundCue = ActivationSoundCueObject.Object;

		ActivationAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ActivationAudioComponent"));
		ActivationAudioComponent->SetupAttachment(RootComponent);
	}

}

// Called when the game starts or when spawned
void AInterruptor::BeginPlay()
{
	Super::BeginPlay();

	//sound
	if (ActivationAudioComponent && ActivationSoundCue) {
		ActivationAudioComponent->SetSound(ActivationSoundCue);
	}
	
}

// Called every frame
void AInterruptor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterruptor::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// && !isActivated pour empecher de consumer chaques blocs
	if (OtherActor->ActorHasTag("outline") && !isActivated)
	{
		isActivated = true;
		OtherActor->Destroy();
		FindComponentByClass<UParticleSystemComponent>()->ActivateSystem();

		//sound
		if (ActivationAudioComponent && ActivationSoundCue) {
			ActivationAudioComponent->Play(0.f);
		}

		if (Door) {
			Door->OpenDoor();
		}
	}
}

void AInterruptor::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	
}



