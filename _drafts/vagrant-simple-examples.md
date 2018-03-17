---
layout: post
title: Vagrant使用的几个简单例子
date: 2018-03-17 12:00
author: felo
tags: vagrant virtualbox
typora-root-url: ../../yangfeiffei.github.io
---



# 1. Vagrant

Vagrant是一个基于Ruby的工具，用于创建和部署虚拟化开发环境。它默认可以使用Oracle的开源VirtualBox虚拟化系统，简单的部署需要的测试或者开发环境。

- 官网：[Vagrant](https://www.vagrantup.com/)
- 下载：[download](https://www.vagrantup.com/downloads.html)
- 文档：[docs](https://www.vagrantup.com/docs/index.html)



# 2. 简单的例子

## 2.1 创建虚拟机

想用`vagrant`创建虚拟机，首先需要有`box`，可以到网上下载现成的`box`也可以自己创建。

### 2.1.1 下载`box`或者创建`box`

可以到`https://app.vagrantup.com/boxes/search`挑选自己喜欢的镜像，其中还有使用的方法等。

```bash
$ mkdir new_dir
$ cd new_dir
$ vagrant init ubuntu/trusty64   # 自动下载，并初始化生成Vagrantfile
$ vagrant up # 启动虚拟机
```
在我的`vrtualbox`里有一个在用的虚拟机`new-lfs`，使用它来创建一个名为`LFS`的`box`。

```bash
localhost:my_lfs$ vagrant package -h
Usage: vagrant package [options] [name|id]

Options:

        --base NAME                  Name of a VM in VirtualBox to package as a base box (VirtualBox Only)
        --output NAME                Name of the file to output
        --include FILE,FILE..        Comma separated additional files to package with the box
        --vagrantfile FILE           Vagrantfile to package with the box
    -h, --help                       Print this help
localhost:my_lfs$ vagrant package --base new-lfs --output lfs.box
==> new-lfs: Attempting graceful shutdown of VM...
    new-lfs: Guest communication could not be established! This is usually because
    new-lfs: SSH is not running, the authentication information was changed,
    new-lfs: or some other networking issue. Vagrant will force halt, if
    new-lfs: capable.
==> new-lfs: Forcing shutdown of VM...
==> new-lfs: Exporting VM...
==> new-lfs: Compressing package to: /Users/yangfeilong/GitHub/my_lfs/lfs.box
localhost:my_lfs$ du -sh lfs.box 
1.1G	lfs.box
```

### 2.1.2 导入`box`

也可以直接下载别人创建好的`box`，然后导入到自己的`vagrant`环境中。

```bash
直接下载一个box文件
$ wget http://box_url/box_name.box
# 将这个box导入到自己的环境中
$ vagrant box add ./box_name.box
or 需要自定义box名称的话，增加--name
$ vagrant box add --name box_name ./box_name.box
查看box状态
$ vagrant box status
```
`vagrant box add` 用法
```bash
localhost:vag_test$ vagrant box add
This command was not invoked properly. The help for this command is
available below.

Usage: vagrant box add [options] <name, url, or path>

Options:

    -c, --clean                      Clean any temporary download files
    -f, --force                      Overwrite an existing box if it exists
        --insecure                   Do not validate SSL certificates
        --cacert FILE                CA certificate for SSL download
        --capath DIR                 CA certificate directory for SSL download
        --cert FILE                  A client SSL cert, if needed
        --location-trusted           Trust 'Location' header from HTTP redirects and use the same credentials for subsequent urls as for the initial one
        --provider PROVIDER          Provider the box should satisfy
        --box-version VERSION        Constrain version of the added box

The box descriptor can be the name of a box on HashiCorp's Vagrant Cloud,
or a URL, or a local .box file, or a local .json file containing
the catalog metadata.

The options below only apply if you're adding a box file directly,
and not using a Vagrant server or a box structured like 'user/box':

        --checksum CHECKSUM          Checksum for the box
        --checksum-type TYPE         Checksum type (md5, sha1, sha256)
        --name BOX                   Name of the box
    -h, --help                       Print this help

```
导入测试：
```bash
localhost:my_lfs$ vagrant box add --name LFS ./lfs.box 
==> box: Box file was not detected as metadata. Adding it directly...
==> box: Adding box 'LFS' (v0) for provider: 
    box: Unpacking necessary files from: file:///Users/myname/GitHub/my_lfs/lfs.box
==> box: Successfully added box 'LFS' (v0) for 'virtualbox'!
localhost:my_lfs$ vagrant box list
LFS              (virtualbox, 0)
```

### 2.1.3 启动虚拟机

```bash
# 使用init来创建Vagrantfile
localhost:my_lfs$ vagrant init
# 修改Vagrantfile，这里就修改一下box的名称，默认为base，修改为LFS
localhost:my_lfs$ cat Vagrantfile |grep vm.box
  config.vm.box = "base"
localhost:my_lfs$ vim Vagrantfile 
localhost:my_lfs$ cat Vagrantfile |grep vm.box
  config.vm.box = "LFS"
# 启动虚拟机
localhost:my_lfs$ vagrant up
Bringing machine 'default' up with 'virtualbox' provider...
==> default: Importing base box 'LFS'...
==> default: Matching MAC address for NAT networking...
==> default: Setting the name of the VM: my_lfs_default_1521257880384_55206
==> default: Fixed port collision for 22 => 2222. Now on port 2200.
==> default: Clearing any previously set network interfaces...
==> default: Preparing network interfaces based on configuration...
    default: Adapter 1: nat
==> default: Forwarding ports...
    default: 22 (guest) => 2200 (host) (adapter 1)
==> default: Booting VM...
==> default: Waiting for machine to boot. This may take a few minutes...
    default: SSH address: 127.0.0.1:2200
    default: SSH username: vagrant
    default: SSH auth method: private key
    some error...
# 由于我的这个lfs虚拟机缺少一些基本服务，虽然不能完全启动，但是虚拟机仍然可以启动的。
```
虽然能启动虚拟机，但是仍有一些问题，如不能正常配置IP，或者启动相应的服务等。所以需要对`Vagrantfile`进行调整。



# 3. Vagrantfile编写





# 4. 对虚拟机的基本操作

```bash
$ vagrant up   # 启动虚拟机
$ vagrant halt  # 关闭虚拟机
$ vagrant destroy   # 删除虚拟机
```

