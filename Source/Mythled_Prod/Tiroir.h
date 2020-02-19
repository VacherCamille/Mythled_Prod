// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Tiroir.generated.h"

UCLASS()
class MYTHLED_PROD_API ATiroir : public AActor
{
	GENERATED_BODY()

		class UTimelineComponent* Timeline;
	
public:	
	// Sets default values for this actor's properties
	ATiroir();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Activate();
	void Desactivate();

	UPROPERTY(EditAnywhere)
		bool isActivated;

	UPROPERTY(EditAnywhere)
		class UCurveFloat* fCurve;

	UPROPERTY()
		FVector StartLocation;

	UPROPERTY()
		FVector EndLocation;

	UPROPERTY(EditAnywhere)
		float XOffset;

	UPROPERTY(EditAnywhere)
		float YOffset;

	UPROPERTY(EditAnywhere)
		float ZOffset;

	FOnTimelineFloat InterpFunction{};

	FOnTimelineEvent TimelineFinished{};

	UFUNCTION()
		void TimelineFloatReturn(float fval);

	UFUNCTION()
		void OnTimelineFinished();
};
