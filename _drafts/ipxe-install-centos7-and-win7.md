---
layout: post
title: Install Centos7 and Win7 with iPXE
date: 2018-09-10 12:00
author: felo
tags: pxe
typora-root-url: ../../yangfeiffei.github.io
---

iPXE是一个开源的网络启动固件，它提供完整的PXE功能同时，还有更高级的特性：

- 启动自http服务器
- 启动自iSCSI或者SAN
- 启动自FCOE SAN
- 启动自Infiniband
- 通过脚本控制启动进程

可以通过iPXE去代替现有网卡中的PXE ROM，iPXE是免费的开源软件，可以根据自己的需求修改编译它。

# 0.准备一个iPXE环境

## 0.1 基本原理

`PXE`(`preboot execute environment`，预启动执行环境)，是实现网络安装Linux或者UNIX操作系统的常用方式，其工作方式简单来说如下所示：

- 客户机开机后，选择网卡启动
- 网卡启动过程中，从DHCP服务器中获取一个IP地址，同时从DHCP服务器获得一个next-server的位置，在这个服务器上获得开机启动文件，这个文件可以有两种方式获得
  - `syslinux`：这是一个常用的软件包，里面有开机相关的基本配置，只需要简单修改就可以实现安装操作系统或者网络启动，其中pxelinux.0，就是这个开机启动文件；
  - `ipxe`：这是另一个常用的方式，功能更强大，更灵活。
- 获取Linux内核和initrd镜像文件，启动操作系统或或者安装程序。

## 0.2 操作步骤

- 操作系统环境

```bash
cat /etc/redhat-release
uname -a
```

- 安装几个软件包 

```bash
yum install dhcp httpd xinet
```

- 修改dhcpd.conf

增加一
```bash

```








# net install a centos 7 






# net install a win 7

## make a winpe for win7

## install a win7
