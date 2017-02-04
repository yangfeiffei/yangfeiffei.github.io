---
layout: post
title: 使用虚拟机部署devstack
author: felo
---

摘要：
记录两个节点的devstack安装和部署过程。



本文大量参考[每天5分钟玩转Openstack](http://cloudman.cc/), 记录了部署的过程


# 1. 环境介绍
创建两个虚拟机，devstack-controller和devstack-compute


## 1.1 网卡配置：


- devstack-controller：
    - eth0：10.10.1.161/24
    - eth1：
    - eth2：
- devstack-compute
    - eth0：10.10.1.162/24
    - eth1：
- 备注：
    - eth0：管理网络+API网络  （需要能上网，下载安装包）
    - eth1：vm网络（tenant网络）或项目网络
    - eth2：外部网络

## 1.2 其他配置：
- devstack-controler：
    - CPU：2
    - 内存：4GB
    - 硬盘：80GB
    - intel vt-x
- devstack-compute：
    - CPU：2
    - 内存：2GB
    - 硬盘：40GB
    - intel vt-x

# 2.部署虚拟机
这里使用的是ubuntu1404。可以手动部署两台虚拟机，也可以使用链接克隆两台虚拟机，
注意网卡的MAC地址修改下。注意安装的时候选择Openssh Server和Virtual Machine两个软件包。


## 2.1 首先部署controller


### 2.1.1 root可远程ssh
安装openssh-server：

```
apt-get install openssh-server -y
```

修改sshd_config配置文件（/etc/ssh/sshd_config）

```
PermitRootLogin yes
```

重启ssh服务

```
service ssh restart
```

修改root用户密码

```
passwd root
```

### 2.1.2 修改配置IP地址
修改配置文件

```
root@ubuntu-test:~# cat /etc/network/interfaces
# interfaces(5) file used by ifup(8) and ifdown(8)
auto lo
iface lo inet loopback

auto eth0
iface eth0 inet static
address 10.10.1.161
netmask 255.255.255.0
gateway 10.10.1.2
dns-nameservers 10.10.1.2

auto eth1
iface eth1 inet manual

auto eth2
iface eth2 inet manual
```

重启服务器

```
reboot
```

### 2.1.3 修改apt源

```
vim /etc/apt/sources.list
deb http://mirrors.aliyun.com/ubuntu/ xenial main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse
```

更新一下：

```
apt-get update
```


# 3.部署openstack

安装很慢，主要原因是网络问题。注意修改源

## 3.1部署controller

### 3.1.1 安装pip
pip是用户安装python包的工具

```
apt-get install python-pip
安装完貌似有问题，用下面的方法安装：
wget https://bootstrap.pypa.io/get-pip.py  --no-check-certificate
python get-pip.py
```

### 3.1.2 下载devstack

```
apt-get install git -y  #没有git的安装一下
git clone https://git.openstack.org/openstack-dev/devstack -b stable/newton
下载好慢，用下面这个试试：
git clone https://github.com/openstack-dev/devstack.git -b stable/newton
```

执行创建用户

```
root@devstack-controller:~# ./devstack/tools/create-stack-user.sh
Creating a group called stack
Creating a user called stack
Giving stack user passwordless sudo privileges

```

### 3.1.3 为root和stack用户配置pip源


```
root@devstack-controller:~# mkdir ~/.pip
root@devstack-controller:~# vim ~/.pip/pip.conf
root@devstack-controller:~# su - stack
stack@devstack-controller:~$ mkdir ~/.pip
stack@devstack-controller:~$ vim ~/.pip/pip.comf
stack@devstack-controller:~$ exit
```

其中pip.conf内容为：豆瓣网的pip源，不太好，有些包不行。

```
[global]
index-url = https://pypi.douban.com/simple
download_cache = ~/.cache/pip
[install]
use-mirrors = true
mirrors = http://pypi.douban.com/
```

### 3.1.4 配置local.conf

```
root@devstack-controller:~# cp -r devstack/ /opt/stack/
root@devstack-controller:~# chown -R stack:stack /opt/stack/devstack/
root@devstack-controller:~# su - stack
stack@devstack-controller:~$ pwd
/opt/stack
stack@devstack-controller:~$ cd devstack/
stack@devstack-controller:~/devstack$ vim local.conf
stack@devstack-controller:~/devstack$ cat local.conf
[[local|localrc]]
#多节点
MULTI_HOST=true  

# management & api network
HOST_IP=10.10.1.161
LOGFILE=/opt/stack/logs/stack.sh.log

# Credentials
ADMIN_PASSWORD=admin
MYSQL_PASSWORD=secret
RABBIT_PASSWORD=secret
SERVICE_PASSWORD=secret
SERVICE_TOKEN=abcdefghijklmnopqrstuvwxyz

# enable neutron-ml2-vlan
disable_service n-net
enable_service q-svc,q-agt,q-dhcp,q-l3,q-meta,neutron,q-lbaas,q-fwaas
Q_AGENT=linuxbridge
ENABLE_TENANT_VLANS=True
TENANT_VLAN_RANGE=3001:4000
PHYSICAL_NETWORK=default

LOG_COLOR=True
LOGDIR=$DEST/logs
SCREEN_LOGDIR=$LOGDIR/screen

# use TryStack git mirror
GIT_BASE=http://git.trystack.cn
NOVNC_REPO=http://git.trystack.cn/kanaka/noVNC.git
SPICE_REPO=http://git.trystack.cn/git/spice/spice-html5.git
```

### 3.1.5 开始部署

```
./devstack/stack.sh
```

说明
过程很长很长，而且很容易失败,
失败就再运行一次部署脚本，直到成功。



# 4.问题解决

## 4.1  generate-subunit not found
```
问题：
2017-01-02 10:34:09.935 | Error on exit
2017-01-02 10:34:09.942 | +./stack.sh:exit_trap:501                  generate-subunit 1483353211 38 fail
2017-01-02 10:34:09.942 | ./stack.sh: line 501: generate-subunit: command not found
解决：
root@devstack-controller:~# pip install -U os-testr
```

## 4.2 安装的时候总有python包错误

解决办法：
- 不使用豆瓣网的pip源；
- 不使用国内的TryStack镜像
