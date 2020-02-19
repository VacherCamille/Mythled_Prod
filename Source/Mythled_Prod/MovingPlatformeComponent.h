// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovingPlatformeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHLED_PROD_API UMovingPlatformeComponent : public UActorComponent
{
	GENERATED_BODY()
	class UTimelineComponent* Timeline;

public:	
	// Sets default values for this component's properties
	UMovingPlatformeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
		AActor* MovingActor;

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
