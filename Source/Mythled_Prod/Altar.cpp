// Fill out your copyright notice in the Description page of Project Settings.

#include "Altar.h"
#include "Components/StaticMeshComponent.h"
#include "CharMovement.h"

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
}

// Called when the game starts or when spawned
void AAltar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAltar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FindComponentByClass<UParticleSystemComponent>()
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
}