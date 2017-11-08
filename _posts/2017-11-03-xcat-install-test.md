---
layout: post
title: CentOS6.5 xCAT Test
date: 2017-11-03 12:00
author: felo
tags: centos xcat
---



# 0. xcat简介

> [xcat官方文档](http://xcat-docs.readthedocs.io/en/stable/)中说明：
>	xCAT enables the administrator to:
- Discover the hardware servers
- Execute remote system management
- Provision operating systems on physical or virtual machines
- Provision machines in Diskful (stateful) and Diskless (stateless)
- Install and configure user applications
- Parallel system management
- Integrate xCAT in Cloud

Github地址：https://github.com/xcat2/xcat-core/

xCat拓扑：

![](/images/xcat-test/xcat-top.png)


节点类型：
- xcat管理节点（managementNode）：xcat主要管理节点
- 服务节点（serviceNode）：如果管理的节点很多，可以减少管理管理节点负担；
- 计算节点（computeNode）：应用节点

网络类型：
- 管理网络（mgmtNetwork）：所有节点之间的连接；
- 服务网络（serviceNetwork）：SP之间连接，其中管理节点需要使用一个操作系统网卡进行连接；
- 应用网络（applicationNetwork）：计算节点之间通信；

其他：
- 网络服务：dhcp，tftp，http等
- SP:硬件管理，如Integrated Management Module (IMM), Flexible Service Processor (FSP), Baseboard Management Controller (BMC)
- RestAPIs：对外API；

## 0.1 快速安装手册

可以按照下面的几个步骤：
- 找一个mgtNode服务器，集群越大，配置要求越高，都是废话；
- 安装xcat管理节点
- 启动xcat管理节点
- 发现目标节点，小集群可以手动收集信息，然后mkdef来定义；大集群可以自动发现，使用lsdef发现；
- 可以对目标节点进行硬件操作，比如开关机之类的；
- 目标节点部署操作系统；
- 升级操作系统：安装软件、同步文件、执行脚本等；
- 执行并行命令

## 0.2 操作系统和硬件支持列表

--|Power|	Power LE	|zVM	|Power KVM	|x86_64|	x86_64 KVM	|x86_64 Esxi
----|--|--|--|--|--|--|--|
RHEL    |	yes	  |yes|	yes	|yes  |	yes	|yes	|yes
SLES    |	yes	  |yes|	yes	|yes  |	yes	|yes	|yes
Ubuntu	|no	    |yes|	no	|yes  |	yes	|yes	|yes
CentOS	|no	    |no	|no		|no	  |yes	|yes	|yes
AIX			|yes    |no	|no	  |no		|no	  |no	  |no
Windows	|no	    |no	|no		|no	  |yes	|yes	|yes

# 1. 安装测试

## 1.1 管理节点安装（Automatic Install Using go-xcat）

> 参考：http://xcat-docs.readthedocs.io/en/stable/guides/install-guides/yum/automatic_install.html

```bash

# 管理节点环境
[root@localhost ~]# uname -a
Linux localhost 2.6.32-431.el6.x86_64 #1 SMP Fri Nov 22 03:15:09 UTC 2013 x86_64 x86_64 x86_64 GNU/Linux
[root@localhost ~]# cat /etc/redhat-release 
CentOS release 6.5 (Final)

# 修改主机名
[root@localhost ~]# hostname xcatmn
[root@localhost ~]# sed -i "s#HOSTNAME=localhost.localdomain#HOSTNAME=xcatmn#g" /etc/sysconfig/network

# 自动安装xcat
[root@xcatmn ~]# yum -y install wget
...
[root@xcatmn ~]# wget https://raw.githubusercontent.com/xcat2/xcat-core/master/xCAT-server/share/xcat/tools/go-xcat -O - >/tmp/go-xcat
...
[root@xcatmn ~]# chmod +x /tmp/go-xcat
[root@xcatmn ~]# yum install yum-plugin-security -y
# 安装最新稳定版本 xCAT
[root@xcatmn ~]# /tmp/go-xcat install    
# 完成后按照提示修改环境变量
[root@xcatmn ~]# source /etc/profile.d/xcat.sh

```
## 1.2 管理节点安装（手动安装）

```bash
# 待续

```



# 2. xcat配置

## 2.1 基本概念

xCAT Objects：可以被xcat进行管理的对象；每个对象都有一组属性；
- node：物理机、虚拟机、SP都可以定义为node
- group：包含多个node的一个组；组的属性可以被node继承；
- osimage：一个可以被xcat部署的操作系统镜像
- 操作命令：
	- mkdef : create object definitions
	- chdef : modify object definitions
	- lsdef : list object definitions
	- rmdef : remove object definitions



xCAT Database：所有的对象都被保存在数据库中(node, group, network, osimage, policy ... and global configuration)
- tabedit:修改表；
- tabdump:显示表；

Global Configuration：xcat全局配置；


xCAT Network：包括硬件发现/管理，OS部署和配置，应用安装和配置；
 
