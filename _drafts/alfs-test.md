---
layout: post
title: ALFS TEST
date: 2018-06-17 12:00
author: felo
tags: LFS
typora-root-url: ../../yangfeiffei.github.io
---





# 0.env

```bash
[root@localhost ~]# cat /etc/redhat-release 
CentOS Linux release 7.4.1708 (Core) 
[root@localhost ~]# uname -a
Linux localhost 3.10.0-693.el7.x86_64 #1 SMP Tue Aug 22 21:09:27 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
```

need 3 files

```bash
[root@localhost ~]# mkdir /alfs
[root@localhost ~]# mount.vboxsf alfs /alfs
[root@localhost ~]# cd /alfs
[root@localhost alfs]# ls
jhalfs-2.4.tar.xz  LFSbook8.0SVN.zip  lfs-packages-8.0-rc1.tar
```

local cdrom repo

```bash
[root@localhost alfs]# vi /etc/yum.repos.d/CentOS-Base.repo 
...
[base-cdrom]
name=CentOS-$releasever - Base CDROM
baseurl=file:///mnt
enabled=1
gpgcheck=0
...
```

install some tools

```bash
[root@localhost alfs]# mount /dev/sr0 /mnt/
[root@localhost alfs]# yum --disablerepo=* --enablerepo=base-cdrom install wget vim
```

version-check.sh

```bash
[root@localhost ~]# cat > version-check.sh <<"EOF"

#!/bin/bash

# Simple script to list version numbers ofcritical development tools

export LC_ALL=C

bash --version | head -n1 | cut -d "" -f2-4

MYSH=$(readlink -f /bin/sh)

echo "/bin/sh -> $MYSH"

echo $MYSH | grep -q bash || echo"ERROR: /bin/sh does not point to bash"

unset MYSH

echo -n "Binutils: "; ld --version | head -n1 | cut -d" " -f3-

bison --version | head -n1

if [ -h /usr/bin/yacc ]; then

echo "/usr/bin/yacc -> `readlink -f/usr/bin/yacc`";

elif [ -x /usr/bin/yacc ]; then

echo yacc is `/usr/bin/yacc --version |head -n1`

else

echo "yacc not found"

fi

bzip2 --version 2>&1 < /dev/null| head -n1 | cut -d" " -f1,6-

echo -n "Coreutils: "; chown --version | head -n1 | cut -d")" -f2

diff --version | head -n1

find --version | head -n1

gawk --version | head -n1

if [ -h /usr/bin/awk ]; then

echo "/usr/bin/awk -> `readlink -f/usr/bin/awk`";

elif [ -x /usr/bin/awk ]; then

echo awk is `/usr/bin/awk --version | head-n1`

else

echo "awk not found"

fi

gcc --version | head -n1

g++ --version | head -n1

ldd --version | head -n1 | cut -d "" -f2- # glibc version

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

echo 'int main(){}' > dummy.c &&g++ -o dummy dummy.c

if [ -x dummy ]

then echo "g++ compilation OK";

else echo "g++ compilationfailed"; fi

rm -f dummy.c dummy

EOF
```

install texinfo

```bash
yum --disablerepo=* --enablerepo=base-cdrom install texinfo
```



# 1. 准备磁盘和文件系统

```bash
[root@localhost ~]# lsblk 
NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
...
sdb      8:16   0 20.6G  0 disk 
├─sdb1   8:17   0  256M  0 part 
└─sdb2   8:18   0 20.4G  0 part 
...
```

创建目录

```bash
[root@localhost ~]# export LFS=/mnt/lfs
[root@localhost ~]# echo $LFS
/mnt/lfs
[root@localhost ~]# mkdir -p $LFS
[root@localhost ~]# mkfs.ext4 /dev/sdb1
[root@localhost ~]# mkfs.ext4 /dev/sdb2
[root@localhost ~]# mount /dev/sdb2 $LFS
[root@localhost ~]# mkdir -p $LFS/boot
[root@localhost ~]# mount /dev/sdb1 $LSF/boot 
```



# 2.准备源码包



```bash
[root@localhost ~]# mkdir -p $LFS/sources
[root@localhost ~]# chmod a+wt $LFS/sources 
```





