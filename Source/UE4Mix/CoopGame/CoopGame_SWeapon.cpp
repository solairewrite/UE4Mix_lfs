// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UE4Mix.h" // #define Physic_Default SurfaceType1
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

// 控制台命令
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("Coop.DebugWeapons"), // 控制台命令字符
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat
);

// Sets default values
ACoopGame_SWeapon::ACoopGame_SWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target"; // UE4编辑器中,P_SomkeTrial的Target.Distribution.ParameterName

	BaseDamage = 20.0f;

	RateOfFire = 600;

	// 同步到客户端
	SetReplicates(true);
	NetUpdateFrequency = 100.0f; // 每秒更新多少次
	MinNetUpdateFrequency = 33.0f; // 默认2.0f
}

// Called when the game starts or when spawned
void ACoopGame_SWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}

// Called every frame
void ACoopGame_SWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoopGame_SWeapon::Fire()
{
	if (Role < ROLE_Authority)
	{
		ServerFire();
		// 这里没有return,为了播放下面的特效,但是我认为这样的做法不好
		// 应该服务端通知客户端播放特效
	}

	AActor* MyOwner = GetOwner();
	if (!MyOwner)
		return;

	// 记录上一次开火时间,冷却时间内无法攻击
	LastFireTime = GetWorld()->TimeSeconds;

	FVector EyeLocation;
	FRotator EyeRotation;
	// 获取Actor的视点,武器射线检测常用
	// 角色重写 GetPawnViewLocation() 返回相机位置
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// Vector() 将旋转转为它的朝向的单位向量
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	// 返回物理材质
	QueryParams.bReturnPhysicalMaterial = true;

	// 射线检测
	FHitResult Hit;
	bool bHitSomething;
	//bHitSomething = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);

	// 碰撞通道设置
	//	Project Settings -> Collision -> Trace Channels, 新建 WeaponFire
	//	这样有碰撞检测功能的组件的属性面板中, Collision -> 碰撞预设 -> 追踪响应, 就会多出 WeaponFire 通道
	//	设置胶囊体组件忽略 WeaponFire 通道, Mesh 组件阻挡 WeaponFire 通道
	// WeaponFire 通道对应预定义宏 ECC_GameTraceChannel1

	bHitSomething = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, Collision_WeaponFire, QueryParams);
	//bHitSomething = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);
	if (bHitSomething)
	{
		//UE_LOG(LogTemp, Warning, TEXT("武器射线检测,检测到物体"));

		OnFireHitSomething(Hit);
	}
	else
	{
		// 同步开火特效
		RepFireInfo.bShouldRep = false;
		RepFireInfo.bHitSomething = false;
		RepFireInfo.FireCount++;
		RepFireInfo.bShouldRep = true;
		OnRep_Fire();
	}

	// 绘制调试线
	if (DebugWeaponDrawing > 1)
	{
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
	}
}

void ACoopGame_SWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ACoopGame_SWeapon::ServerFire_Validate()
{
	return true;
}

void ACoopGame_SWeapon::StartFire()
{
	// 射击多久之后开始: 上一次开火时间+冷却时间-当前时间
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	// 定时任务:开火
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ACoopGame_SWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ACoopGame_SWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ACoopGame_SWeapon::OnFireHitSomething(FHitResult HitResult)
{
	// 设置物理材质
	//	编辑->项目设置->Physical Surface
	//		添加SurfaceType1 : FleshDefault
	//	新建 Physics->Physical Material : PhysMat_FleshDefault
	//		设置 Physical Properties->Surface Type : FleshDefault(可能需要重启)
	//	角色骨骼的 Physics 设置, 选中想要修改的物理形体(比如头部)
	//		设置 Collision->Phys Mat Override : PhysMat_FleshDefault

	// 检测物理材质
	// EPhysicalSurface SurfaceType == SurfaceType1(系统预设,对应FleshDefault)
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

	if (Role == ROLE_Authority)
	{
		float RealDamage = BaseDamage;
		switch (SurfaceType)
		{
		case Flesh:
			break;
		case Physic_Head:
			RealDamage *= 4.0f;
			break;
		default:
			break;
		}

		// 伤害处理
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			AActor* HitActor = HitResult.GetActor();
			FVector ShotDirection = HitResult.TraceEnd - HitResult.TraceStart;
			AController* InstigatorController = MyOwner->GetInstigatorController();
			UGameplayStatics::ApplyPointDamage(HitActor, RealDamage, ShotDirection, HitResult, InstigatorController, MyOwner, DamageType);

			//UE_LOG(LogTemp, Warning, TEXT("武器射线造成伤害: %f"), RealDamage);
		}
	}

	// 同步播放特效
	RepFireInfo.bShouldRep = false;
	RepFireInfo.bHitSomething = true;
	RepFireInfo.HitLoc = HitResult.ImpactPoint;
	RepFireInfo.SurfaceType = SurfaceType;
	RepFireInfo.FireCount++;
	RepFireInfo.bShouldRep = true;
	OnRep_Fire();

	if (DebugWeaponDrawing > 0)
	{
		DrawDebugSphere(GetWorld(), HitResult.Location, 3.0f, 12, FColor::Cyan, false, 1.0f, 0, 1.0f);
	}
}

void ACoopGame_SWeapon::OnRep_Fire()
{
	if (!RepFireInfo.bShouldRep)
	{
		return;
	}

	// 音效
	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleLocation);
	}

	// 镜头抖动
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}

	// 枪口特效
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	//if (!bHitSomething)
	if (!RepFireInfo.bHitSomething)
		return;

	// 击中特效
	UParticleSystem* tHitEffect = DefaultImpactEffect;
	switch (RepFireInfo.SurfaceType)
	{
	case Flesh:
		break;
	case Physic_Head:
		tHitEffect = HeadImpactEffect;
		break;
	default:
		break;
	}

	if (tHitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), tHitEffect, RepFireInfo.HitLoc);
	}

	// 弹道特效
	if (TracerEffect)
	{
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			// 粒子效果参数
			// 在 Particle Emitter 中,右键 Beam -> Target
			// 属性栏中 Target -> Distribution(选择 Vector) -> Parameter Name(给程序的变量名)
			TracerComp->SetVectorParameter(TracerTargetName, RepFireInfo.HitLoc);
		}
	}
}

void ACoopGame_SWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 同步给所有的客户端,除了所有者
	DOREPLIFETIME_CONDITION(ACoopGame_SWeapon, RepFireInfo, COND_SkipOwner);
}