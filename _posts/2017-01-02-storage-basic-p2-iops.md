---

layout: post
title: Storage Basics – Part II: IOPS
date:  2017-1-5 17:21:13
categories: storage
tags: storage
author: felo

---

* content
{:toc}

在这一系列的第一部分中，我讨论了在虚拟环境中存储性能的重要,并介绍了存储环境的一些基本测量方法。在第二部分中，我们将看看在VMware服务器整合环境是许多SQL环境，和VDI环境等等，最重要的存储设计的考虑是：IOPS。
如果像第一部分坚持以一个单盘为中心，IOPS是一个非常简单的衡量每秒钟完成多少次读写命令的参数。IOPS是共享存储环境（如VMware）和高交易率的工作量（如SQL）的一个重要的性能指标。由于硬盘必须遵守物理定律，所以一个给定配置的磁盘IOPS能力是可预测的。计算一个给定的磁盘IOPS公式是：
```
IOPS = 1000/(Seek Latency + Rotational Latency)
IOPS = 1000/(寻道延迟 + 旋转延迟)   延迟时间单位为：ms
```
不同的磁盘类型，质量，盘片数量等都是确切的因素。您可以查看市场上大多数驱动器的技术规格。作为一个例子，我有随机选择的希捷cheatah 15K SAS驱动器的技术规格。这种特殊的驱动器具有以下性能特点：


- 平均旋转延迟：2.0msec
- 平均读寻道时间（延迟）：3.4msec
- 平均写寻道时间（延迟）：3.9msec

使用读取延迟数，是这样的：
```
1000/(2.0+3.4) = 185          最大 read IOPS 
```
最大写入IOPS的就会少一点（~169iops），因为写寻道延迟更高一些，写作比读更“昂贵”，因此更慢。
幸运的是，有一些被广泛接受的“工作数字”，所以你不必要每个硬盘都计算一次。因为旋转延迟是基于旋转速度的，我们每分钟可以使用已发布的旋转（RPM，每分钟的旋转次数）的来猜测估计IOPS能力评级。典型的主轴速度（以RPM）和等效的IOPS如下：
RPM | IOPS 
-- | --
7,200     |     80 
10,000    |   130 
15,000    |  180 
SSD       |    2500 – 6000

不仅仅是机械旋转硬盘，还包括市场上的固态硬盘（SSD）。SSD的IOPS是一个范围，因为不同的科技型（SLC、MLC等）。你可以检查http://en.wikipedia.org/wiki/solid-state_drive的介绍，或让你的供应商提供更深入的技术信息。
如果你是新手（你还在读，恭喜！），你可以使用Windows电脑要打开性能监视器，能看到多少IOPS，查看磁盘转/秒”计数器。

如果你是在VMware ESX环境查看一些数据，看看esxtop和CMDS/s的输出。我发表了几篇关于使用esxtop在[这里](http://vmtoday.com/2009/09/the-skinny-on-esxtop/)和[这里](http://vmtoday.com/2009/09/esxtop-batch-mode-windows-perfmon/)。从PerfMon和esxtop数字让你看到很接近但被扭曲的一些东西，我们会在以后的文章中讨论。

现在我们看下实际情况：单盘在服务器上是很罕见的。因此我们将从单盘情况开始，扩展到企业级多盘存储系统。讨论多磁盘阵列的IOPS是一个伟大的开始。从一个最基本的角度来看，磁盘阵列就是结合多个硬盘驱动器，聚合他们的性能。例如，两块15K RPM硬盘一起工作，服务器工作负荷提供了理论360的IOPS（180 + 180）。这样表示10块15K RPM磁盘可以提供1800的IOPS，100块15K RPM的磁盘可以提供18000的IOPS。
**设计你的环境的时候，你的存储可以提供足够的IOPS要求的工作量是非常重要的**。如果你的工作是设计存储，那么需要考虑perfmon, top, iostat, esxtop和vscsiStats的数据。通常情况下至少需要24小时来收集性能数据，并且要在95%为起点，如果你的业务有不同的情况，可以使用更多的时间收集数据。所以用一个非常简单的方法，如果你的数据和计算结果显示1800的IOPS要求，你至少应该有十块15K RPM的磁盘（或二十三个7.2K RPM SATA磁盘）。一些简单的数据和一个漂亮的小的电子表格就可以帮助你理解和证明正确的硬件工作。

现在，在你开始设计一个新的存储系统之前，我们还有更多的一些事情需要讨论。RAID,cache以及存储的高级技术将改变这些值，我们需要去了解。Duncan Epping发布的一篇文章，几乎包括了所有的内容http://www.yellow-bricks.com/2009/12/23/iops/。还有http://virtualgeek.typepad.com/virtual_geek/2009/12/whats-what-in-vmware-view-and-vdi-land.html和http://vpivot.com/2009/09/18/storage-is-the-problem/。

---
本文翻译了vm today的一篇系列文章，原文地址如下：
[Storage Basics – Part I: An Introduction](http://vmtoday.com/2009/12/storage-basics-part-i-intro/)
[Storage Basics – Part II: IOPS](http://vmtoday.com/2009/12/storage-basics-part-ii-iops/#comments)










