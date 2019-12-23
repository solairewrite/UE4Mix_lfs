// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoopGame_SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ACoopGame_SWeapon;
class UCoopGame_SHealthComponent;

UCLASS()
class UE4MIX_API ACoopGame_SCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACoopGame_SCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float ZoomedFOV;
	float DefaultFOV;

	// 相机开镜速度
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100.0))
		float ZoomInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<ACoopGame_SWeapon> StarterWeaponClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
		FName WeaponAttackSocketName;

	void StartFire();
	void StopFire();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UCoopGame_SHealthComponent* HealthComp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void BeginCrouch();
	void EndCrouch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	// 同步变量
	UPROPERTY(Replicated)
		ACoopGame_SWeapon* CurrentWeapon;

	virtual FVector GetPawnViewLocation() const override;

	void BeginZoom();
	void EndZoom();

	void Fire();

	void CreateAndSetWeapon(TSubclassOf<ACoopGame_SWeapon> WeaponClass);

	// 代理函数
	UFUNCTION()
		void OnHealthChanged(UCoopGame_SHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 同步变量,蓝图中控制播放死亡动画
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
		bool bDied;

	UFUNCTION(BlueprintImplementableEvent, Category = "Components")
		void BPUpdateHealthUI();
};
