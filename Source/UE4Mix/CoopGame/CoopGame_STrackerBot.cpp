// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "CoopGame_SHealthComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "CoopGame_SCharacter.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "CoopGame_CollideQueryActor.h"

// Sets default values
ACoopGame_STrackerBot::ACoopGame_STrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 是否遮挡导航网格
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	SetRootComponent(MeshComp);

	bUseVelocityChange = true;
	MovementForce = 1000;
	RequiredDistanceToTarget = 100;

	HealthComp = CreateDefaultSubobject<UCoopGame_SHealthComponent>("HealthComp");
	HealthComp->OnHealthChanged.AddDynamic(this, &ACoopGame_STrackerBot::HandleTakeDamage);

	ExplosionDamage = 40.0f;
	ExplosionRadius = 200.0f;

	// 创建球形碰撞组件
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200.0f);
	// 设置碰撞类型,只检测玩家
	// 编辑器警告: 模拟物理与启用碰撞不兼容,不知道解决办法
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);

	SelfDamageInterval = 0.25f;
}

// Called when the game starts or when spawned
void ACoopGame_STrackerBot::BeginPlay()
{
	Super::BeginPlay();

	// 定时任务: 检测附近Bot数量,更新PowerLevel
	FTimerHandle TimerHandle_CheckPowerLevel;
	GetWorldTimerManager().SetTimer(TimerHandle_CheckPowerLevel, this, &ACoopGame_STrackerBot::OnCheckNearbyBots, 1.0f, true);

	// 生成用于碰撞检测的Actor
	//ACoopGame_CollideQueryActor* CollideActor = GetWorld()->SpawnActor<ACoopGame_CollideQueryActor>(GetActorLocation(), FRotator::ZeroRotator);
	//if (CollideActor)
	//{
	//	CollideActor->Init(this);
	//}
}

FVector ACoopGame_STrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	// UE4Mix.build.cs 中添加 "NavigationSystem"
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);
	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}
	// 寻路失败
	return GetActorLocation();
}

void ACoopGame_STrackerBot::HandleTakeDamage(UCoopGame_SHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("Health is %s of %s"), *FString::SanitizeFloat(Health), *GetName());
	if (MatInst == nullptr)
	{
		// 直接修改会导致所有的Bot材质都被修改,Dynamic 只修改这个
		// 动态获取材质,动态创建材质实例
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	if (MatInst)
	{
		// 根据变量名,动态修改材质参数
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	// 死亡
	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
}

void ACoopGame_STrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}
	bExploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	MeshComp->SetVisibility(false, true);
	// 可能模拟物理了之后就禁止修改碰撞
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Role == ROLE_Authority)
	{	// 爆炸伤害
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		// 根据附件Bot数量,增大伤害
		float ActualDamage = ExplosionDamage * (1 + PowerLevel);

		UGameplayStatics::ApplyRadialDamage(this, ActualDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 1.0f, 0, 1.0f);

		//Destroy();
		SetLifeSpan(2.0f); // 留出时间给客户端播放FX
	}

}

void ACoopGame_STrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

void ACoopGame_STrackerBot::OnCheckNearbyBots()
{
	// 检测附近Bot的距离
	const float Radius = 600;
	// 创建临时碰撞形状
	FCollisionShape CollShape;
	CollShape.SetSphere(Radius);
	// 只检测Pawn
	FCollisionObjectQueryParams QueryParams;
	// Bot的Mesh组件,在BP中设置为PhysicsBody(默认 Physics Simulated Actors)
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);

	// 动态创建碰撞形状,进行碰撞检测
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, QueryParams, CollShape);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::White, false, 1.0f);

	int32 BotsNum = 0;
	for (FOverlapResult Result : Overlaps)
	{
		ACoopGame_STrackerBot* Bot = Cast<ACoopGame_STrackerBot>(Result.GetActor());
		if (Bot && Bot != this)
		{
			BotsNum++;
		}
	}

	const int32 MaxPowerLevel = 4;
	PowerLevel = FMath::Clamp(BotsNum, 0, MaxPowerLevel);

	// 更新Mesh
	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	if (MatInst)
	{
		float Alpha = PowerLevel / (float)MaxPowerLevel;
		MatInst->SetScalarParameterValue("PowerLevelAlpha", Alpha);
	}
	//DrawDebugString(GetWorld(), FVector(0, 0, 0), FString::FromInt(PowerLevel), this, FColor::White, 1.0f, true);
}

// Called every frame
void ACoopGame_STrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority && !bExploded)
	{
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();
			//DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!", nullptr, FColor::White, 1);
		}
		else
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= MovementForce;
			MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 64, FColor::Blue, false, 0, 0, 2);
		}
		DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0, 0, 1);
	}

}

// Called to bind functionality to input
void ACoopGame_STrackerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACoopGame_STrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!bStartedSelfDestruction && !bExploded)
	{
		ACoopGame_SCharacter* PlayerPawn = Cast<ACoopGame_SCharacter>(OtherActor);
		if (PlayerPawn)
		{
			if (Role == ROLE_Authority)
			{
				GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ACoopGame_STrackerBot::DamageSelf, SelfDamageInterval, true, 0.0f);
			}
			bStartedSelfDestruction = true;

			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}
}

void ACoopGame_STrackerBot::OnOverlap(AActor* OtherActor)
{
	if (!bStartedSelfDestruction && !bExploded)
	{
		ACoopGame_SCharacter* PlayerPawn = Cast<ACoopGame_SCharacter>(OtherActor);
		if (PlayerPawn)
		{
			if (Role == ROLE_Authority)
			{
				GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ACoopGame_STrackerBot::DamageSelf, SelfDamageInterval, true, 0.0f);
			}
			bStartedSelfDestruction = true;

			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}
}

