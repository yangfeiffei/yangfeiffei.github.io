---
layout: post
title: Openstack学习笔记-基础知识-KVM-1
date: 2017-12-09 12:00
author: felo
tags: openstack kvm
---


>基于内核的虚拟机（英语：Kernel-based Virtual Machine，缩写为 KVM），是一种用于Linux内核中的虚拟化基础设施，可以将Linux内核转化为一个hypervisor。KVM在2007年2月被导入Linux 2.6.20核心中，以可加载核心模块的方式被移植到FreeBSD及illumos上。

KVM在具备Intel VT或AMD-V功能的x86平台上运行。它也被移植到S/390[1]，PowerPC[2]与IA-64平台上。在Linux内核3.9版中，加入ARM架构的支持[3]。

KVM目前由Red Hat等厂商开发，对CentOS/Fedora/RHEL等Red Hat系发行版支持极佳。

- KVM是开源软件，全称是kernel-based virtual machine（基于内核的虚拟机）。
- 是x86架构且硬件支持虚拟化技术（如 intel VT 或 AMD-V）的Linux全虚拟化解决方案。
- 它包含一个为处理器提供底层虚拟化 可加载的核心模块kvm.ko（kvm-intel.ko或kvm-AMD.ko）。
- KVM还需要一个经过修改的QEMU软件（qemu-kvm），作为虚拟机上层控制和界面。
- KVM能在不改变linux或windows镜像的情况下同时运行多个虚拟机，（它的意思是多个虚拟机使用同一镜像）并为每一个虚拟机配置个性化硬件环境（网卡、磁盘、图形适配器……）同时KVM还能够使用ksm技术帮助宿主服务器节约内存。
- 在主流的Linux内核，如2.6.20以上的内核均已包含了KVM核心。

上述摘自维基百科-![KVM](https://zh.wikipedia.org/wiki/%E5%9F%BA%E4%BA%8E%E5%86%85%E6%A0%B8%E7%9A%84%E8%99%9A%E6%8B%9F%E6%9C%BA)

# 1.安装

```bash

[root@localhost ~]# uname -a
Linux localhost.localdomain 3.10.0-693.el7.x86_64 #1 SMP Tue Aug 22 21:09:27 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
[root@localhost ~]# cat /etc/redhat-release 
CentOS Linux release 7.4.1708 (Core) 


# 内核
[root@localhost 3.10.0-693.el7.x86_64]# find . -name kvm*
./kernel/arch/x86/kvm
./kernel/arch/x86/kvm/kvm-amd.ko.xz
./kernel/arch/x86/kvm/kvm-intel.ko.xz
./kernel/arch/x86/kvm/kvm.ko.xz
./kernel/drivers/gpu/drm/i915/gvt/kvmgt.ko.xz
[root@localhost 3.10.0-693.el7.x86_64]# pwd
/lib/modules/3.10.0-693.el7.x86_64

# 包
[root@localhost ~]# yum grouplist 
Loaded plugins: fastestmirror, langpacks
Loading mirror speeds from cached hostfile
Installed Environment Groups:
   Virtualization Host
...

[root@localhost ~]# rpm -qa |grep -E "^virt*"
virt-who-0.19-5.el7.noarch
virt-top-1.0.8-23.el7.x86_64
virt-manager-1.4.1-7.el7.noarch
virtuoso-opensource-6.1.6-6.el7.x86_64
virt-manager-common-1.4.1-7.el7.noarch
virt-viewer-5.0-7.el7.x86_64
virt-what-1.13-10.el7.x86_64
virt-install-1.4.1-7.el7.noarch

# virt-install：命令行工具
# virt-manager：图形工具
```

```bash
# virt-install
[root@localhost ~]# virt-install 
WARNING  KVM acceleration not available, using 'qemu'
ERROR    
--name is required
--memory amount in MiB is required
--disk storage must be specified (override with --disk none)
An install method must be specified
(--location URL, --cdrom CD/ISO, --pxe, --import, --boot hd|cdrom|...)
```

![](/images/openstack-basic-kvm/virt-manager-cut.png)



# 2.创建一个虚拟机

使用`virt-manager`的方法和使用virtualBox或者vmware workstation差不多，
不重复了。这里使用virt-install命令行创建虚拟机。

```bash
# virt-install创建一个最简单的虚拟机

# 从 http://download.cirros-cloud.net/0.3.3/ 下载一个镜像，作为虚拟机的硬盘
# 太高的版本使用的是qcow3，不能使用，这里使用0.3.3
[root@localhost data]# file cirros-0.3.3-x86_64-disk.img 
cirros-0.3.3-x86_64-disk.img: QEMU QCOW Image (v2), 41126400 bytes

# 开始创建
[root@localhost ~]# virt-install -n c01 \
-r 128 \
--disk path=/data/cirros-0.3.3-x86_64-disk.img,format=qcow2 \
--network=default \
--boot hd \
--noautoconsole
WARNING  KVM acceleration not available, using 'qemu'
WARNING  No operating system detected, VM performance may suffer. Specify an OS with --os-variant for optimal results.

Starting install...
Domain creation completed.

# -n：c01，虚拟机名称
# -r：128，内存128M
# --disk：硬盘情况
# --network：网络情况
# --boot：从硬盘启动，也可以使用hd,network
# --noautoconsole：默认不弹出启动窗口

# 查看安装情况
[root@localhost ~]# virsh 
Welcome to virsh, the virtualization interactive terminal.

Type:  'help' for help with commands
       'quit' to quit

virsh # list --all
 Id    Name                           State
----------------------------------------------------
 2     c01                            running

virsh # console 2
Connected to domain c01
Escape character is ^]
[    0.000000] Initializing cgroup subsys cpuset
[    0.000000] Initializing cgroup subsys cpu
...
=== datasource: None None ===
=== cirros: current=0.3.3 uptime=261.20 ===
  ____               ____  ____
 / __/ __ ____ ____ / __ \/ __/
/ /__ / // __// __// /_/ /\ \ 
\___//_//_/  /_/   \____/___/ 
   http://cirros-cloud.net


login as 'cirros' user. default password: 'cubswin:)'. use 'sudo' for root.
cirros login: 

```

```bash
# 创建一个centos7虚拟机

[root@localhost data]# qemu-img create -f qcow2 centos7-1.qcow2 10G
Formatting 'centos7-1.qcow2', fmt=qcow2 size=10737418240 encryption=off cluster_size=65536 lazy_refcounts=off 
[root@localhost data]# virt-install -n centos7-1 \
-r 512 \
--disk path=/data/centos7-1.qcow2,format=qcow2 \
--network=default \
--cdrom=/data/centos7.iso \
--graphics vnc,password=123456,listen=0.0.0.0 \
--noautoconsole

# qemu-img 创建一个虚拟机使用的硬盘；
# --cdrom 为虚拟机增加一个光驱，注意iso格式，还需要调整这个iso的权限
# --graphics 图形界面，默认vnc
```

# 3.virsh管理虚拟机（简）

```bash
# 列出所有虚拟机
virsh # list --all
 Id    Name                           State
----------------------------------------------------
 2     c01                            running

# 温柔得关闭虚拟机
virsh # shutdown 2
Domain 2 is being shutdown

virsh # list --all
 Id    Name                           State
----------------------------------------------------
 -     c01                            shut off

# 启动虚拟机
virsh # start c01
Domain c01 started

virsh # list --all
 Id    Name                           State
----------------------------------------------------
 4     c01                            running

# 强行关闭虚拟机
virsh # destroy c01
Domain c01 destroyed

virsh # list --all
 Id    Name                           State
----------------------------------------------------
 -     c01                            shut off

# 删除虚拟机，不会删除虚拟机的磁盘文件
virsh # undefine c01
Domain c01 has been undefined

virsh # list --all
 Id    Name                           State
----------------------------------------------------

```

>
- 虚拟机的id，每次主机临时生成，不固定；
- `list`只列举在运行的虚拟机；
