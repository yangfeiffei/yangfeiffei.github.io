---
layout: post
title: Kickstart自动部署Centos6(未完成)
date: 2017-06-14 12:00
author: felo
tags: centos pxe
published: false
---


# 1.原理介绍

## 1.1 PXE

## 1.2 kickstart

## 1.3 使用tcpdump抓包测试

tcpdump是一个简单的抓包工具，抓包测试一下在部署一个操作系统中都发生了啥。
> tcpdump参考资料：
- [Linux tcpdump命令详解](http://www.cnblogs.com/ggjucheng/archive/2012/01/14/2322659.html)
- [tcpdump非常实用的抓包实例](http://blog.csdn.net/nanyun2010/article/details/23445223)
- [TCPDUMP man page](http://www.tcpdump.org/manpages/tcpdump.1.html)




# 2.安装测试


## 2.1 安装环境

```bash
[root@centos603 ~]# uname -a
Linux centos603 2.6.32-696.1.1.el6.x86_64 #1 SMP Tue Apr 11 17:13:24 UTC
2017 x86_64 x86_64 x86_64 GNU/Linux
[root@centos603 ~]# cat /etc/redhat-release
CentOS release 6.9 (Final)

# 注意关闭selinux 和 iptables
[root@centos603 ~]# getenforce
Disabled
[root@centos603 ~]# service iptables status
iptables: Firewall is not running.

# IP 地址 和 主机名
[root@centos603 ~]# ip addr show  eth1
3: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP>
mtu 1500 qdisc pfifo_fast state UP qlen 1000
    link/ether 00:50:56:27:3b:d9 brd ff:ff:ff:ff:ff:ff
    inet 10.10.3.167/24 brd 10.10.3.255 scope global eth1
    inet6 fe80::250:56ff:fe27:3bd9/64 scope link
       valid_lft forever preferred_lft forever

[root@centos603 ~]# hostname
centos603
```

## 2.2 开始安装

### 2.2.1 部署DHCP

```bash
# 安装
[root@centos603 ~]# yum -y install dhcp
# 修改配置文件
[root@centos603 ~]# cat /etc/dhcp/dhcpd.conf |grep -v "^#"
subnet 10.10.3.0 netmask 255.255.255.0 {
        range 10.10.3.100 10.10.3.200;
        option subnet-mask 255.255.255.0;
        default-lease-time 21600;
        max-lease-time 43200;
        next-server 10.10.3.167;
        filename "/pxelinux.0";
}

# 指定网卡
[root@centos603 ~]# cat /etc/sysconfig/dhcpd
# Command line options here
DHCPDARGS=eth1

# 启动dhcp
[root@centos603 ~]# service dhcpd restart
Shutting down dhcpd:                                       [  OK  ]
Starting dhcpd:                                            [  OK  ]
# 查看服务启动情况
[root@centos603 ~]# netstat -nltup |grep 67
udp        0      0 0.0.0.0:67      0.0.0.0:*         3033/dhcpd
```

### 2.2.2 部署tftp

```bash
[root@centos603 ~]# yum install tftp-server -y
[root@centos603 ~]# cat /etc/xinetd.d/tftp
service tftp
{
        socket_type             = dgram
        protocol                = udp
        wait                    = yes
        user                    = root
        server                  = /usr/sbin/in.tftpd
        server_args             = -s /var/lib/tftpboot
        disable                 = no  # yes修改为no
        per_source              = 11
        cps                     = 100 2
        flags                   = IPv4
}
[root@centos603 ~]# /etc/init.d/xinetd restart
Stopping xinetd:                                           [  OK  ]
Starting xinetd:                                           [  OK  ]
[root@centos603 ~]# netstat -tnulp |grep 69
udp        0      0 0.0.0.0:69              0.0.0.0:*       3365/xinetd
```


### 2.2.3 部署HTTP

```bash
[root@centos603 ~]# yum install httpd -y
[root@centos603 ~]# cat /etc/httpd/conf/httpd.conf |grep ServerName
# ServerName gives the name and port that the server uses to identify itself.
#ServerName www.example.com:80
ServerName 127.0.0.1:80

[root@centos603 ~]# mkdir /var/www/html/CentOS-6.9/
[root@centos603 ~]# /bin/mount /dev/cdrom /var/www/html/CentOS-6.9/

[root@centos603 ~]# /etc/init.d/httpd restart
```

### 2.2.4 PXE配置 default 文件




### 2.2.5 编写 ks.cfg 文件


### 2.2.6 增加开机优化脚本





## 2.3 测试




# 3.其他说明

## 3.1 参考

## 3.2 其他
