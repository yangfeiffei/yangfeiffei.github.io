---
layout: post
title:  用户管理和sudo
date: 2019-08-30 12:00
tags: linux
categories: linux-basics 
---



# 0. 概述

用户和权限相关的内容而已。


# 1. 用户

在Linux中可以随意切换用户，而且十分方便。管理用户必须使用root。

## 1.1 创建和修改用户

创建用户：

```bash
# 直接创建一个用户，默认会增加一个同名的组
[root@vm-centos7 home]# useradd test1
[root@vm-centos7 home]# id test1
uid=1002(test1) gid=1002(test1) groups=1002(test1)
# 创建用户的时候指定一些参数
[root@vm-centos7 home]# useradd -u 1100 -g test1 -m -s /bin/bash test2
#  -u 指定用户id，一般1000以后，1000以前系统用；
#  -g 指定gid，-G指定groups GID
#  -m 创建home目录
#  -s 指定shell
```
修改用户：

```bash
# 修改用户的组
[root@vm-centos7 home]# usermod -G root,test1 test2
[root@vm-centos7 home]# id test2
uid=1100(test2) gid=1002(test1) groups=1002(test1),0(root)
```

## 1.2 删除用户


## 1.3 其他特别的地方

- useradd和usermod差别很小，几乎命令都差不多；
- adduser也是增加用户的命令；
- -k命令

```bash
 -k, --skel SKEL_DIR           use this alternative skeleton directory
```

- 创建用户的同时可以修改密码策略

```bash
  -e, --expiredate EXPIRE_DATE  expiration date of the new account
  -f, --inactive INACTIVE       password inactivity period of the new account
```

# 2. 权限

## 2.1 文件和目录的权限



## 2.2 特殊权限


## 2.3 修改权限


# 3. 用户密码

## 3.1 `/etc/passwd`

## 3.2 其他

# 4. `sudo`

# 5. `PAM`


# x. 参考





