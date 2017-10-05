---
layout: post
title: virtualbox配置共享磁盘
date: 2017-10-05 21:00
author: felo
tags: centos virtualbox
---


# 0.环境准备


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

# 1.配置共享磁盘


![](/images/vb_share_disk/新增磁盘.png)


![](/images/vb_share_disk/修改磁盘属性.png)





# 2.在linux中查看


在linux中检查磁盘对应情况

```bash
[root@c01 ~]# blkid
/dev/sda1: UUID="294e19f8-4974-48a5-b63b-1bf4a48d917d" TYPE="ext4"
/dev/sda2: UUID="NUVWTX-VXvj-FD1P-4PYt-bJlZ-wSKg-c5j1R8" TYPE="LVM2_member"
/dev/mapper/vg_c01-lv_root: UUID="7e963d1e-d294-4807-97bf-0d3bff8a0eff" TYPE="ext4"
/dev/mapper/vg_c01-lv_swap: UUID="e4aa1894-770e-481a-88ae-0af0152461b4" TYPE="swap"
[root@c01 ~]# ll /dev/disk/by-uuid/
total 0
lrwxrwxrwx 1 root root 10 Oct  5 23:12 294e19f8-4974-48a5-b63b-1bf4a48d917d -> ../../sda1
lrwxrwxrwx 1 root root 10 Oct  5 23:12 7e963d1e-d294-4807-97bf-0d3bff8a0eff -> ../../dm-0
lrwxrwxrwx 1 root root 10 Oct  5 23:12 e4aa1894-770e-481a-88ae-0af0152461b4 -> ../../dm-1
[root@c01 ~]# ll /dev/disk/by-id/
total 0
lrwxrwxrwx 1 root root  9 Oct  5 23:12 ata-VBOX_HARDDISK_VB2c219235-b5342f0a -> ../../sdd
lrwxrwxrwx 1 root root  9 Oct  5 23:12 ata-VBOX_HARDDISK_VB8be33791-dbd131d9 -> ../../sdc
lrwxrwxrwx 1 root root  9 Oct  5 23:12 ata-VBOX_HARDDISK_VBd1f49c4d-f7d10830 -> ../../sdb
lrwxrwxrwx 1 root root  9 Oct  5 23:12 ata-VBOX_HARDDISK_VBe837dfd9-75743ee5 -> ../../sda
lrwxrwxrwx 1 root root 10 Oct  5 23:12 ata-VBOX_HARDDISK_VBe837dfd9-75743ee5-part1 -> ../../sda1
lrwxrwxrwx 1 root root 10 Oct  5 23:12 ata-VBOX_HARDDISK_VBe837dfd9-75743ee5-part2 -> ../../sda2
lrwxrwxrwx 1 root root 10 Oct  5 23:12 dm-name-vg_c01-lv_root -> ../../dm-0
lrwxrwxrwx 1 root root 10 Oct  5 23:12 dm-name-vg_c01-lv_swap -> ../../dm-1
lrwxrwxrwx 1 root root 10 Oct  5 23:12 dm-uuid-LVM-TE5oTnS0nytbFWgPeKlSCFZuYxcCgPk80DjcBzJyQbULlWfG3QSa8DgSJO1MncCc -> ../../dm-1
lrwxrwxrwx 1 root root 10 Oct  5 23:12 dm-uuid-LVM-TE5oTnS0nytbFWgPeKlSCFZuYxcCgPk8jEuJ9InDraGakO6okGxG7OUaZXJVazHy -> ../../dm-0
lrwxrwxrwx 1 root root  9 Oct  5 23:12 scsi-SATA_VBOX_HARDDISK_VB2c219235-b5342f0a -> ../../sdd
lrwxrwxrwx 1 root root  9 Oct  5 23:12 scsi-SATA_VBOX_HARDDISK_VB8be33791-dbd131d9 -> ../../sdc
lrwxrwxrwx 1 root root  9 Oct  5 23:12 scsi-SATA_VBOX_HARDDISK_VBd1f49c4d-f7d10830 -> ../../sdb
lrwxrwxrwx 1 root root  9 Oct  5 23:12 scsi-SATA_VBOX_HARDDISK_VBe837dfd9-75743ee5 -> ../../sda
lrwxrwxrwx 1 root root 10 Oct  5 23:12 scsi-SATA_VBOX_HARDDISK_VBe837dfd9-75743ee5-part1 -> ../../sda1
lrwxrwxrwx 1 root root 10 Oct  5 23:12 scsi-SATA_VBOX_HARDDISK_VBe837dfd9-75743ee5-part2 -> ../../sda2

```