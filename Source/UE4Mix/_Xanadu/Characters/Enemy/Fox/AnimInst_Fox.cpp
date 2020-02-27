// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst_Fox.h"

void UAnimInst_Fox::AnimNotify_Melee(UAnimNotify* inNotify)
{
	// 打印屏幕
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, TEXT("狐狸动画通知 C++  Melee"));
}
