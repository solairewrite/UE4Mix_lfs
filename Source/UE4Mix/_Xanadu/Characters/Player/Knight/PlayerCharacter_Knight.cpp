// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter_Knight.h"
#include "Components/BoxComponent.h"
#include "_Xanadu/Characters/Base/Interfaces/IHealth.h"
//#include "UE4Mix.h"
#include "_Xanadu/Base/XanaduTypes.h"

extern TAutoConsoleVariable<int32> CVARDebugLevel;

APlayerCharacter_Knight::APlayerCharacter_Knight()
{
	MeleeDamageArr[0] = 10.0f;
	MeleeDamageArr[1] = 20.0f;
	MeleeDamageArr[2] = 30.0f;

	MeleeImpulseValue = 1000.0f;
}

void APlayerCharacter_Knight::OnMelee()
{
	// 避免造成基类中的伤害
}

void APlayerCharacter_Knight::OnMelee1()
{
	DoMeleeDamage(MeleeDamageArr[0]);
}

void APlayerCharacter_Knight::OnMelee2()
{
	DoMeleeDamage(MeleeDamageArr[1]);
}

void APlayerCharacter_Knight::OnMelee3()
{
	DoMeleeDamage(MeleeDamageArr[2], true);
}

void APlayerCharacter_Knight::OnSaveAttack()
{
	// 检测是否下达了连击命令
	if (!bHasSaveMeleeCombo)
	{
		return;
	}

	bHasSaveMeleeCombo = false;
	// 播放下一条连击动画
	if (AnimMontage_MeleeArr[CurrMeleeComboIndex])
	{
		PlayAnimMontage(AnimMontage_MeleeArr[CurrMeleeComboIndex]);
		CurrMeleeComboIndex++;
		if (CurrMeleeComboIndex >= MaxMeleeComboCount)
		{
			CurrMeleeComboIndex = 0;
		}
	}
}

void APlayerCharacter_Knight::OnResetCombo()
{
	bIsMeleeing = false;
	bHasSaveMeleeCombo = false;
	CurrMeleeComboIndex = 0;
}

void APlayerCharacter_Knight::Melee()
{
	// 如果已经在近战攻击中,只记录连击命令
	if (bIsMeleeing)
	{
		bHasSaveMeleeCombo = true;
		return;
	}

	bIsMeleeing = true;
	if (AnimMontage_MeleeArr[0])
	{
		PlayAnimMontage(AnimMontage_MeleeArr[0]);
		CurrMeleeComboIndex = 1;
	}
}

void APlayerCharacter_Knight::DoMeleeDamage(float inDamage, bool bAddImpulse /*= false*/)
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

		IIHealth* tHealthActor = Cast<IIHealth>(tActor);
		//if (tActor->GetClass()->ImplementsInterface(UIHealth::StaticClass()))
		if (tHealthActor && tHealthActor->GetHealth() > 0)
		{
			tActor->TakeDamage(inDamage, FDamageEvent(), GetController(), this);

			// 播放受击动画
			if (tHealthActor->CanPlayTakeHitAnim())
			{
				tHealthActor->PlayTakeHitAnim();
			}

			if (bAddImpulse)
			{
				FVector tImpulseDir = tActor->GetActorLocation() - GetActorLocation();
				tImpulseDir.Z = 0;
				tImpulseDir.Normalize();
				AddImpulse(tActor, tImpulseDir, MeleeImpulseValue);
			}

			// 绘制调试信息
			if (CVARDebugLevel.GetValueOnGameThread() > 0)
			{
				FString tName = GetDebugName(tActor);
				//float health = IIHealth::Execute_GetHealth(tActor);
				float health = Cast<IIHealth>(tActor)->GetHealth();
				FString tInfo = FString::Printf(TEXT("%s 血量: %s"), *tName, *FString::SanitizeFloat(health, 0));
				GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Yellow, tInfo);
			}
		}
	}
}

void APlayerCharacter_Knight::AddImpulse(AActor* inActor, FVector inDir, float inValue)
{
	IIHealth* tHealthActor = Cast<IIHealth>(inActor);
	if (tHealthActor && tHealthActor->CanTakeImpulse())
	{
		tHealthActor->TakeImpulse(inDir * inValue);
	}
}
