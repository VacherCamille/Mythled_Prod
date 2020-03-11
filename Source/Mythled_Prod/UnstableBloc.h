// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnstableBloc.generated.h"

UCLASS()
class MYTHLED_PROD_API AUnstableBloc : public AActor
{
	GENERATED_BODY()
		class UTimelineComponent* Timeline;
	
public:	
	// Sets default values for this actor's properties
	AUnstableBloc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UBoxComponent* HitBox;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	/////////

	UPROPERTY(EditAnywhere)
		class UCurveFloat* fCurve;

	UPROPERTY()
		FVector StartLocation;

	UPROPERTY()
		FVector EndLocation;

	UPROPERTY(EditAnywhere)
		float ZOffset;

	FOnTimelineFloat InterpFunction{};

	FOnTimelineEvent TimelineFinished{};

	UFUNCTION()
		void TimelineFloatReturn(float fval);

	UFUNCTION()
		void OnTimelineFinished();

};
