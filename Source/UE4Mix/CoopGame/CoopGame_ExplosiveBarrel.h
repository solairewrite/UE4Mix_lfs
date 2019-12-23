// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopGame_ExplosiveBarrel.generated.h"

class UCoopGame_SHealthComponent;
class UStaticMeshComponent;
class URadialForceComponent;
class UParticleSystem;
class UMaterialInterface;

UCLASS()
class UE4MIX_API ACoopGame_ExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACoopGame_ExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UCoopGame_SHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DamageRadius;

	UFUNCTION()
		void OnHealthChanged(UCoopGame_SHealthComponent* OwingHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(ReplicatedUsing = OnRep_Exploded)
		bool bExploded;

	UFUNCTION()
		void OnRep_Exploded();

	// 爆炸时,对自己施加的向上的力
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
		UParticleSystem* ExplosionFX;

	// 爆炸后的材质
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		UMaterialInterface* ExplodedMat;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
