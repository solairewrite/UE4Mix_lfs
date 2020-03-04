# UE4Mix
UE4开发技能合集  
***
## 资源包
使用 Epic Games Launcher 虚幻引擎-库, 安装资源包  
安装方式: 
+ 添加到工程  
+ 复制 Content  
创建工程,然后将Content文件夹下所有的文件,复制到此项目的Content文件夹内  
+ 某些项目需要导入.ini配置文件, Edit -> Project Settings  
  每个模块都有 Export, Import 按钮,尤其注意 Physics, Input  
+ 某些项目需要安装插件, Edit -> Plugins  
  如: 蓝图丢失 Destructible Component, 需要安装破碎组件 Apex Destruction  


| 包名 | 安装方式 | 描述 |
| - | - | - |
| Animation Starter Pack | 添加到工程 | 简单的动画 |
| ContentExamples | 复制 Content | 中文名'内容示例',官方教程项目<br/>位于'虚幻引擎 -> 学习 -> Engine Feature Samples' |
| Ballistics FX | 复制 Content/BallisticsVFX<br/>配置 Physics, Input, Collision, Rendering<br/>安装Plugin: Apex Destruction | FPS音效,视效 |
| SuperGridStarterpack | 复制Content | 各种格子材质 |
| DynamicCombatSystem | 复制Content | 简单的对战,可以释放魔法 |
| Paragon: Greystone | 添加到工程 | 主角 |
| Animal Variety Pack | 添加到工程 | 动物 |



***

+ 添加字体  

新宋体,由于文件较大,所以未上传  
C:\Windows\Fonts\新宋体 常规,在编辑器内拖到 Content/_MyProjects/Fonts  
然后设置 引擎内容/EngineFonts/Roboto, 默认字体族系,为SimSun  

+ 骨骼

骨骼不能移动路径,因为蓝图和动画大量引用骨骼  
如果不传动画包,就需要重新设置 Socket  

## UnrealEngine4
### 网络同步
[网络同步Github文档](https://github.com/solairewrite/UE4Mix/blob/master/Notes/%E7%BD%91%E7%BB%9C%E5%90%8C%E6%AD%A5.md#服务端函数)  
### 物理
[物理Github文档](https://github.com/solairewrite/UE4Mix/blob/master/Notes/%E7%89%A9%E7%90%86.md)  
### 定时器
[定时器Github文档](https://github.com/solairewrite/UE4Mix/blob/master/Notes/%E5%AE%9A%E6%97%B6%E5%99%A8.md)  
### 代理
[代理Github文档](https://github.com/solairewrite/UE4Mix/blob/master/Notes/%E4%BB%A3%E7%90%86.md)  
### 动画
[动画Github文档](https://github.com/solairewrite/UE4Mix/blob/master/Notes/%E5%8A%A8%E7%94%BB.md)  
### UI
[UI Github文档](https://github.com/solairewrite/UE4Mix/blob/master/Notes/UI.md)  
## 常见问题
[报错Github文档](https://github.com/solairewrite/UE4Mix/blob/master/Notes/%E6%8A%A5%E9%94%99.md)  
[常见问题Github文档](https://github.com/solairewrite/UE4Mix/blob/master/Notes/%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98.md)  

## Visual Studio
+ 拉取后C++文件存在,但是VS删选器中不存在  
  右键.uproject生成VS项目文件

+ 修改C++文件路径  
  删除目录下Binaries文件夹,右键.uproject生成VS项目文件  

+ Output窗口乱码  
  原因: 编码  
  解决: 根据情况设置 Windows cmd 编码为 GBK 或 utf-8  
  注册表编辑器 HKEY_CURRENT_USER\Console\%SystemRoot%_system32_cmd.exe  
  修改CodePage为936(GBK), 或65001(utf-8)  


## Git
+ 只拉取最近一次的提交  
`git pull --depth=1 --allow-unrelated-histories`  
+ 远端仓库回滚
```
git reset bb28e5c3c02d64d26e88b3969466139a76755cf0
git push -f
```
+ 记住密码  
`git config credential.helper store`  
+ 合并另一个版本  
`git merge bb28e5c3c02d64d26e88b3969466139a76755cf0 --allow-unrelated-histories`  
+ Failed to connect to github.com port 443: Timed out  
设置代理  
`git config --global http.proxy http://127.0.0.1:1080`  
`git config --global https.proxy http://127.0.0.1:1080`  
查看代理是否设置成功  
`git config --get https.proxy`  
取消代理  
`git config --global --unset https.proxy`  
+ 查看git仓库大小  
打开网页 https://api.github.com/repos/solairewrite/UE4Mix_lfs  
+ git pull 总是失败  
  修改 C:\Windows\System32\drivers\etc\host,删除关于 git 的行  

### git lfs 大文件存储
#### 提交
+ 查看 lfs 版本  
`git lfs -v`  
+ 开启 lfs 功能  
`git lfs install`  
+ 大文件追踪  
`git lfs track *.uasset`  
+ 查看当前追踪模式  
`git lfs track`  
+ 查看当前追踪文件列表,在提交后可使用  
`git lfs ls-files`  
#### 拉取
```
// 可能并不需要这条命令
git lfs install

// 拉取下来的lfs对象存储在 .git/lfs/objects 目录下
// 如果没有
git lfs pull
```
SourceTree可能上传失败,这里用命令行(可以记住密码)成功率较高  
