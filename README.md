# MouseSet

#### 介绍
RPA机器人，模拟鼠标点击工具。自动完成一系列鼠标点击、键盘输入任务。

- 已知Issues：
  1. 模拟鼠标拖动，在部分应用无法达到效果。如：无法实现桌面鼠标的拖动，无法拖动文件，无法拖动窗口滚动条；
  



- V4.0版本特性
  1. 支持鼠标录制功能，可以自动将鼠标的单击、双击、拖动动作添加到任务列表；
  2. 支持右击任务修改任务信息；
  3. 支持任务信息保存到本地磁盘文件和从本地磁盘文件加载；
  4. 修复键盘输入在个别应用无效的问题。

* V3.0版本特性：
  对任务进行优化
1. 支持添加任务说明，赋予操作业务逻辑意义；
 	2. 支持选择鼠标操作类型：单击/双击/拖动。

 	issue:
 	1. 鼠标拖动在部分地方无法达到效果，如无法拖动桌面鼠标，无法拖动文件；
 	2. 键盘输入，只对个别应用有效，目前测试可以在QT/Visual Studio/Chrome/Typora中可以，在word/UltraEdit/Explore中无效。

* V2.0版本特性：

	1. 支持任务的复制/删除等基本的任务管理
	2. 支持各种任务的批量执行。

* V1.0版本特性：

	1. 一个简单鼠标点击的重复执行。

#### 软件架构
小工具，没啥架构，能用就行


#### 安装教程

1.  打开直接用

#### 使用说明

1.  通过【添加任务】按钮，添加鼠标点击任务；
2.  可以通过【复制任务】按钮，批量添加重复的鼠标点击任务；
3.  设置好点击任务后，点击【开始模拟】，软件自动根据任务设置完成一系列鼠标点击；
4.  执行过程中可以删除没有执行的任务，也可以添加新的任务；
5.  可以通过【重置】按钮，清除所有任务，恢复初始状态；
6.  可以双击某个未执行的任务，删除该任务;
7.  通过【录制鼠标】可以录制鼠标的单击、双击、拖动；
8.  通过在任务上右击，可以插入任务，修改任务。

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
