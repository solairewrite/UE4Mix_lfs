// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "_Xanadu/Characters/Base/Interfaces/IHealth.h" // 接口必须include
#include "_Xanadu/Base/XanaduTypes.h"
#include "AICharacterBase.generated.h"

class AIControllerBase;
class APlayerCharacterBase;
class UNavigationPath;
class AAICommand;
class UHealthComponent;
class ATargetPoint;

UCLASS()
class UE4MIX_API AAICharacterBase : public ACharacter,
	public IIHealth
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float AccelerateSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float RotateSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float MeleeRange;

	APlayerCharacterBase* GetPlayer();
	template<class T>
	T* GetPlayer() const
	{
		return Cast<T>(GetPlayer());
	}

	AAICommand* CurrentCommand;

	// Tick()中决定是否移动到玩家
	bool bMovingToPlayer;
	// Tick()中决定是否转向玩家
	UPROPERTY(BlueprintReadOnly, Category = "AICharacter")
		bool bTurningToPlayer;

	UPROPERTY(BlueprintReadOnly, Category = "AICharacter")
		ERotateDirection RotateDirection;

	// AI移动的目标位置,是两次寻路位置的差值
	// 防止突然改变目标位置造成的训传卡顿
	FVector TargetLoc;
	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float TargetLocInterpSpeed;

	// AI寻路的路径点,实际移动的目标位置TargetLoc,是两个路径点的差值
	FVector NextPathPoint;
	FVector GetNextPathPoint();

	FTimerHandle TH_RefreshPath;
	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float RefreshPathInterval;
	void RefreshPath();

	void DebugDrawPath(UNavigationPath* inPath);

	void DebugDrawRotateInfo(ACharacter* inPlayer);

	void TickMoveToPlayer(float DeltaTime);

	void TickTurnToPlayer(float DeltaTime);

	// 计算旋转方向,向右旋转(顺时针)增加Yaw值,返回+1
	// 向左旋转(逆时针)减少Yaw值,返回-1
	int CalcRotateDirection(FRotator inRotator);

	// 动画名称和资源Map
	UPROPERTY(EditAnywhere, Category = "Anim")
		TMap<FName, UAnimSequenceBase*> AnimMap;

	// 已经播放的Montage和动画名Map
	TMap<UAnimMontage*, FName> MontageNameMap;

	// 动画结束回调,代理函数
	UFUNCTION()
		void OnAnimMontageEnd(UAnimMontage* inMontage, bool bInterrupted);

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadOnly, Category = "AICharacter")
		bool bDead;

public:

	void SetCurrentCommand(AAICommand* inCommand);

	// 当前任务成功
	void CommandSuccess();

	// 当前任务失败
	void CommandFail();

	void MoveToPlayer();

	void TurnToPlayer();

	// 根据AnimSequence,动态播放Montage,返回动画时长
	float PlayAnim(FName inAnimName, bool bPlayImmediately = false);

	// 血量接口
	virtual float GetHealth_Implementation() override;
	virtual float GetHealthMax_Implementation() override;

	virtual bool IsAI() override;

	virtual void OnAttackBy(class AController* InstigatedBy, AActor* DamageCauser) override;

	virtual void OnDead();

	void OnDeathAnimEnd();

	void OnMelee();

	// 受击动画
	virtual bool CanPlayTakeHitAnim() override;
	virtual void PlayTakeHitAnim() override;

	// 受到冲力
	virtual bool CanTakeImpulse() override;
	virtual void TakeImpulse(FVector inImpulseVector) override;

	// AIState
protected:
	// 每次Idle的最短时间
	UPROPERTY(EditDefaultsOnly, Category = "AIState")
		float IdleTimeRangeMin;
	// 每次Idle的最长时间
	UPROPERTY(EditDefaultsOnly, Category = "AIState")
		float IdleTimeRangeMax;

	UPROPERTY(EditDefaultsOnly, Category = "AIState")
		bool bCanPatrol;
	// Idle后巡逻的概率
	UPROPERTY(EditDefaultsOnly, Category = "AIState")
		float PatrolProbability;
	// 是否使用固定巡逻点
	UPROPERTY(EditDefaultsOnly, Category = "AIState")
		bool bFixdPatrolPoint;
	// 如果使用固定巡逻点,巡逻点的引用数组
	UPROPERTY(EditAnywhere, Category = "AIState")
		TArray<ATargetPoint*> PatrolPointArr;
	FVector CurrPatrolLoc; // 当前巡逻点位置

	void TickPatrol();
	bool bPatroling;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UBoxComponent* MeleeBoxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float MeleeDamage;

public:
	EAIState GetAIState();
	void SetAIState(EAIState inNewState);

	float GetIdleTimeRangeMin() { return IdleTimeRangeMin; }
	float GetIdleTimeRangeMax() { return IdleTimeRangeMax; }

	bool CanPatrol() { return bCanPatrol; }
	float GetPatrolProbability() { return PatrolProbability; }
	FVector GetRandomPatrolLoc();
	void StartPartol();
	void PatrolFinish();

protected:
	// 血条
	UPROPERTY(VisibleAnywhere, Category = "HealthBar")
		class UWidgetComponent* WidgetComp;

	void CreateWidgetComp();
	void InitWidgetComp();
};
