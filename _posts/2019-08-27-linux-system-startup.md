---
layout: post
title:  系统启动
date: 2019-08-27 12:00
tags: linux
categories: linux-basics 
typora-root-url: ..
---

系统启动其实很复杂，要想每个过程都了解感觉还是很难的。

这里记录一下主要过程吧，将来水平提升了再来补充。

# 0. 分类和组合

- BIOS legacy & MBR & grub legacy & sysinit & initrd.img   --> Centos5
- BIOS legacy & MBR & grub legacy & upstart & initramfs    --> Centos6
- UEFI        & GPT & grub legacy & upstart & initramfs    --> Centos6
- BIOS legacy & MBR & grub2       & systemd & initramfs    --> Centos7
- BIOS legacy & GPT & grub2       & systemd & initramfs    --> Centos7   # 可选
- UEFI        & GPT & grub2       & systemd & initramfs    --> Centos7

# 1. 按下电源

在没按下电源之前，在机器已经通电的情况下，已经有一个bmc模块已经在运行了。通过bmc可以模拟开机的动作，也可以查看机器的状况。

在按下电源的时候，就会进入开机自检的状态，检查设备的CPU、硬盘等，如果发现严重的硬件故障，就会停止启动。

# 2. `BIOS`

通常启动到一定时候就会出现 Press F1 xxxx, F12 xxxxx的字样，选择怎样的启动方式：

- 硬盘
- usb
- dvd
- network
- ...


BIOS有两种模式：

- legacy模式，也就是传统的BIOS模式；
- UEFI模式；

# 3. `BootLoader`

不管是硬盘还是光驱，都需要BootLoader来启动。

这里就说硬盘了，硬盘不同的分区，还是有点区别的，mbr不支持2TB以上的磁盘，因此，现在GPT分区的硬盘也越来越多了。

## 3.1 `mbr BootLoader`


第一个扇区，512字节中的前446个字节就是`Boot`程序，是一段汇编代码，因为空间只有446个字节，太少了，它的基本思想就是指向其他位置。

```bash

# 这三个是硬盘上的位置是系统启动的关键位置。
mbr --->  mbr gap ---> /boot

# 1.mbr 主引导分区，512字节的第一扇区
# 2.mbr gap  主引导分区和第一个分区之间的空隙
# 3./boot分区，一般是第一个分区

```

使用`grub`工具可以简单的部署上面说的这些东西，其实就是一些文件。但是使用`grub`和`grub2`有点区别，需要注意一下。

### 3.1.1 `grub`

`grub`，一般指的是`grub legacy`，版本号为`0.97`。`Centos6`以及之前的版本使用。

`grub`把启动需要的阶段分成了3个步骤，`stage1`、`stage1_5`、`stage2`。简单的来说，

- `stage1`就是`mbr`的第一个分区的446字节二进制文件，作用是指向`stage1_5`。
- `stage1_5`,主要任务就是识别文件系统，包含了很多文件系统的驱动。保存在`mbr gap` 缝隙之中，作用是指向`stage2`。
- `stage2`，作用是加载`grub`配置文件，进入加载内核等。保存在`/boot`目录下。

其实三个阶段设计的文件都保存在`/boot`下，当指定`setup`命令安装`grub`的时候，会将相应的文件写到指定的位置上。

```bash
#大概步骤就是这样了：

stage1 ---> stage1_5 ---> stage2    ---> vmlinuz&initrd ---> ...
启动   ---> fs驱动   ---> grub配置  ---> 内核&更多驱动  ---> ...

MBR --> stage1_5( mbr gap ) --> /boot/grub/menu.lst --> vmlinuz --> ...

```

这个步骤是固定的，基本需要调整的就是`grub`的配置文件`/boot/grub/menu.lst`。


### 3.1.2 `grub2`

`grub 2.x`的版本，`RHEL7/Centos7`以后开始使用，替换了原先的`grub legacy`。

`grub2`的方式基本和`grub`差不多，毕竟一脉相承，但是不再使用`stage`文件，而是用`boot.img,core.img`代替。

- `boot.img`：这个文件基本跟`stage1`作用是一样的,作用就是跳转到`core.img`上；
- `core.img`：跟`stage1_5`基本差不多,在mbr的磁盘分区方式下，会放在`mbr gap` 缝隙中。`core.img`内会包含一些必要的模块，以便能够grub访问到`/boot/grub`目录。因为`grub2`使用了大量动态模块，`core.img`会足够小。当然，`core.img`里面还有不同，不细说了。

```bash
所以启动的步骤也基本类似的：
1，读取硬盘第一扇区，也就是加载了boot.img
2，跳转到mbr和第一个分区的缝隙中的core.img，加载必要的模块，启动grub2
3，跳转到/boot下，grub2启动并加载grub2.cfg的配置文件
4，加载vmlinuz内核等...

简单来说就是： BIOS --> MBR --> core.img ( mbr gap ) --> grub.cfg --> vmlinuz --> ...

```

同样的，常见的调整就是修改`grub2`的配置，不过`grub2`的配置文件相对复杂一些，一般不再直接修改`/boot/grub2/grub.cfg`文件，而是通过修改`/etc/grub/default`和`grub-mkconfig`来自动生成配置。

## 3.2 `GPT BootLoader`

对于`GPT`硬盘，一般会使用`UEFI`进行启动，或者`BIOS legacy`方式。这里只说明`BIOS`的方式，`UEFI`下次研究下再说。

### 3.2.1` grub`

不确定，应该是支持的吧...

### 3.2.2 `grub2`

- BIOS + GPT 模式

在`BIOS&GPT`硬盘下的`grub2`与`mbr`方式只有一点区别，就是`core.img`的位置，它放在了一个专用的分区之中，使用不同工具划分分区，需要添加不同的标签，这个分区大小`1M`即可。

```bash
# parted   -->  bios_grub
# gdisk    -->  ef02
# anconda图形界面  --> 需要指定为 BIOS Boot 的Filesystem
# 分区可能如下：

[root@vm-centos7 ~]# df -hT |grep -v tmpfs
Filesystem              Type      Size  Used Avail Use% Mounted on
/dev/mapper/centos-root xfs        17G  7.9G  9.2G  47% /
/dev/sda1               xfs      1014M  146M  869M  15% /boot

```

启动顺序为：

```bash
BIOS --> MBR --> core.img (EF02 partition) --> grub.cfg --> vmlinuz --> ...
```

- UEFI + GPT 模式

在UEFI&GPT的模式下，因为UEFI能够直接读取文件系统，不需要直接读取某个扇区中的内容，因此直接指向类似core.img的一个grub.efi文件来启动grub2程序，加载grub.cfg这个配置文件。

这个文件的位置是：`/boot/efi/EFI/centos/grubx64.efi`。

这种模式下的分区也有些区别，需要额外增加一个专用`efi`分区，格式为`fat32`，挂载位置是`/boot/efi`，大小自定默认200M。

```bash
# 这种模式下的分区，是这样的：
[root@cn2 ~]# df -hT |grep -v tmpfs
Filesystem              Type      Size  Used Avail Use% Mounted on
/dev/mapper/centos-root xfs        17G  1.2G   16G   7% /
/dev/sda2               xfs      1014M  138M  877M  14% /boot
/dev/sda1               vfat      200M   12M  189M   6% /boot/efi
```

启动顺序：

```bash
启动顺序为：UEFI --> grubx64.efi --> grub.cfg --> vmlinuz --> ...
```


# 4. `kernel & initrd`

## 4.1 vmlinuz 



## 4.2 initrd.img & initramfs



## 4.3 `sysinit` & `upstart` & `systemd`




# x 参考资料

- [GRUB2配置文件"grub.cfg"详解(GRUB2实战手册)](http://www.jinbuguo.com/linux/grub.cfg.html)
- [GPT 分区详解](http://www.jinbuguo.com/storage/gpt.html)
- [第14章 Linux开机详细流程](https://www.cnblogs.com/f-ck-need-u/p/7100336.html)
- [grub2详解(翻译和整理官方手册)](https://www.cnblogs.com/f-ck-need-u/p/7094693.html)




