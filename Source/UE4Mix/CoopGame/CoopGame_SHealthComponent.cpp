// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_SHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "CoopGame_SCharacter.h"

static int32 DebugHealth = 0;
FAutoConsoleVariableRef CVARDebugHealth(
	TEXT("Coop.DebugHealth"), // 控制台命令字符
	DebugHealth,
	TEXT("Show Debug String of Health Component"),
	ECVF_Cheat
);

// Sets default values for this component's properties
UCoopGame_SHealthComponent::UCoopGame_SHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DefaultHealth = 100;

	// 同步组件
	SetIsReplicated(true);
}


// Called when the game starts
void UCoopGame_SHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		Health = DefaultHealth;

		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			// 组件给玩家添加受伤代理
			// 玩家受伤->触发组件代理函数->组件代理函数内部广播 OnHealthChanged.Broadcast
			// ->触发玩家代理函数 OnTakeAnyDamage.AddDynamic->玩家代理函数内部处理死亡等逻辑
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UCoopGame_SHealthComponent::HandleTakeAnyDamage);
		}
	}
}

void UCoopGame_SHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	// 格式化浮点数,删除后面的0
	if (DebugHealth >= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("血量改变: %s"), *FString::SanitizeFloat(Health));
	}

}

// Called every frame
void UCoopGame_SHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCoopGame_SHealthComponent::OwnerUpdateHealthUI(float OldHealth)
{
	ACoopGame_SCharacter* MyOwner;
	MyOwner = Cast<ACoopGame_SCharacter>(GetOwner());
	if (MyOwner)
	{
		MyOwner->BPUpdateHealthUI();
	}

	float Damage = Health - OldHealth;
	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

void UCoopGame_SHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0 || Health <= 0)
	{
		return;
	}
	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);
	// SanitizeFloat: 去掉小数点后面的0
	UE_LOG(LogTemp, Warning, TEXT("加血: %s (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealAmount));
	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}

// 标记为const,禁止修改任何变量
float UCoopGame_SHealthComponent::GetHealth() const
{
	return Health;
}

void UCoopGame_SHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCoopGame_SHealthComponent, Health);
}