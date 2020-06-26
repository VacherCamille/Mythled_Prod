// Fill out your copyright notice in the Description page of Project Settings.

#include "CharMovement.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

// Sets default values
ACharMovement::ACharMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	canDash = true;
	dashDistance = 1500.0f;
	dashCooldown = 0.5f;
	dashStop = 1.0f;
	ForcePower = 100000.0f;
	ForceDistance = 100000.0f;

	maxWalkSpeed = 450.0f;
	maxRunSpeed = 600.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 450.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	FollowObject = NULL;
	CurrentObject = NULL;
	isHolding = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	ForceHandle = CreateDefaultSubobject<USphereComponent>(TEXT("ForceHandle"));
	ForceHandle->SetupAttachment(FollowCamera);

	FollowCamera->bUsePawnControlRotation = false;
}



// Called when the game starts or when spawned
void ACharMovement::BeginPlay()
{
	Super::BeginPlay();
	maxWalkSpeed = 350.0f;
	maxRunSpeed = 600.0f;
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed;
	isHolding = false;
	GravityPrimitive = NULL;
	PhysicsHandle = FindComponentByClass<UPhysicsHandleComponent>();
}

// Called every frame
void ACharMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ForceLocation = ForceHandle->GetComponentLocation();

	if (isHolding == false && HighlightPrimitive != NULL) {
		HighlightPrimitive->SetRenderCustomDepth(false);
		FollowObject = NULL;
		CurrentObject = NULL;
	}

	if(isHolding == true){
		//FIXEROTATION
		FRotator NewRotator = FollowCamera->GetComponentRotation();
		NewRotator.Pitch = 0;
		NewRotator.Roll = 0;
		//NewRotator.Yaw = NewRotator.Yaw - 90;
		GetRootComponent()->SetWorldRotation(NewRotator);
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("HEY")));
	}
	
	FHitResult OutHit;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector End = (Start + (ForwardVector * ForceDistance));

	FCollisionQueryParams CollisionParams;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	if (isHit) {
		if (OutHit.bBlockingHit) {
			if (OutHit.GetActor()->ActorHasTag("outline") || OutHit.GetActor()->ActorHasTag("outline_tiroir")) {
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString()));
				if (HighlightPrimitive != NULL) {
					HighlightPrimitive->SetRenderCustomDepth(false);
				}
				HighlightPrimitive = OutHit.GetActor()->FindComponentByClass<UPrimitiveComponent>();
				HighlightPrimitive->SetRenderCustomDepth(true);
				FollowObject = OutHit.GetActor();
			} else {
				if (HighlightPrimitive != NULL) {
					HighlightPrimitive->SetRenderCustomDepth(false);
				}
			}
		}
	}
	if (CurrentObject) {
		//PhysicsHandle->SetTargetLocationAndRotation(ForceHandle->GetComponentLocation(), ForceHandle->GetComponentRotation());
		PhysicsHandle->SetTargetLocation(ForceHandle->GetComponentLocation());
		PhysicsHandle->SetTargetRotation(ForceHandle->GetComponentRotation());
	}
	if (!(CurrentObject && CurrentObject->IsValidLowLevel() && !CurrentObject->IsPendingKill()) && isHolding) Interact();
}

// Called to bind functionality to input
void ACharMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump/Interact", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump/Interact", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharMovement::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharMovement::MoveRight);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACharMovement::RunStart);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACharMovement::RunEnd);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ACharMovement::Dash);

	PlayerInputComponent->BindAction("Attraction", IE_Pressed, this, &ACharMovement::Attraction);
	PlayerInputComponent->BindAction("Repulsion", IE_Pressed, this, &ACharMovement::Repulsion);

	PlayerInputComponent->BindAction("Grab object", IE_Pressed, this, &ACharMovement::Interact);
	PlayerInputComponent->BindAction("Exit/Menu", IE_Pressed, this, &ACharMovement::RestartMap);

	PlayerInputComponent->BindAction("ResetRotation", IE_Pressed, this, &ACharMovement::ResetRotation);
}

void ACharMovement::MoveForward(float Axis)
{
	moveForwardValue = Axis;

	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Axis);

}

void ACharMovement::MoveRight(float Axis)
{
	moveRightValue = Axis;

	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);
}

void ACharMovement::RunStart()
{
	if (isHolding == false) {
		bIsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = maxRunSpeed;
	}
}

void ACharMovement::RunEnd()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed;
}

float ACharMovement::GetMoveRight()
{
	return moveRightValue;
}

float ACharMovement::GetMoveForward()
{
	return moveForwardValue;
}

bool ACharMovement::IsRunning()
{
	return bIsRunning;
}

void ACharMovement::Dash()
{
	if (canDash && !GetCharacterMovement()->IsFalling() && isHolding == false)
	{
		GetCharacterMovement()->JumpZVelocity = 0.0f;
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		LaunchCharacter(FVector(this->GetActorForwardVector().X, this->GetActorForwardVector().Y, 0).GetSafeNormal()*dashDistance, true, true);
		canDash = false;
		GetWorldTimerManager().SetTimer(unusedHandle, this, &ACharMovement::StopDashing, dashStop, false);
	}
}

void ACharMovement::ResetDash()
{
	canDash = true;
}

void ACharMovement::StopDashing()
{
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	//GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetWorldTimerManager().SetTimer(unusedHandle, this, &ACharMovement::ResetDash, dashCooldown, false);
}

void ACharMovement::Attraction()
{
	if (FollowObject != NULL && CurrentObject == NULL && GravityPrimitive == NULL) {
		if (FollowObject->ActorHasTag("outline")) {
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("ATTRACTION")));
			CurrentObject = FollowObject;
			GravityPrimitive = CurrentObject->FindComponentByClass<UPrimitiveComponent>();
			//CurrentObject->FindComponentByClass<UPhysicsConstraintComponent>();
			CurrentObject->SetActorLocationAndRotation(ForceLocation, ForceRotation, false, 0, ETeleportType::None);

			PhysicsHandle->GrabComponentAtLocationWithRotation(GravityPrimitive, FName(), ForceHandle->GetComponentLocation(), ForceHandle->GetComponentRotation());

			//ResetRotation();
			//PhysicsHandle->GrabComponentAtLocation(GravityPrimitive, FName(), ForceHandle->GetComponentLocation());

			//GravityPrimitive->SetEnableGravity(false);
			//CurrentObject->GetRootComponent()->ComponentVelocity = NulVelocity;
			//SET LOCATION AND ROTATION OF FOLLOW OBJECT PARENT TO CHARACTER ACTOR	
			isHolding = true;
		} else if (FollowObject->ActorHasTag("outline_tiroir")) {
			Cast<ATiroir>(FollowObject)->Activate();
		}
	}
}

void ACharMovement::Repulsion()
{
	if (CurrentObject != NULL && GravityPrimitive != NULL) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("REPULSION")));
		//DELETE LINK BETWEEN CURRENT OBJECT AND CHARACTER ACTOR
		//GravityPrimitive->SetEnableGravity(true);
		//CurrentObject->GetRootComponent()->ComponentVelocity = NulVelocity;

		PhysicsHandle->ReleaseComponent();

		GravityPrimitive->AddImpulse(FollowCamera->GetForwardVector() * ForcePower);
		//ADDFORCE TO THE CURRENT OBJECT ON DIRECTION TO THE FORWARD VECTOR OF FOLLOW CAMERA
		GravityPrimitive = NULL;
		CurrentObject = NULL;

		FRotator NewRotator = FollowCamera->GetComponentRotation();
		NewRotator.Pitch = 0;
		NewRotator.Roll = 0;
		GetRootComponent()->SetWorldRotation(NewRotator);		

		isHolding = false;
	}
	else if (FollowObject != NULL) {
		if (FollowObject->ActorHasTag("outline")) {
			if (GetDistanceTo(FollowObject) < 1000.0f) {
				FollowObject->FindComponentByClass<UPrimitiveComponent>()->AddImpulse(FollowCamera->GetForwardVector()*ForcePower);
			}
		}
		else if (FollowObject->ActorHasTag("outline_tiroir")) {
				Cast<ATiroir>(FollowObject)->Desactivate();
		}
	}
}

void ACharMovement::Interact()
{
	if (isHolding == true) {
		PhysicsHandle->ReleaseComponent();
		//GravityPrimitive->SetEnableGravity(true);
		//CurrentObject->GetRootComponent()->ComponentVelocity = NulVelocity;
		GravityPrimitive = NULL;
		CurrentObject = NULL;

		FRotator NewRotator = FollowCamera->GetComponentRotation();
		NewRotator.Pitch = 0;
		NewRotator.Roll = 0;
		GetRootComponent()->SetWorldRotation(NewRotator);

		isHolding = false;
	}
	if (isHolding == false) {
		if (InteractObject) {
			InteractObject->ChangeIsActivated();
		}
	}
}

void ACharMovement::FixeRotationPlayer()
{
	//FVector MeshPosition = FVector(0.0f, 0.0f, FollowCamera->GetForwardVector().Z);
	//SetWorldRotation(MeshPosition);
	//GetRootComponent()->SetRelativeRotation(MeshPosition);
}


void ACharMovement::ResetRotation()
{
	if (CurrentObject != NULL && isHolding == true) {
		CurrentObject->SetActorRelativeRotation(FRotator(0, 0, 0));
	}

}


void ACharMovement::RestartMap()
{
	//UGameplayStatics::OpenLevel(this, "Sandbox_MAP");
}

void ACharMovement::BindJump()
{
	UInputComponent* PlayerInputComponent = NULL;
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump/Interact", IE_Pressed, this, &ACharacter::Jump);
}