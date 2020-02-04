# AI
## EQS Environment Querying System
在选定位置附近生成一系列测试点,并根据自定义的规则对这些测试点进行评分,返回合适的测试点数组  
### 应用
在指定位置附近生成敌人,距离玩家越远越好  
流程:  
+ 编辑 -> 编辑器偏好设置,勾选 Environment Querying System  
+ 新建: 人工智能 -> 环境查询,命名为 EQS_FindSpawnLoc  
+ 新建: BP -> EnvQueryContext_BlueprintBase,命名为 EQS_BotSpawns  
+ 编辑 EQS_BotSpawns,重写函数 Provide Actors Set -> Get All Actors of Class(Target Point) -> Return Node, 作为生成测试栅格的Actor  
+ 相同的,新建 EQS_AllPlayers, 返回自定义的玩家类  
+ 编辑 EQS_FindSpawnLoc, 从Root拖出 Points: Grid, 设置 Generate Around: EQS_BotSpawns  
+ 在 SimpleGrid 节点,右键添加测试 Distance,设置 Distance to: EQS_AllPlayers  
+ 
+ 在 GameMode 蓝图中,合适的事件后(如BeginPlay),添加节点 Run EQSQuery(EQS_FindSpawnLoc)  
+ -> 绑定事件到 OnQueryFinished,添加符合签名的自定义事件  
+ Switch Query Status,在 Success 的情况下 -> Get Query Results as Locations(Query Instance),查询到的点的数组  

## 寻路
.build.cs 中添加 "AIModule", "NavigationSystem"  
```
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

FVector ACoopGame_STrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);

	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}
	// 寻路失败
	return GetActorLocation();
}
```