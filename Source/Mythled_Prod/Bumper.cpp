// Fill out your copyright notice in the Description page of Project Settings.

#include "Bumper.h"
#include "CharMovement.h"
#include "GameFramework/Character.h"
#include "Engine.h"

// Sets default values
ABumper::ABumper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->AttachTo(Root);
	//HitBox->InitCapsuleSize(200.0f, 200.0f);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ABumper::OnOverlapBegin);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &ABumper::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ABumper::BeginPlay()
{
	Super::BeginPlay();
	CurrentActor = NULL;
	isSet = false;
}

// Called every frame
void ABumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentActor != NULL) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("PROUT")));
	}

}

void ABumper::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (Cast<ACharacter>(OtherActor)) {
			CurrentActor = OtherActor;
			if (isSet == false) {
				Cast<ACharacter>(CurrentActor)->InputComponent->BindAction("Jump/Interact", IE_Pressed, this, &ABumper::ImpulsePlayer);
				isSet = true;
			}		
		}
	}
}

void ABumper::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		if (Cast<ACharacter>(OtherActor)) {
			//Cast<ACharMovement>(CurrentActor)->BindJump();
			if (Cast<ACharMovement>(CurrentActor)->GetCharacterMovement()->JumpZVelocity != 500.0f) {
				Cast<ACharMovement>(CurrentActor)->GetCharacterMovement()->JumpZVelocity = 500.0f;		
			}
			CurrentActor = NULL;
		}
	}
}

void ABumper::ImpulsePlayer()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("PROUT")));
	if (CurrentActor != NULL) {
		if (Cast<ACharMovement>(CurrentActor)->GetCharacterMovement()->JumpZVelocity != 1000.0f) {
			Cast<ACharMovement>(CurrentActor)->GetCharacterMovement()->JumpZVelocity = 1000.0f;
		}
		
	}
	
}

