// Fill out your copyright notice in the Description page of Project Settings.


#include "IHealth.h"

// Add default functionality here for any IIHealth functions that are not pure virtual.

//float IIHealth::GetHealth_Implementation()
//{
//	return 0;
//}

float IIHealth::GetHealth()
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
