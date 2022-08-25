# 精简版第三人称射击局域网游戏

## 简介

游戏演示及可执行文件：

https://pan.baidu.com/s/1dHuRHf0wgYaQ_soSjMhN8g 提取码：2048

## 游戏截图
![](https://github.com/olleh-dlrow/ProtectionPVE/blob/master/readme/QQ%E6%88%AA%E5%9B%BE20220825151302.jpg)

![](https://github.com/olleh-dlrow/ProtectionPVE/blob/master/readme/QQ%E6%88%AA%E5%9B%BE20220825151640.jpg)

![](https://github.com/olleh-dlrow/ProtectionPVE/blob/master/readme/QQ%E6%88%AA%E5%9B%BE20220825151857.jpg)

![](https://github.com/olleh-dlrow/ProtectionPVE/blob/master/readme/QQ%E6%88%AA%E5%9B%BE20220825151928.jpg)

## 启动方法

1. 运行NetworkTest/WindowServer/Server.bat
1. 运行NetworkTest/WindowsNoEditor/ProtectionPVE.exe
1. 重复步骤2

## 实现功能
射击游戏常用UI实现，如弹药数量、生命值、游戏倒计时

人物动画，包括下蹲、奔跑、射击等姿势的混合

部分枪械及特效实现，如手枪、榴弹枪、冲锋枪

场地掉落物体拾取

子弹物理碰撞效果

使用行为树实现敌人和己方NPC的交互动作，包括敌人刷新、死亡、近战攻击效果，NPC的路径规划

使用RPC建立局域网游戏，实现状态同步

## 操作说明

移动：WSAD
开火：鼠标左键
更换武器：1, 2
拾起武器：E
投掷手雷：T
跳跃：Space
蹲下：C
换弹：R
退出对局：Esc

## 游戏规则

玩家在限定时间内保护NPC到达目标地点

