// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE4MIX_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float HealthMax;

	float Health;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 代理函数,当Actor调用OnTakeAnyDamage.Broadcast时,会调用此函数
	UFUNCTION()
		virtual void OnTakeDamage(
			AActor* DamagedActor,
			float Damage,
			const class UDamageType* DamageType,
			class AController* InstigatedBy,
			AActor* DamageCauser);

	float GetHealth();
};
