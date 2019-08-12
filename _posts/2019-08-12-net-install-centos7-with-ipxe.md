---
layout: post
title:  使用ipxe安装centos7
date: 2019-08-12 12:00
tags: ipxe centos
categories: public
typora-root-url: ..
---

# 1. 准备
 
- centos7 虚拟机；
- centos7 iso文件；

# 2.配置dnsmasq服务和ipxe基本环境
 
安装需要的包：
 
```bash
yum install -y ipxe-bootimgs dnsmasq
```
 
创建目录
 
```bash
mkdir /tftpboot
# 适用于 BIOS 硬件
cp /usr/share/ipxe/undionly.kpxe /tftpboot/
# 适用于 EFI 硬件
cp /usr/share/ipxe/ipxe.efi /tftpboot/
``` 
编写一个boot.ipxe文件
 
```bash
vi /tftpboot/menu/boot.ipxe
```

内容如下：
 
```bash
#!ipxe
menu PXE Boot Options
item shell iPXE shell
item exit  Exit to BIOS
# 默认选择 exit， 延迟10s
choose --default exit --timeout 10000 option && goto ${option}
# 可以切换到shell
:shell
shell
# 退出
:exit
exit
```
 
修改`/etc/dnsmasq.conf`配置文件
 
```bash
# 禁用dns
port=0
 
# 监听地址和网口
listen-address=192.168.72.2
interface=ens38
 
# 启动tftp
enable-tftp
tftp-root=/tftpboot
 
# 随机分配地址或者静态分配地址
#dhcp-range=192.168.72.200,192.168.72.250,255.255.255.0
dhcp-range=192.168.72.200,static,255.255.255.0
 
# 配置引导文件
dhcp-vendorclass=BIOS,PXEClient:Arch:00000
dhcp-match=set:ipxe,175
dhcp-boot=tag:!ipxe,tag:BIOS,undionly.kpxe
dhcp-boot=tag:!ipxe,tag:!BIOS,ipxe.efi
dhcp-boot=tag:ipxe,menu/boot.ipxe
 
# 自定义mac地址和dhcp客户端
dhcp-host=00:0c:29:55:77:75,192.168.72.202,cn2
dhcp-host=00:0c:29:55:77:74,192.168.72.201,cn1,ignore
```
 
# 启动服务并重启

```bash
systemctl disable firewalld
systemctl enable dnsmasq
sed -i 's/SELINUX=enforcing/SELINUX=disabled/g' /etc/selinux/config
reboot
```
 
# 3. 设置CentOS7网络安装
 
 
安装httpd服务
 
```bash
yum -y install httpd
systemctl start httpd
systemctl enable httpd
```
 
挂载iso到http服务上
 
```bash
mkdir -p /var/www/html/media/centos7
mount -t iso9660 /tmp/CentOS7.iso /var/www/html/media/centos7
# 如果是使用dvd挂载的话：
# mount /dev/sr0 /var/www/html/media/centos7
```
 
 
修改`/tftpboot/menu/boot.ipxe`如下
 
```bash
#!ipxe
:start
menu PXE Boot Options
item shell iPXE shell
item centos7-net CentOS 7 installation
item exit  Exit to BIOS
choose --default centos7-net --timeout 10000 option && goto ${option}
:shell
shell
:centos7-net
set server_root http://192.168.72.2/media/centos7
initrd ${server_root}/images/pxeboot/initrd.img
# 这里没有使用ks文件,如果使用ks自动安装的情况
# kernel ${server_root}/images/pxeboot/vmlinuz inst.ks=${server_root}/centos7.ks ip=dhcp ipv6.disable initrd=initrd.img
kernel ${server_root}/images/pxeboot/vmlinuz inst.repo=${server_root}/ ip=dhcp ipv6.disable initrd=initrd.img inst.geoloc=0 devfs=nomount
boot
:exit
exit
 
```
 
重启网络安装即可。
 
 
# 4. 其他
 
ks文件如下：
 
```bash
[root@vm-centos7 ~]# cat /var/www/html/centos7.ks

#version=DEVEL

# System authorization information

auth --enableshadow --passalgo=sha512

# Use CDROM installation media

# 主要就是这条，需要指明一个安装的repo

url --url http://192.168.72.2/media/centos7/

# Use graphical install

# graphical

text

reboot

selinux --disabled

# Run the Setup Agent on first boot

firstboot --enable

ignoredisk --only-use=sda

# Keyboard layouts

keyboard --vckeymap=us --xlayouts='us'

# System language

lang en_US.UTF-8

 

# Network information

network  --bootproto=dhcp --device=ens33 --onboot=on --noipv6

network  --hostname=localhost

 

# Root password

rootpw --iscrypted $6$ZG/SVZBlg7fGdgjx$hcHX.aoHPxMGbu968ZezzwEYzd6lIieOaU8VBZnojQ0jQG/PNgBsVmbyoM/FaRMDB0T8rHOLHk3DscaXetgOA/

# System services

services --enabled="chronyd"

# System timezone

timezone Asia/Shanghai --isUtc

# System bootloader configuration

clearpart --all --drives=sda

bootloader --append=" crashkernel=auto" --location=mbr --boot-drive=sda

autopart --type=lvm

# Partition clearing information

#clearpart --none --initlabel

 

%packages

@^minimal

@core

chrony

kexec-tools

 

%end

 

%addon com_redhat_kdump --enable --reserve-mb='auto'

 

%end

 

%anaconda

pwpolicy root --minlen=6 --minquality=1 --notstrict --nochanges --notempty

pwpolicy user --minlen=6 --minquality=1 --notstrict --nochanges --emptyok

pwpolicy luks --minlen=6 --minquality=1 --notstrict --nochanges --notempty

%end

```
 
 
 
 
 
 
 

