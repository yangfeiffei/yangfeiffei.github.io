---
layout: post
title: ALFS测试
date: 2018-08-02 12:00
author: felo
tags: lfs
typora-root-url: ../../yangfeiffei.github.io
---

# 1.准备工作

## 1.1 准备安装包

- 下载` lfs book`

使用`svn`下载`lfs book`，目前最近的是8.2版本

```bash
[root@test ~]# svn co svn://svn.linuxfromscratch.org/LFS/tags/8.2
```

- 下载` lfs packages`

下载最近的8.2版本，300M多一些

```bash
wget http://ftp.lfs-matrix.net/pub/lfs/lfs-packages/lfs-packages-8.2.tar
```

- 下载`alfs`脚本

```bahs
svn co svn://svn.linuxfromscratch.org/ALFS/jhalfs/trunk jhalfs  # 貌似不好使
wget http://www.linuxfromscratch.org/alfs/downloads/jhalfs/stable/jhalfs-2.4.tar.xz  # 直接下载2.4版本
```

下载完成的安装包

```bash
[root@test lfs_book]# ls -l
total 380108
drwxr-xr-x 18 root root      4096 Aug  2 11:27 8.2
drwxr-xr-x 15 root root      4096 Aug  2 11:16 jhalfs  # 2.4版本的内容
-rw-r--r--  1 root root 389222400 Aug  2 11:12 lfs-packages-8.2.tar
```

## 1.2 准备环境

- 操作系统，`CENTOS6/RHEL6`不行，内核版本太低。

```bash
[root@test ~]# cat /etc/redhat-release
CentOS Linux release 7.4.1708 (Core)
[root@test ~]# uname -a
Linux localhost.localdomain 3.10.0-693.el7.x86_64 #1 SMP Tue Aug 22 21:09:27 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
```


准备一个用于安装alfs的一个磁盘，这里是sdb

```bash
[root@test ~]# lsblk /dev/sdb
NAME     MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sdb      8:16   0  100G  0 disk
├─sdb1   8:17   0  190M  0 part      # 用来做boot
├─sdb2   8:18   0  976M  0 part      # swap
└─sdb3   8:19   0 98.9G  0 part       # 根 /
```

检查操作系统包安装准备

```bash
[root@test ~]# cat version-check.sh
#!/bin/bash
# Simple script to list version numbers of critical development tools
export LC_ALL=C
bash --version | head -n1 | cut -d" " -f2-4
echo "/bin/sh -> `readlink -f /bin/sh`"
echo -n "Binutils: "; ld --version | head -n1 | cut -d" " -f3-
bison --version | head -n1
if [ -h /usr/bin/yacc ]; then
echo "/usr/bin/yacc -> `readlink -f /usr/bin/yacc`";
elif [ -x /usr/bin/yacc ]; then
echo yacc is `/usr/bin/yacc --version | head -n1`
else
echo "yacc not found"
fi
bzip2 --version 2>&1 < /dev/null | head -n1 | cut -d" " -f1,6-
echo -n "Coreutils: "; chown --version | head -n1 | cut -d")" -f2
diff --version | head -n1
find --version | head -n1
gawk --version | head -n1
if [ -h /usr/bin/awk ]; then
echo "/usr/bin/awk -> `readlink -f /usr/bin/awk`";
elif [ -x /usr/bin/awk ]; then
echo yacc is `/usr/bin/awk --version | head -n1`
else
echo "awk not found"
fi
gcc --version | head -n1
g++ --version | head -n1
ldd --version | head -n1 | cut -d" " -f2- # glibc version
grep --version | head -n1
gzip --version | head -n1
cat /proc/version
m4 --version | head -n1
make --version | head -n1
patch --version | head -n1
echo Perl `perl -V:version`
sed --version | head -n1
tar --version | head -n1
makeinfo --version | head -n1
xz --version | head -n1
echo 'main(){}' > dummy.c && g++ -o dummy dummy.c
if [ -x dummy ]
then echo "g++ compilation OK";
else echo "g++ compilation failed"; fi
rm -f dummy.c dummy
```

检查结果

```bash
[root@test ~]# bash version-check.sh
bash, version 4.1.2(1)-release
/bin/sh -> /bin/bash
Binutils: version 2.20.51.0.2-5.44.el6 20100205
bison (GNU Bison) 2.4.1
Usage: /usr/bin/yacc [options] filename

Options:
  -b file_prefix        set filename prefix (default "y.")
  -d                    write definitions (y.tab.h)
  -g                    write a graphical description
  -l                    suppress #line directives
  -o output_file        (default "y.tab.c")
  -p symbol_prefix      set symbol prefix (default "yy")
  -r                    produce separate code and table files (y.code.c)
  -t                    add debugging support
  -v                    write description (y.output)
  -V                    show version information and exit
yacc is
bzip2,  Version 1.0.5, 10-Dec-2007.
Coreutils:  8.4
diff (GNU diffutils) 2.8.1
find (GNU findutils) 4.4.2
GNU Awk 3.1.7
/usr/bin/awk -> /bin/gawk
gcc (GCC) 4.4.7 20120313 (Red Hat 4.4.7-17)
g++ (GCC) 4.4.7 20120313 (Red Hat 4.4.7-17)
(GNU libc) 2.12
grep (GNU grep) 2.20
gzip 1.3.12
Linux version 2.6.32-642.el6.x86_64 (mockbuild@x86-033.build.eng.bos.redhat.com) (gcc version 4.4.7 20120313 (Red Hat 4.4.7-17) (GCC) ) #1 SMP Wed Apr 13 00:51:26 EDT 2016
m4 (GNU M4) 1.4.13
GNU Make 3.81
patch 2.6
Perl version='5.10.1';
GNU sed version 4.2.1
tar (GNU tar) 1.23
makeinfo (GNU texinfo) 4.13
xz (XZ Utils) 4.999.9beta
g++ compilation OK
```

## 1.3 准备磁盘和目录

新磁盘分区并创建ext4文件系统

```bash
[root@test ~]# mkfs.ext4 /dev/sdb1
[root@test ~]# mkfs.ext4 /dev/sdb3
```

设置LFS根分区，并挂载目录

```bash
[root@test ~]# export LFS=/mnt
[root@test ~]# mount /dev/sdb3 $LFS
[root@test ~]# mkdir $LFS/boot
[root@test ~]# mount /dev/sdb1 $LFS/boot
```

准备源码放置目录，并将包解压到这个目录中

```bash
[root@test ~]# mkdir $LFS/sources
[root@test ~]# chmod -v a+wt $LFS/sources
mode of `/mnt/sources' changed to 1777 (rwxrwxrwt)
[root@test ~]# tar -xf lfs_book/lfs-packages-8.2.tar -C /mnt/sources/
```

准备lfsbook放到指定位置

```bash
[root@test mnt]# cp -r  /root/lfs_book/8.2  $LFS/LFSbooks
[root@test mnt]# ls /mnt/LFSbooks/
appendices        chapter02  chapter06  general.ent  lfs-latest.php     packages.ent        prologue     udev-lfs
aux-file-data.sh  chapter03  chapter07  images       make-aux-files.sh  patches.ent         README
bootscripts       chapter04  chapter08  index.xml    Makefile           pdf-fixups.sh       stylesheets
chapter01         chapter05  chapter09  INSTALL      obfuscate.sh       process-scripts.sh  tidy.conf
```

准备`alfs`，将`jhalfs`目录内容拷贝到`sources`

```bash
[root@test mnt]# cp -r /root/lfs_book/jhalfs $LFS/sources/
[root@test mnt]# cd /mnt/sources/
[root@test sources]# ls jhalfs/
BLFS        CLFS2   Config.in  FUNCTION_LIST          jhalfs   Makefile  pkgmngt      README.CLFS    README.PACKAGE_MANAGEMENT
CHEATSHEET  CLFS3   custom     HLFS                   LFS      menu      README       README.CUSTOM  TODO
CLFS        common  extras     install-blfs-tools.sh  LICENSE  optimize  README.BLFS  README.HLFS
```

创建一个编译目录

```bash
[root@test sources]# mkdir $LFS/LFSBuild
[root@test mnt]# ls $LFS
boot  LFSbooks  LFSBuild  lost+found  sources
```

## 1.4 准备用户

添加jhalfs用户

```bash
[root@test mnt]# useradd -m -k /dev/null jhalfs
[root@test mnt]# passwd jhalfs
[root@test mnt]# visudo
# 添加一行
jhalfs  ALL=(ALL)       ALL
```

# 2.开始编译

## 2.1 准备`ALFS`

首先准备和编译`jhalfs`

```bash
[root@test jhalfs]# pwd
/mnt/sources/jhalfs
[root@test jhalfs]# make
```

BOOK setting

![](/images/alfs-test/lfsbook-settings.PNG)

- 使用版本 systemd，试一下，不行拉倒
- release  workcopy，并填写位置，存档book的位置

GENERAL settings

![](/images/alfs-test/general-settings.PNG)

- build目录：存放编译好的文件
- package目录：存放本地下载的包



Build setting

![](/images/alfs-test/build-settings.PNG)



可能会缺少xsltproc和ncurce包，进行安装

```bash
[root@test jhalfs]# yum install libxslt
[root@test jhalfs]# yum install ncurses-devel
```

完成会输出大概如下

```bash
Creating Makefile... START
        Processing... <Chapter4     ( SETUP ) >
        Processing... <Chapter5     ( LUSER ) >
        Processing... <Chapter6     ( CHROOT ) >
        Processing... <Chapter7/8   ( BOOT ) >
Creating Makefile... DONE
------------------------------------------------------------------------------


KERNEL............ <3.10.0-693.el7.x86_64> OK (Min version: 3.2)
BASH.............. <4.2.46(2)-release>   OK (Min version: 3.2)
GCC............... <4.8.5>               OK (Min version: 4.7)
G++............... <4.8.5>               OK (Min version: 4.7)
GLIBC............. <2.17>                OK (Min version: 2.11)
BINUTILS.......... <2.25.1-31.base.el7>  OK (Min version: 2.17)
TAR............... <1.26>                OK (Min version: 1.22)
BZIP2............. <1.0.6>               OK (Min version: 1.0.4)
BISON............. <3.0.4>               OK (Min version: 2.3)
COREUTILS......... <8.22>                OK (Min version: 6.9)
DIFF.............. <3.3>                 OK (Min version: 2.8.1)
FIND.............. <4.5.11>              OK (Min version: 4.2.31)
GAWK.............. <4.0.2>               OK (Min version: 4.0.1)
GREP.............. <2.20>                OK (Min version: 2.5.1a)
GZIP.............. <1.5>                 OK (Min version: 1.3.12)
M4................ <1.4.16>              OK (Min version: 1.4.10)
MAKE.............. <3.82>                OK (Min version: 3.81)
PATCH............. <2.7.1>               OK (Min version: 2.5.4)
PERL.............. <5.16.3>              OK (Min version: 5.8.8)
SED............... <4.2.2>               OK (Min version: 4.1.5)
TEXINFO........... <5.1>                 OK (Min version: 4.7)
XZ................ <5.2.2>               OK (Min version: 5.0.0)
------------------------------------------------------------------------------
```

## 2.2 编译软件包

开始编译软件包

```bash
[root@test jhalfs-2.4]# su - jhalfs
-bash-4.2$ cd /mnt/LFSBuild/
-bash-4.2$ make -C /mnt/LFSBuild/jhalfs/
开始自动编译软件包
```

输出如下所示大概

```bash
--------------------------------------------------------------------------------
 Building target 020-creatingtoolsdir

 Target 020-creatingtoolsdir OK
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
 Building target 021-addinguser

 Target 021-addinguser OK
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
 Building target 022-settingenvironment

 Target 022-settingenvironment OK
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
mk_LUSER
You are going to log into the user account lfs
sudo requires a password
make: Entering directory `/mnt/LFSBuild/jhalfs'
--------------------------------------------------------------------------------
 Building target 034-binutils-pass1
 [+++++++++++++++++++++++|                                    ] 3 min. 23 sec
 Target 034-binutils-pass1 OK
--------------------------------------------------------------------------------
...
```

时间很长，耐心等待。。。半天吧。。。

如果中间意外断开的话，再次启动即可。
```bash
[root@test jhalfs-2.4]# su - jhalfs
-bash-4.2$ cd /mnt/LFSBuild/
-bash-4.2$ make -C /mnt/LFSBuild/jhalfs/
开始自动编译软件包
```



## 2.2 形成`LFS`系统



拷贝目录到$LFS中

```bash

```

激活和挂载dev

```bash

```

`chroot`

```bash

```

创建`/etc/fstab`文件

```bash

```



## 2.4 编译内核



























