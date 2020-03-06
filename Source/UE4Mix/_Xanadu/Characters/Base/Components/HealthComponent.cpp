// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "_Xanadu/Characters/Base/Interfaces/IHealth.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HealthMax = 100.0f;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = HealthMax;

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetOwner() || GetOwner() != DamagedActor || Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, HealthMax);

	// AI记录被攻击
	IIHealth* tHealthActor = Cast<IIHealth>(GetOwner());
	if (tHealthActor)
	{
		if (tHealthActor->IsAI())
		{
			tHealthActor->OnAttackBy(InstigatedBy, DamageCauser);
		}

		if (Health <= 0.0f)
		{
			tHealthActor->OnDead();
		}
	}
}

float UHealthComponent::GetHealth()
{
	return Health;
}

float UHealthComponent::GetHealthMax()
{
	return HealthMax;
}

