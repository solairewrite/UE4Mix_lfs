// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopGame_SWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;

// ����ͬ���ṹ��
USTRUCT()
struct FRepFireInfo
{
	GENERATED_BODY()
public:
	// �²�: ÿ��ͬ�������Զ���Ҫ��UPROPERTY()
	UPROPERTY()
		bool bHitSomething;
	UPROPERTY()
		FVector_NetQuantize HitLoc; // ����ͬ���Ż��������ݸ�ʽ
	UPROPERTY()
		TEnumAsByte<EPhysicalSurface> SurfaceType; // ͬ��ö��
	UPROPERTY()
		int FireCount; // ȷ��ÿ�νṹ�嶼����һ��ͬ��ʱ��ͬ,������ͬ����ֵ�����޷�ͬ��
	UPROPERTY()
		bool bShouldRep;
};

UCLASS()
class UE4MIX_API ACoopGame_SWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACoopGame_SWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float BaseDamage;

	// ��ǹ��ʱ����
	FTimerHandle TimerHandle_TimeBetweenShots;
	float LastFireTime;
	// ÿ�����������
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float RateOfFire;
	float TimeBetweenShots;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire();

	void StartFire();
	void StopFire();

	void OnFireHitSomething(FHitResult HitResult);

	UPROPERTY(ReplicatedUsing = OnRep_Fire)
		FRepFireInfo RepFireInfo;
	UFUNCTION()
		void OnRep_Fire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* HeadImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundBase* FireSound;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
