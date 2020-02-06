// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CoopGame_STrackerBot.generated.h"

class UStaticMeshComponent;
class UCoopGame_SHealthComponent;
class UMaterialInstanceDynamic;
class USphereComponent;
class USoundCue;

UCLASS()
class UE4MIX_API ACoopGame_STrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACoopGame_STrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* MeshComp;

	FVector NextPathPoint;
	FVector GetNextPathPoint();

	void RefreshPath();

	FTimerHandle TH_RefreshPath;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float RequiredDistanceToTarget;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UCoopGame_SHealthComponent* HealthComp;

	UFUNCTION()
		void HandleTakeDamage(UCoopGame_SHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// 受伤时闪烁的材质
	UMaterialInstanceDynamic* MatInst;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		UParticleSystem* ExplosionEffect;

	bool bExploded;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float ExplosionRadius;
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float ExplosionDamage;

	void SelfDestruct();

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USphereComponent* SphereComp;
	
	// AI靠近Player倒计时
	bool bStartedSelfDestruction;
	FTimerHandle TimerHandle_SelfDamage;
	void DamageSelf();

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float SelfDamageInterval;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		USoundCue* SelfDestructSound;
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		USoundCue* ExplodeSound;

	void OnCheckNearbyBots();

	// 爆炸等级,影响伤害和颜色
	int32 PowerLevel;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 相交回调函数
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void OnOverlap(AActor* OtherActor);
};
