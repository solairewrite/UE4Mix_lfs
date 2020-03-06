// Fill out your copyright notice in the Description page of Project Settings.


#include "IHealth.h"

float IIHealth::GetHealth_Implementation()
{
	return 0;
}

float IIHealth::GetHealthMax_Implementation()
{
	return 0;
}

bool IIHealth::IsAI()
{
	return false;
}

void IIHealth::OnAttackBy(class AController* InstigatedBy, AActor* DamageCauser)
{

}

void IIHealth::OnDead()
{

}

bool IIHealth::CanPlayTakeHitAnim()
{
	return false;
}

void IIHealth::PlayTakeHitAnim()
{

}

bool IIHealth::CanTakeImpulse()
{
	return false;
}

void IIHealth::TakeImpulse(FVector inImpulseVector)
{

}
