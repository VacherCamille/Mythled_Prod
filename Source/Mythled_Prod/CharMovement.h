// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Altar.h"
#include "Tiroir.h"

#include "CharMovement.generated.h"
UCLASS()
class MYTHLED_PROD_API ACharMovement : public ACharacter
{
	GENERATED_BODY()

private:
	bool bIsRunning;
	float moveForwardValue;
	float moveRightValue;

public:
	// Sets default values for this character's properties
	ACharMovement();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USphereComponent* ForceHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Physics)
		UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		AActor* FollowObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		AActor* CurrentObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		AAltar* InteractObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float maxWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float maxRunSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float dashCounter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float dashDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float dashCooldown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		bool canDash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		bool isHolding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float dashStop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float ForcePower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float ForceDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float MaxTeleportDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		FTimerHandle unusedHandle;

	UPROPERTY(EditAnywhere, Category = "Location")
		FVector ForceLocation;

	UPROPERTY(EditAnywhere, Category = "Location")
		FVector ObjectVector;

	UPROPERTY(EditAnywhere, Category = "Location")
		float ObjectDistance;

	UPROPERTY(EditAnywhere, Category = "Location")
		FVector NulVelocity;

	UPROPERTY(EditAnywhere, Category = "Location")
		FQuat ForceRotation;

	UPROPERTY(EditAnywhere, Category = "Glow")
		UPrimitiveComponent* HighlightPrimitive;

	UPROPERTY(EditAnywhere, Category = "Gravity")
		UPrimitiveComponent* GravityPrimitive;

	UFUNCTION(BlueprintCallable, Category = Animation)
		void Dash();

	UFUNCTION(BlueprintCallable, Category = Animation)
		bool IsRunning();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void RunStart();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void RunEnd();

	UFUNCTION(BlueprintCallable, Category = Animation)
		float GetMoveRight();

	UFUNCTION(BlueprintCallable, Category = Animation)
		float GetMoveForward();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void StopDashing();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void ResetDash();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void Attraction();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void Repulsion();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void Interact();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void FixeRotationPlayer();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void ResetRotation();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void RestartMap();

	UFUNCTION(BlueprintCallable, Category = Animation)
		void BindJump();


	void MoveForward(float Axis);
	void MoveRight(float Axis);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
