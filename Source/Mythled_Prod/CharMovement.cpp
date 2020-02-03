// Fill out your copyright notice in the Description page of Project Settings.

#include "CharMovement.h"
#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values
ACharMovement::ACharMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	canDash = true;
	dashDistance = 1500.0f;
	dashCooldown = 0.5f;
	dashStop = 1.0f;

	maxWalkSpeed = 450.0f;
	maxRunSpeed = 600.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.2f; 

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

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
}

// Called every frame
void ACharMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	moveForwardValue = Axis;

	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);
}

void ACharMovement::RunStart()
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = maxRunSpeed;
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
	if (canDash && !GetCharacterMovement()->IsFalling()) 
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
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetWorldTimerManager().SetTimer(unusedHandle, this, &ACharMovement::ResetDash, dashCooldown, false);
}


