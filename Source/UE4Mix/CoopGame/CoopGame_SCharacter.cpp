// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_SCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CoopGame/CoopGame_SWeapon.h"
#include "Engine/World.h"
#include "UE4Mix.h"
#include "Components/CapsuleComponent.h"
#include "CoopGame_SHealthComponent.h"
//#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACoopGame_SCharacter::ACoopGame_SCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true; // 使用玩家旋转
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// 允许下蹲
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// 右键瞄准
	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20;

	WeaponAttackSocketName = "WeaponSocket";

	// 碰撞设置,禁止子弹打到胶囊体,只允许打到Mesh
	GetCapsuleComponent()->SetCollisionResponseToChannel(Collision_WeaponFire, ECR_Ignore);

	// 添加血量组件
	HealthComp = CreateDefaultSubobject<UCoopGame_SHealthComponent>(TEXT("HealthComp"));
}

// Called when the game starts or when spawned
void ACoopGame_SCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;
	// 血量组件添加代理
	HealthComp->OnHealthChanged.AddDynamic(this, &ACoopGame_SCharacter::OnHealthChanged);

	if (Role == ROLE_Authority)
	{
		// 生成默认武器
		CreateAndSetWeapon(StarterWeaponClass);
	}
}

void ACoopGame_SCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void ACoopGame_SCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

// Called every frame
void ACoopGame_SCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 右键缩放
	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	// 差值计算
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComp->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void ACoopGame_SCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 按键绑定
	PlayerInputComponent->BindAxis("MoveForward", this, &ACoopGame_SCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACoopGame_SCharacter::MoveRight);
	// 鼠标移动视角
	PlayerInputComponent->BindAxis("LookUp", this, &ACoopGame_SCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ACoopGame_SCharacter::AddControllerYawInput);
	// 下蹲
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACoopGame_SCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACoopGame_SCharacter::EndCrouch);
	// 跳跃
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACoopGame_SCharacter::Jump);
	// 瞄准
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ACoopGame_SCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ACoopGame_SCharacter::EndZoom);
	// 开火
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACoopGame_SCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACoopGame_SCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACoopGame_SCharacter::StopFire);
}

void ACoopGame_SCharacter::MoveForward(float Value)
{
	// 控制玩家前进
	//AddMovementInput(GetActorForwardVector() * Value);
	FRotator ControlRot(0, GetControlRotation().Yaw, 0);
	FVector ControlForward = UKismetMathLibrary::GetForwardVector(ControlRot);
	AddMovementInput(ControlForward * Value);
}

void ACoopGame_SCharacter::MoveRight(float Value)
{
	// 控制玩家向右移动
	//AddMovementInput(GetActorRightVector() * Value);
	FRotator ControlRot(0, GetControlRotation().Yaw, 0);
	FVector ControlRight = UKismetMathLibrary::GetRightVector(ControlRot);
	AddMovementInput(ControlRight * Value);
}

void ACoopGame_SCharacter::BeginCrouch()
{
	Crouch();
}

void ACoopGame_SCharacter::EndCrouch()
{
	UnCrouch();
}

FVector ACoopGame_SCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		// 获取组件世界空间坐标
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ACoopGame_SCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ACoopGame_SCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void ACoopGame_SCharacter::Fire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void ACoopGame_SCharacter::CreateAndSetWeapon(TSubclassOf<ACoopGame_SWeapon> WeaponClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACoopGame_SWeapon* tWeapon = GetWorld()->SpawnActor<ACoopGame_SWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (tWeapon)
	{
		CurrentWeapon = tWeapon;
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttackSocketName);
	}
}

void ACoopGame_SCharacter::OnHealthChanged(UCoopGame_SHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// 死亡
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;
		// 停止移动
		GetMovementComponent()->StopMovementImmediately();
		// 禁用胶囊体组件碰撞
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 使Pawn与PC分离,等待销毁
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}

void ACoopGame_SCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 同步
	DOREPLIFETIME(ACoopGame_SCharacter, CurrentWeapon);

	DOREPLIFETIME(ACoopGame_SCharacter, bDied);
}