// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "_Xanadu/Characters/Base/Interfaces/IHealth.h"
#include "PlayerCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UHealthComponent;

UCLASS()
class UE4MIX_API APlayerCharacterBase : public ACharacter,
	public IIHealth
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
		UAnimMontage* AnimMontage_Melee;

	bool bIsMeleeing;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UBoxComponent* MeleeBoxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float MeleeDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bDead;

public:
	void MoveForward(float inValue);
	void MoveRight(float inValue);

	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetViewRotation() const override;

	// 近战动画通知
	virtual void OnMelee();

protected:
	// 按下鼠标左键触发近战命令
	virtual void Melee();
	void OnMeleeMontageFinish();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UHealthComponent* HealthComp;

	// 血量接口
public:
	virtual float GetHealth_Implementation() override;
	virtual float GetHealthMax_Implementation() override;

	virtual void OnDead() override;
};
