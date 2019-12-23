// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopGame_SPickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class ACoopGame_SPowerupActor;

UCLASS()
class UE4MIX_API ACoopGame_SPickupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACoopGame_SPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
		TSubclassOf<ACoopGame_SPowerupActor> PowerupClass;

	ACoopGame_SPowerupActor* PowerupInstance;

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
		float CooldownDuration; // 重新生成时间

	FTimerHandle TimerHandle_RespawnTimer;

	void Respawn();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 检测相交
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
