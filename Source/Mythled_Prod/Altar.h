// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Components/CapsuleComponent.h"
#include "Door.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "Altar.generated.h"

UCLASS()
class MYTHLED_PROD_API AAltar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAltar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	bool isActivated;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* HitBox;

	UPROPERTY(EditAnywhere)
		ADoor* Door;

	//Sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAcess = "true"))
		class USoundCue* ActivationSoundCue;

	UAudioComponent* ActivationAudioComponent;
	
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void ChangeIsActivated();


};
