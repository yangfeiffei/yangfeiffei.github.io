---
layout: post
title: SSH互信配置
date: 2017-10-06 21:00
author: felo
tags: centos ssh
---



# 0.环境配置


准备了三个虚拟机c01,c02,c03，配置ssh互信。

```bash
[root@c01 ~]# cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
192.168.56.101 c01
192.168.56.102 c02
192.168.56.103 c03
[root@c01 ~]# uname -a
Linux c01 2.6.32-696.10.3.el6.x86_64 #1 SMP Tue Sep 26 18:14:22 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
[root@c01 ~]# cat /etc/redhat-release
CentOS release 6.5 (Final)
```


# 1.方法1：使用ssh-copy-id配置

![](https://yangfeiffei.github.io/2017/09/26/gpfs%E5%AE%89%E8%A3%85%E9%83%A8%E7%BD%B2%E6%B5%8B%E8%AF%95.html)


# 2.方法2：手动拷贝

## 2.1 zhunbei
[root@c01 ~]# for i in 1 2 3 ;do ping c0$i -c 3;done
PING c01 (192.168.56.101) 56(84) bytes of data.
64 bytes from c01 (192.168.56.101): icmp_seq=1 ttl=64 time=0.029 ms
64 bytes from c01 (192.168.56.101): icmp_seq=2 ttl=64 time=0.043 ms
64 bytes from c01 (192.168.56.101): icmp_seq=3 ttl=64 time=0.048 ms

--- c01 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 1999ms
rtt min/avg/max/mdev = 0.029/0.040/0.048/0.008 ms
PING c02 (192.168.56.102) 56(84) bytes of data.
64 bytes from c02 (192.168.56.102): icmp_seq=1 ttl=64 time=1.01 ms
64 bytes from c02 (192.168.56.102): icmp_seq=2 ttl=64 time=0.481 ms
64 bytes from c02 (192.168.56.102): icmp_seq=3 ttl=64 time=0.485 ms

--- c02 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2003ms
rtt min/avg/max/mdev = 0.481/0.661/1.019/0.254 ms
PING c03 (192.168.56.103) 56(84) bytes of data.
64 bytes from c03 (192.168.56.103): icmp_seq=1 ttl=64 time=1.92 ms
64 bytes from c03 (192.168.56.103): icmp_seq=2 ttl=64 time=0.592 ms
64 bytes from c03 (192.168.56.103): icmp_seq=3 ttl=64 time=0.585 ms

--- c03 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2004ms
rtt min/avg/max/mdev = 0.585/1.033/1.922/0.628 ms



## 2.2 config nfs server 
[root@c01 ~]# yum list |grep -E "rpcbind|nfs-utils"
nfs-utils.x86_64                           1:1.2.3-75.el6              base
nfs-utils-lib.i686                         1.1.5-13.el6                base
nfs-utils-lib.x86_64                       1.1.5-13.el6                base
nfs-utils-lib-devel.i686                   1.1.5-13.el6                base
nfs-utils-lib-devel.x86_64                 1.1.5-13.el6                base
rpcbind.x86_64                             0.2.0-13.el6_9.1            updates
[root@c01 ~]# yum -y install rpcbind nfs-utils


# server node
[root@c01 ~]# mkdir c01ssh
[root@c01 ~]# chmod -R 777 c01ssh/
[root@c01 ~]# vi /etc/exports
[root@c01 ~]# service rpcbind restart;service nfs restart
Stopping rpcbind:                                          [  OK  ]
Starting rpcbind:                                          [  OK  ]
Shutting down NFS daemon:                                  [  OK  ]
Shutting down NFS mountd:                                  [  OK  ]
Shutting down NFS services:                                [  OK  ]
Shutting down RPC idmapd:                                  [  OK  ]
Starting NFS services:                                     [  OK  ]
Starting NFS mountd:                                       [  OK  ]
Starting NFS daemon:                                       [  OK  ]
Starting RPC idmapd:                                       [  OK  ]
[root@c01 ~]# cat /etc/exports
/root/c01ssh *(insecure,rw,async,no_root_squash)

[root@c01 ~]# showmount -e
Export list for c01:
/root/c01ssh *

# client node
[root@c02 ~]# mount -t nfs c01:/root/c01ssh /root/c01ssh
[root@c02 ~]# df
Filesystem                 1K-blocks    Used Available Use% Mounted on
/dev/mapper/vg_c01-lv_root   6795192 1140904   5302444  18% /
tmpfs                         510024       0    510024   0% /dev/shm
/dev/sda1                     487652   51796    410256  12% /boot
c01:/root/c01ssh             6795264 1140864   5302656  18% /root/c01ssh



# ssh key

[root@c01 ~]# ssh-keygen
Generating public/private rsa key pair.
Enter file in which to save the key (/root/.ssh/id_rsa):
Created directory '/root/.ssh'.
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /root/.ssh/id_rsa.
Your public key has been saved in /root/.ssh/id_rsa.pub.
The key fingerprint is:
63:68:d4:0f:82:9f:e2:84:3d:dc:10:5d:07:da:8f:12 root@c01
The key's randomart image is:
+--[ RSA 2048]----+
|    .. .o..      |
|     o.+ .       |
|    o E +        |
|   + = = =       |
|  . * B S o      |
|   o + o .       |
|    .            |
|                 |
|                 |
+-----------------+


[root@c01 ~]# sed -i "/StrictHostKeyChecking/a StrictHostKeyChecking no" /etc/ssh/ssh_config
[root@c01 ~]# cat /etc/ssh/ssh_config |grep StrictHostKeyChecking
#   StrictHostKeyChecking ask
StrictHostKeyChecking no
[root@c01 ~]# service sshd restart
Stopping sshd:                                             [  OK  ]
Starting sshd:                                             [  OK  ]


[root@c01 c01ssh]# cat /root/.ssh/id_rsa.pub >>/root/c01ssh/authorized_keys



# 3.测试




