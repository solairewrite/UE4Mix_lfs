// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 90.0f));
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true; // 相机延迟移动到目标位置,以平滑移动
	SpringArmComp->bEnableCameraRotationLag = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// 运动
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->RotationRate = FRotator(0, 540.0f, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;
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
	if (AnimMontage_Melee)
	{
		PlayAnimMontage(AnimMontage_Melee);
	}
}

