// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

/**
 * 游戏枚举,结构体
 */

// 声明控制台变量
static TAutoConsoleVariable<int32> CVARDebugLevel(
	TEXT("DebugLevel"),
	0,
	TEXT("调试等级,控制是否显示调试球等,数值越大,能显示的越多"),
	ECVF_Cheat
);

/** AI 状态 */
UENUM(BlueprintType)
enum class EAIState :uint8
{
	None,
	Escape, // 逃跑
	Sleep,
	Rest, // 休息,如坐下
	Idle,
	Patrol, // 正常巡逻
	Alert, // 警觉,如听到声音,看到尸体
	SearchTarget, // 搜索攻击目标,如交战后丢失玩家的寻找阶段
	Attack,
};

/** AI Action 状态 */
UENUM(BlueprintType)
enum class EActionState :uint8
{
	None,
	Doing,
	Success,
	Fail,
	Abort, // 外部终止Action,如丢失目标
};

/** AI Command 状态 */
UENUM(BlueprintType)
enum class ECommandState :uint8
{
	None,
	Doing,
	Pause,
	Success,
	Fail,
};

/** 上一条命令失败时该如何处理此条命令 */
UENUM(BlueprintType)
enum class EDoWhatOnLastCommandFail :uint8
{
	Execute,
	ReDoLastCommand,
	Skip,
	ActionFail,
};

/** 旋转方向 */
UENUM(BlueprintType)
enum class ERotateDirection :uint8
{
	None,
	Left,
	Right,
};
