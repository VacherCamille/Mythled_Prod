// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "Math/Rotator.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotatingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHLED_PROD_API URotatingComponent : public UActorComponent
{
	GENERATED_BODY()
	class UTimelineComponent* Timeline;

public:	
	// Sets default values for this component's properties
	URotatingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
		AActor* RotatingActor;

	UPROPERTY(EditAnywhere)
		class UCurveFloat* fCurve;

	UPROPERTY()
		FRotator StartLocation;

	UPROPERTY()
		FRotator EndLocation;

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
