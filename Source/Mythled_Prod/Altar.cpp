// Fill out your copyright notice in the Description page of Project Settings.

#include "Altar.h"
#include "Components/StaticMeshComponent.h"
#include "CharMovement.h"
#include "Engine.h"

// Sets default values
AAltar::AAltar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(Root);

	HitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBox"));
	HitBox->AttachTo(Root);
	HitBox->InitCapsuleSize(200.0f, 200.0f);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AAltar::OnOverlapBegin);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AAltar::OnOverlapEnd);

	//sound
	static ConstructorHelpers::FObjectFinder<USoundCue> ActivationSoundCueObject(TEXT("SoundCue'/Game/Objects/Sound_effects/Altar/AltarCue.AltarCue'"));
	if (ActivationSoundCueObject.Succeeded()) {
		ActivationSoundCue = ActivationSoundCueObject.Object;

		ActivationAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ActivationAudioComponent"));
		ActivationAudioComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AAltar::BeginPlay()
{
	Super::BeginPlay();

	//sound
	if (ActivationAudioComponent && ActivationSoundCue) {
		ActivationAudioComponent->SetSound(ActivationSoundCue);
	}
	
}

// Called every frame
void AAltar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isActivated) FindComponentByClass<UPointLightComponent>()->SetLightColor(FLinearColor(0.527115, 0.386429, 0.208637, 1.0));
	else FindComponentByClass<UPointLightComponent>()->SetLightColor(FLinearColor(0.0, 0.0, 0.0, 1.0));
}

void AAltar::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (Cast<ACharacter>(OtherActor)) {
			Cast<ACharMovement>(OtherActor)->InteractObject = this;
		}
	}
}

void AAltar::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (Cast<ACharacter>(OtherActor)) {
			Cast<ACharMovement>(OtherActor)->InteractObject = NULL;
		}
	}
}

void AAltar::ChangeIsActivated() 
{
	isActivated = !isActivated;
		if (isActivated) {
			//sound
			if (ActivationAudioComponent && ActivationSoundCue) {
				ActivationAudioComponent->Play(0.f);
			}
			if (Door) {
				Door->OpenDoor();
			}
			else {
				Door->CloseDoor();
			}
		}
}