// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XanaduUserWidget.generated.h"

/**
 *
 */
UCLASS()
class UE4MIX_API UXanaduUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 基类没有无参构造函数,子类不能声明无参构造函数
	UXanaduUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

protected:
	class UButton* AvatarButton;

	class UImage* AvatarImage;
	FString AvatarImagePath;

	class UTextBlock* PlayerNameText;
	FString PlayerName;

protected:
	virtual void InitAvatar();

	UFUNCTION()
		virtual void OnClickAvatarButton();
};
