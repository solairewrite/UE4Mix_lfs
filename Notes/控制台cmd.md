# 控制台cmd
## 控制台修改变量  
```
// 声明控制台变量
static int32 DebugHealth = 0;
FAutoConsoleVariableRef CVARDebugHealth(
	TEXT("Coop.DebugHealth"), // 控制台命令字符
	DebugHealth,
	TEXT("Show Debug String of Health Component"),
	ECVF_Cheat
);

// 控制台变量决定是否显示log
if (DebugHealth >= 1)
{
    UE_LOG(LogTemp, Warning, TEXT("血量改变: %s"), *FString::SanitizeFloat(Health));
}
```