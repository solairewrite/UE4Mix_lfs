// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "../../Enemy/Base/AICharacterBase.h"
#include "_Xanadu/Characters/Base/Interfaces/IHealth.h"

// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 当关卡开始时或Pawn Spawn时,哪个PC会自动控制这个Pawn
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 90.0f));
	SpringArmComp->TargetArmLength = 500.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true; // 相机延迟移动到目标位置,以平滑移动
	SpringArmComp->bEnableCameraRotationLag = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	MeleeBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeBoxComp"));
	MeleeBoxComp->SetRelativeLocation(FVector(110.0f, 0, 0));
	MeleeBoxComp->SetRelativeScale3D(FVector(3.75f, 4.5f, 2.0f));
	// 设置碰撞
	MeleeBoxComp->SetCollisionObjectType(ECC_Pawn);
	MeleeBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeBoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeBoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeleeBoxComp->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
	MeleeBoxComp->SetupAttachment(RootComponent);

	// 运动
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->RotationRate = FRotator(0, 540.0f, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 伤害
	MeleeDamage = 10.0f;
}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerCharacterBase::Destroyed()
{
	Super::Destroyed();
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 按键绑定
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 移动
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacterBase::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacterBase::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacterBase::Jump);

	// 技能
	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &APlayerCharacterBase::Melee);
}

void APlayerCharacterBase::MoveForward(float inValue)
{
	FRotator tControlRot = GetControlRotation();
	tControlRot.Roll = 0;
	tControlRot.Pitch = 0;

	FVector tForwardDir = UKismetMathLibrary::GetForwardVector(tControlRot);

	AddMovementInput(tForwardDir, inValue);
}

void APlayerCharacterBase::MoveRight(float inValue)
{
	FRotator tControlRot = GetControlRotation();
	tControlRot.Roll = 0;
	tControlRot.Pitch = 0;

	FVector tRightDir = UKismetMathLibrary::GetRightVector(tControlRot);

	AddMovementInput(tRightDir, inValue);
}

FVector APlayerCharacterBase::GetPawnViewLocation() const
{
	return Super::GetPawnViewLocation();
}

FRotator APlayerCharacterBase::GetViewRotation() const
{
	return Super::GetViewRotation();
}

void APlayerCharacterBase::Melee()
{
	if (bIsMeleeing)
	{
		return;
	}
	bIsMeleeing = true;

	if (AnimMontage_Melee)
	{
		float animTime = PlayAnimMontage(AnimMontage_Melee);
		animTime -= 0.2f; // 减去大概的动画融合时间
		animTime = FMath::Clamp(animTime, 0.0f, animTime);
		FTimerHandle tTH;
		GetWorldTimerManager().SetTimer(tTH, this, &APlayerCharacterBase::OnMeleeMontageFinish, animTime);
	}
}

void APlayerCharacterBase::OnMelee()
{
	if (!MeleeBoxComp)
	{
		return;
	}

	TArray<AActor*> tActorArr;
	MeleeBoxComp->GetOverlappingActors(tActorArr);
	for (AActor* tActor : tActorArr)
	{
		if (tActor == this)
		{
			continue;
		}

		FString tName = GetDebugName(tActor);
		float health = 0;
		bool bIsAI = false;

		// 接口检测
		if (tActor->GetClass()->ImplementsInterface(UIHealth::StaticClass()))
		{
			tActor->TakeDamage(MeleeDamage, FDamageEvent(), GetController(), this);
			health = IIHealth::Execute_GetHealth(tActor);
		}

		// 接口检测
		IIHealth* tHealthActor = Cast<IIHealth>(tActor);
		if (tHealthActor)
		{
			bIsAI = tHealthActor->IsAI();
		}

		// 格式化字符串,浮点数需要转为字符串,否则小数点后面有很多0
		FString tInfo = FString::Printf(TEXT("%s 血量: %s, %sAI"),
			*tName, *FString::SanitizeFloat(health, 0), (bIsAI ? TEXT("是") : TEXT("不是")));
		// 打印屏幕,@param Key: 唯一键值,相同的键值只能同时显示一条屏幕Log
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Yellow, tInfo);
	}
}

void APlayerCharacterBase::OnMeleeMontageFinish()
{
	bIsMeleeing = false;
}

