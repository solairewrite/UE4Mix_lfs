// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoopGame_SHealthComponent.generated.h"

// 声明代理签名
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UCoopGame_SHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

// 设置 ClassGroup, 在蓝图中分类
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
UCLASS(ClassGroup = (COOP), meta = (BlueprintSpawnableComponent))
class UE4MIX_API UCoopGame_SHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCoopGame_SHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
		float DefaultHealth;
	UPROPERTY(ReplicatedUsing = OwnerUpdateHealthUI, EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
		float Health;

	// 代理
	// 查找参数列表的方法
	//	.cpp 中 MyOwner->OnTakeAnyDamage.AddDynamic(),查找 OnTakeAnyDamage
	//	Actor.h 中,定位到 FTakeAnyDamageSignature OnTakeAnyDamage, 查找 FTakeAnyDamageSignature
	//	Actor.h 中,定位到 DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams(FTakeAnyDamageSignature, ...)
	UFUNCTION()
		void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool bIsDead;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 声明代理事件
	// BlueprintAssignable: 可以在蓝图中赋值
	// BP_PlayerPawn蓝图,右键关闭情境关联,搜索OnHealthChanged
	// 分配 OnHealthChanged,会创建一个事件节点和绑定事件节点
	// BeginPlay中,将事件绑定,目标设为组件: CoopGame_SHealth
	// 每一次 OnHealthChanged.Broadcast,就会触发事件
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnHealthChangedSignature OnHealthChanged;

	// 同步变量触发的函数可以将原来的值作为参数
	UFUNCTION()
		void OwnerUpdateHealthUI(float OldHealth);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		void Heal(float HealAmount);

	float GetHealth() const;
};
